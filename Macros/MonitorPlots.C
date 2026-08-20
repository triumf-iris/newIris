// #include "header_40Ar.h"
// #include "header_37Cl.h"
// #include "header_37K.h"
#include "header_12Cd.h"
// #include "header_41K.h"
#include "nucleus.h"
#include "dedx.h"
#include "eloss.h" // for target thickness calculation
#include "CalibPHYSICS.h"
#include "geometry.h"
#include "runDepPar.h"
#include "IrisMaterial.h"
#include <fstream>
#include <algorithm>
#include "TCutG.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraphErrors.h"
#include "TH1.h"
#include "TMath.h"
#include "TROOT.h"
#include "TSpectrum.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TString.h"
#include "TVirtualFitter.h"
#include "TPolyMarker.h"

gSystem->Load("libIRISCore.so");

// Modified function to return mean and its error
std::pair<Double_t, Double_t> PlotAndFitGaussian(const std::string &filename, const std::string &xVariable, Int_t Bin, Int_t xMin, Int_t xMax, Int_t var)
{
    // Open the ROOT file
    TFile file(filename.c_str());
    if (file.IsZombie())
    {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return std::make_pair(-1, -1);
    }

    // Get the tree from the file
    TTree *tree = (TTree *)file.Get("Iris");
    if (!tree)
    {
        std::cerr << "Error: Tree not found in file " << filename << "\n";
        return std::make_pair(-1, -1);
    }

    // Create a canvas to draw the histogram
    TCanvas *canvas = new TCanvas("canvas", "Histogram", 800, 600);

    std::string selection;

    if (var == 1)
    {
        selection = "TICEnergy > " + std::to_string(ICMin) + " && TICEnergy < " + std::to_string(ICMax) + " && TSd1rChannel == " + std::to_string(Sd1ring);
    }
    else if (var == 2)
    {
        selection = "TICEnergy > " + std::to_string(ICMin) + " && TICEnergy < " + std::to_string(ICMax) + "&& TSd2rChannel == " + std::to_string(Sd2ring);
    }
    else
    {
        selection = "TICEnergy > " + std::to_string(ICMin) + " && TICEnergy < " + std::to_string(ICMax);
    }

    // Create a histogram
    TH1F *hist = new TH1F("hist", "Histogram", Bin, xMin, xMax);

    // Build the draw command
    std::string drawCommand = xVariable + ">>hist";

    // Draw the histogram
    tree->Draw(drawCommand.c_str(), selection.c_str());

    // Use TSpectrum to find peaks in the histogram
    TSpectrum spectrum;
    Int_t nPeaks = spectrum.Search(hist, 2, "", 0.5); // Peak finding parameters can be tuned
    if (nPeaks == 0)
    {
        std::cerr << "Error: No peaks found in the histogram.\n";
        return std::make_pair(-1, -1);
    }

    // Get the position of the highest peak
    Double_t *peakPositions = spectrum.GetPositionX();
    Double_t peakPosition = peakPositions[0]; // Assume the first peak is the most prominent

    std::cout << "Found peak at position: " << peakPosition << std::endl;

    Double_t minrange = peakPosition - peakPosition * 0.1;
    Double_t maxrange = peakPosition + peakPosition * 0.1;

    // Fit a Gaussian using the peak position as the initial guess for the mean
    //  TF1* gausFit = new TF1("gausFit", "gaus", xMin, xMax);
    TF1 *gausFit = new TF1("gausFit", "gaus", minrange, maxrange);
    gausFit->SetParameter(1, peakPosition); // Set the initial guess for the mean

    hist->Fit(gausFit, "R"); // Fit the Gaussian to the histogram, restricting to the range

    // Draw the histogram with the fit
    hist->Draw();

    // Get the mean value and error of the fitted Gaussian
    Double_t mean = gausFit->GetParameter(1);     // Mean of the Gaussian
    Double_t meanError = gausFit->GetParError(1); // Error in the mean

    // Clean up
    delete canvas;
    delete hist;

    // Return the mean value and its error
    return std::make_pair(mean, meanError);
}

Double_t PlotICEnergy(const std::string &filename)
{
    std::string IC = "TICEnergy";

    // Call the PlotAndFitGaussian function with IC energy variable, binning, and selection
    std::pair<Double_t, Double_t> ICMean = PlotAndFitGaussian(filename, IC, ICBin, ICMin, ICMax, 0);
    Double_t gaussianMean = ICMean.first;
    // Return the fitted Gaussian mean value for the IC energy plot
    return gaussianMean;
}

// Function to plot and fit Gaussian for S3 energy plot
std::pair<std::pair<Double_t, Double_t>, std::pair<Double_t, Double_t>> PlotS3Energy(const std::string &filename)
{
    // Define the variable for S3 energy
    std::string sd1 = "TSd1rEnergy";
    std::string sd2 = "TSd2rEnergy";

    // Call the PlotAndFitGaussian function with S3 energy variable, binning, and selection
    std::pair<Double_t, Double_t> sd1Result = PlotAndFitGaussian(filename, sd1, Sd1Bin, Sd1Min, Sd1Max, 1);
    std::pair<Double_t, Double_t> sd2Result;
    if (tgt_stopin_Sd1 == 1)
    {
        sd2Result = {0.0, 0.0};
    }
    if (tgt_stopin_Sd1 == 0)
    {
        sd2Result = PlotAndFitGaussian(filename, sd2, Sd2Bin, Sd2Min, Sd2Max, 2);
    }

    // Return the fitted Gaussian mean values and errors for the S3 energy plot
    return std::make_pair(sd1Result, sd2Result);
}

// Function to calculate target thickness using energy loss from S3 detectors
std::pair<Double_t, Double_t> CalculateTargetThickness(const std::string &filename)
{
    Double_t eTgt[100], dedxTgt[100];
    Double_t eAg[100], dedxAg[100];
    Double_t eAl[100], dedxAl[100];
    Double_t eB[100], dedxB[100];
    Double_t eP[100], dedxP[100];
    Double_t eSiO2[100], dedxSiO2[100];
    //Double_t T = Theta; // Ring in S3

    std::string Name0 = isotope;
    //Double_t mass0 = isotopemass;

    CalibPHYSICS calPhys;
    runDep runDepPar;
    geometry geoP;
    calPhys.Load(CalibFile);
	runDepPar.setRunDepPar(calPhys.fileRunDepPar); // setting run dependent parameters.
	geoP.ReadGeometry(calPhys.fileGeometry.data());

	Double_t T = TMath::RadToDeg() * atan((geoP.SdInnerRadius * (24. - (Sd1ring + 0.5)) + geoP.SdOuterRadius * (Sd1ring + 0.5)) / 24. / (geoP.Sd1Distance + 14.8));

    if (runDepPar.na == "1H")
        TrgtD = 0.0868;
    else if (runDepPar.na == "2H")
        TrgtD = 0.201;
    else {
        cout << "NO TARGET DEFINED IN runDepPar FILE!" << endl;
        cout << "ABORTING!" << endl;
        std::exit(0);
    }

    nucleus beam;

    beam.getInfo("", runDepPar.nA);
    
	if (calPhys.booldedx == kTRUE)
    {
        std::cout << "Using dedx tables at " << calPhys.filededx << std::endl;
		auto dedxstr = calPhys.filededx;
	    beam.EL.loadIncomingELoss(dedxstr, beam.name.data(), geoP.MFoil, geoP.MTgt, beam.mass);
    }
    else
    {
		std::cout << "Using dedx tables from catima" << std::endl;
		beam.EL.makeCatimaTables(beam.A, beam.Z, geoP.MFoil, geoP.MTgt, beam.mass);
    }

    //std::string eLossFileTgt = "/home/iris/LISE_dedx/lise_" + isotope + "_in_" + TgtName + ".txt"; // Stopping power table generated with LISE++
    //std::string eLossFileAg = "/home/iris/LISE_dedx/lise_" + isotope + "_in_Ag.txt";               // Stopping power table generated with LISE++
    //std::string eLossFileAl = "/home/iris/LISE_dedx/lise_" + isotope + "_in_Al.txt";               // Stopping power table generated with LISE++

    //loadELoss(Form("/home/iris/LISE_dedx/lise_%s_in_Al.txt", Name0.c_str()), eAl, dedxAl, mass0);
    //loadELoss(Form("/home/iris/LISE_dedx/lise_%s_in_B.txt", Name0.c_str()), eB, dedxB, mass0);
    //loadELoss(Form("/home/iris/LISE_dedx/lise_%s_in_P.txt", Name0.c_str()), eP, dedxP, mass0);
    //loadELoss(Form("/home/iris/LISE_dedx/lise_%s_in_SiO2.txt", Name0.c_str()), eSiO2, dedxSiO2, mass0);
    //loadELoss(Form("/home/iris/LISE_dedx/lise_%s_in_Ag.txt", Name0.c_str()), eAg, dedxAg, mass0);
    //loadELoss(eLossFileTgt, eTgt, dedxTgt, mass0);

    auto s3Results = PlotS3Energy(filename);
    Double_t meanS1 = s3Results.first.first;    // Mean for TSd1rEnergy
    Double_t meanS2 = s3Results.second.first;   // Mean for TSd2rEnergy
    Double_t errorS1 = s3Results.first.second;  // Error for TSd1rEnergy
    Double_t errorS2 = s3Results.second.second; // Error for TSd2rEnergy

    Double_t M = TBE0;
    Double_t energy = meanS2;
    Double_t energyError = errorS2;

    // Energy corrections
    energy = energy + elossFi(beam, energy, 0.0911 / T, IrisMaterial::P);
    energy = energy + elossFi(beam, energy, 0.08106 / T, IrisMaterial::Al);
    energy = energy + elossFi(beam, energy, 0.08106 / T, IrisMaterial::Al);
    energy = energy + elossFi(beam, energy, 0.0911 / T, IrisMaterial::P);

    // Add meanS1 to energy
    energy += meanS1;
    energyError = sqrt(energyError * energyError + errorS1 * errorS1);

    std::cout << "Energy is " << energy << std::endl;

    energy = energy + elossFi(beam, energy, 0.11751 / T, IrisMaterial::B);
    energy = energy + elossFi(beam, energy, 0.08106 / T, IrisMaterial::Al);
    energy = energy + elossFi(beam, energy, 0.9275 / T, IrisMaterial::SiO2);
    energy = energy + elossFi(beam, energy, 0.4053 / T, IrisMaterial::Al);

    std::cout << "Energy before Ag is " << energy << std::endl;

    Double_t S3Total = energy;

    if (upstream == 1)
        energy = energy + elossFi(beam, energy, geoP.FoilThickness / T, IrisMaterial::Foil);
    std::cout << "The reconstructed value of meanTBE is " << energy << std::endl;

    Double_t targetThickness = thickness(beam, M, energy, IrisMaterial::Target) * 10. / TrgtD;
    std::cout << "Calculated target thickness: " << targetThickness << " um" << std::endl;
    std::cout << "Error in energy thickness: " << energyError << " um" << std::endl;
    // Estimate error in thickness due to error in energy
    Double_t energyShift = energy + energyError;
    Double_t targetThicknessHigh = thickness(beam, TBE0, energyShift, IrisMaterial::Target) * 10.0 / TrgtD;
    Double_t thicknessError = fabs(targetThicknessHigh - targetThickness);

    if (std::isinf(targetThickness))
    {
        std::cerr << "Error: Target thickness is infinite. Returning -1." << std::endl;
        return std::make_pair(-1, -1);
    }

    return std::make_pair(targetThickness, thicknessError);
}

// Function to calculate particle ratio to IC scalar
Double_t CalculateParticleRatioToICScalar(const std::string &filename, Int_t gate)
{
    // Open the ROOT file
    TFile file(filename.c_str());
    if (file.IsZombie())
    {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return -1;
    }

    // Get the tree from the file
    TTree *tree = (TTree *)file.Get("Iris");
    if (!tree)
    {
        std::cerr << "Error: Tree not found in file " << filename << "\n";
        return -1;
    }

    std::string GateName1 = "protons";
    std::string GateName2 = "deuterons";
    std::string GateName3 = "tritons";
    std::string GateName4 = "alphas";

    std::string Selection = "TICEnergy > " + std::to_string(ICMin) + " && TICEnergy < " + std::to_string(ICMax);
    std::string GateName;

    if (gate == 1)
    {
        if(processGate1 == "")
            return -1;
        gROOT->ProcessLine(processGate1.c_str());
        GateName = GateName1;
    }
    else if (gate == 2)
    {
        if(processGate2 == "")
            return -1;
        gROOT->ProcessLine(processGate2.c_str());
        GateName = GateName2;
    }
    else if (gate == 3)
    {
        if(processGate3 == "")
            return -1;
        gROOT->ProcessLine(processGate3.c_str());
        GateName = GateName3;
    }
    else
    {
        if(processGate4 == "")
            return -1;
        gROOT->ProcessLine(processGate4.c_str());
        GateName = GateName4;
    }

    std::string particleCountSelection = GateName + " && " + Selection;
    Int_t particleCount = tree->GetEntries(particleCountSelection.c_str());

    std::cout << "Number of particles gate: " << particleCount << std::endl;

    TH1F *histICSum = new TH1F("histICSum", "TICEnergy", ICBin, ICMin, ICMax);
    tree->Draw("TICEnergy>>histICSum");

    // Find the bin with the highest count (the IC scalar)
    // Int_t highestBin = histICSum->GetMaximumBin();  // Get the bin with the maximum count
    // Double_t ICScalar = histICSum->GetXaxis()->GetBinCenter(highestBin);  // Get the value of ICSum at the highest bin

    Double_t ICScalar = histICSum->GetEntries();

    std::cout << "IC counts: " << ICScalar << std::endl;

    if (ICScalar == 0)
    {
        std::cerr << "Error: IC scalar is zero, cannot compute ratio.\n";
        return -1;
    }

    Double_t particleRatio = static_cast<Double_t>(particleCount) / ICScalar;
    std::cout << "Particle ratio to IC scalar: " << particleRatio << std::endl;

    // Clean up
    delete histICSum;

    return particleRatio;
}

std::set<Int_t> ReadProcessedRuns(const std::string &outputFilename)
{
    std::set<Int_t> processedRuns;
    std::ifstream inFile(outputFilename);

    if (!inFile)
    {
        std::cout << "No existing output file found, will create a new one." << std::endl;
        return processedRuns; // Return empty set if no file exists
    }

    std::string line;
    while (getline(inFile, line))
    {
        if (line.find("RunNumber") != std::string::npos)
            continue; // Skip the header line
        Int_t runNumber;
        std::istringstream ss(line);
        ss >> runNumber;                 // Extract the run number from the line
        processedRuns.insert(runNumber); // Store the run number in the set
    }

    inFile.close();
    return processedRuns;
}

// Function to read data from the output file and return vectors of values
void ReadDataFromFile(const std::string &filename, std::vector<int> &runNumbers, std::vector<double> &icPeaks,
                      std::vector<double> &s3Peaks, std::vector<double> &ratiosP,
                      std::vector<double> &ratiosD, std::vector<double> &ratiosT, std::vector<double> &ratiosA, std::vector<double> &thicknesses, std::vector<double> &thicknessError)
{
    std::ifstream inFile(filename);
    if (!inFile)
    {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    std::string line;
    // Skip the header line
    std::getline(inFile, line);

    // Read data
    int runNumber;
    double icPeak, s3Peak, ratioP, ratioD, ratioT, ratioA, thickness, Error;
    while (inFile >> runNumber >> icPeak >> s3Peak >> ratioP >> ratioD >> ratioT >> ratioA >> thickness >> Error)
    {
        runNumbers.push_back(runNumber);
        icPeaks.push_back(icPeak);
        s3Peaks.push_back(s3Peak);
        ratiosP.push_back(ratioP);
        ratiosD.push_back(ratioD);
        ratiosT.push_back(ratioT);
        ratiosA.push_back(ratioA);
        thicknesses.push_back(thickness);
        thicknessError.push_back(Error);
    }

    inFile.close();
}

int MonitorPlots()
{

    ROOT::EnableImplicitMT();

    std::set<Int_t> processedRuns = ReadProcessedRuns(outputFilename);
    std::ofstream outFile(outputFilename, std::ios_base::app);
    if (!outFile)
    {
        std::cerr << "Error: Could not open the output file " << outputFilename << std::endl;
        return 1; // Return 1 to indicate error
    }

    // If the file is empty, write the header
    if (processedRuns.empty())
    {
        outFile << "RunNumber\tICPeak\tS3Peak\tratioP\tratioD\tratioT\tratioA\tTargetThickness\tTargetThicknessError\n";
    }

    for (Int_t i = 0; i < nRuns; ++i)
    {
        // Check if the run has already been processed
        if (processedRuns.find(runs[i]) != processedRuns.end())
        {
            std::cout << "Run " << runs[i] << " already processed, skipping." << std::endl;
            continue; // Skip this run
        }
        std::string filename = treeDir + "tree0" + std::to_string(runs[i]) + ".root";
        std::cout << "Processing file: " << filename << std::endl;

        Double_t icPeak = PlotICEnergy(filename);
        // std::pair<Double_t, Double_t> s3Means = PlotS3Energy(filename);
        auto s3Means = PlotS3Energy(filename);
        Double_t meanS1 = s3Means.first.first;  // Mean for TSd1rEnergy
        Double_t meanS2 = s3Means.second.first; // Mean for TSd2rEnergy
        Double_t S3Total = meanS1 + meanS2;

        auto thicknessResult = CalculateTargetThickness(filename);
        Double_t thickness = thicknessResult.first;
        Double_t thicknessError = thicknessResult.second;
        Double_t ratioP = CalculateParticleRatioToICScalar(filename, 1);
        Double_t ratioD = CalculateParticleRatioToICScalar(filename, 2);
        Double_t ratioT = CalculateParticleRatioToICScalar(filename, 3);
        Double_t ratioA = CalculateParticleRatioToICScalar(filename, 4);

        outFile << runs[i] << "\t" << icPeak << "\t" << S3Total << "\t" << ratioP << "\t" << ratioD << "\t" << ratioT << "\t" << ratioA << "\t" << thickness << "\t" << thicknessError << "\t" << "\n";

        std::cout << "Run: " << runs[i] << " | IC Peak: " << icPeak << " | S3 Peak: " << S3Total << " | Target Thickness: " << thickness << std::endl;
    }

    outFile.close();

    // Vectors to store data
    std::vector<int> runNumbers;
    std::vector<double> icPeaks, s3Peaks, ratiosP, ratiosD, ratiosT, ratiosA, thicknesses, thicknessError;
    // Initialize error1 with 0s (no errors along x-axis)
    std::vector<double> error1(runNumbers.size(), 0);
    // Read data from file
    ReadDataFromFile(outputFilename, runNumbers, icPeaks, s3Peaks, ratiosP, ratiosD, ratiosT, ratiosA, thicknesses, thicknessError);

    // Convert runNumbers from int to double (for compatibility with TGraph)
    std::vector<double> runNumbersDouble(runNumbers.begin(), runNumbers.end());

    // Create a canvas and divide it into 6 sections (2x3 layout)
    TCanvas *canvas = new TCanvas("canvas", "Run Data Plots", 1200, 800);
    canvas->Divide(3, 2); // Divide canvas into 6 sections (3 rows, 2 columns)

    // Create TGraph for Run Number vs IC Peak
    TGraph *grICPeak = new TGraph(runNumbersDouble.size(), &runNumbersDouble[0], &icPeaks[0]);
    grICPeak->SetTitle("Run Number vs IC Peak;Run Number;IC Peak");
    grICPeak->SetMarkerStyle(21); // Set square markers
    grICPeak->SetMarkerSize(1);

    // Create TGraph for Run Number vs S3 Peak
    TGraph *grS3Peak = new TGraph(runNumbersDouble.size(), &runNumbersDouble[0], &s3Peaks[0]);
    grS3Peak->SetTitle("Run Number vs S3 Peak;Run Number;S3 Peak");
    grS3Peak->SetMarkerStyle(21); // Set square markers
    grS3Peak->SetMarkerSize(1);

    // Create TGraph for Run Number vs Thickness
    TGraphErrors *grThickness = new TGraphErrors(runNumbersDouble.size(), &runNumbersDouble[0], &thicknesses[0], &error1[0], &thicknessError[0]);
    grThickness->SetTitle("Run Number vs Thickness;Run Number;Thickness");
    grThickness->SetMarkerStyle(21); // Set square markers
    grThickness->SetMarkerSize(1);

    // Create TGraph for Run Number vs RatioP
    TGraph *grRatioP = new TGraph(runNumbersDouble.size(), &runNumbersDouble[0], &ratiosP[0]);
    grRatioP->SetTitle("Run Number vs RatioP;Run Number;RatioP");
    grRatioP->SetMarkerStyle(21); // Set square markers
    grRatioP->SetMarkerSize(1);

    // Create TGraph for Run Number vs RatioD
    TGraph *grRatioD = new TGraph(runNumbersDouble.size(), &runNumbersDouble[0], &ratiosD[0]);
    grRatioD->SetTitle("Run Number vs RatioD;Run Number;RatioD");
    grRatioD->SetMarkerStyle(21); // Set square markers
    grRatioD->SetMarkerSize(1);

    // Create TGraph for Run Number vs RatioP
    TGraph *grRatioT = new TGraph(runNumbersDouble.size(), &runNumbersDouble[0], &ratiosT[0]);
    grRatioT->SetTitle("Run Number vs RatioT;Run Number;RatioT");
    grRatioT->SetMarkerStyle(21); // Set square markers
    grRatioT->SetMarkerSize(1);

    // Create TGraph for Run Number vs RatioP
    TGraph *grRatioA = new TGraph(runNumbersDouble.size(), &runNumbersDouble[0], &ratiosA[0]);
    grRatioA->SetTitle("Run Number vs RatioA;Run Number;RatioA");
    grRatioA->SetMarkerStyle(21); // Set square markers
    grRatioA->SetMarkerSize(1);

    TFile *file = new TFile(rootOutputFile.c_str(), "RECREATE");
    if (file->IsZombie())
    {
        std::cerr << "Error: Could not open the ROOT file." << std::endl;
        return 1;
    }

    // Write each graph individually to the ROOT file
    grICPeak->Write("ICPeakGraph");
    grS3Peak->Write("S3PeakGraph");
    grThickness->Write("ThicknessGraph");
    grRatioP->Write("RatioPGraph");
    grRatioD->Write("RatioDGraph");
    grRatioT->Write("RatioTGraph");
    grRatioA->Write("RatioAGraph");

    // Plot each graph in its respective section
    canvas->cd(1);        // Go to the first section
    grICPeak->Draw("AP"); // "AP" means draw axis and points

    canvas->cd(2); // Go to the second section
    grS3Peak->Draw("AP");

    canvas->cd(3);
    grThickness->Draw("AP");

    canvas->cd(4);
    grRatioP->Draw("AP");

    canvas->cd(5);
    grRatioD->Draw("AP");

    canvas->cd(6);
    grRatioT->Draw("AP");

    canvas->Write("AllGraphsTogether");

    file->Close();

    std::cout << "Graphs saved in " << rootOutputFile << " successfully!" << std::endl;

    canvas->Update(); // Update the canvas to reflect the changes
    return 0;
}
