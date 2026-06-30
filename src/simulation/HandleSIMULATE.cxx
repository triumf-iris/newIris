#include <stdlib.h>
#include <fstream>
#include <array>

#include "TFile.h"
#include "TTree.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TLorentzVector.h"
#include "TRandom3.h"
#include "TGenPhaseSpace.h"
#include "TStopwatch.h"
#include "YYHit.h"
#include "CsIHit.h"
#include "IDet.h"
#include "PTrack.h"
#include "S3Hit.h"
#include "header.h"
#include "detHits.h"
#include "eloss.h"
#include "IrisMaterial.h"
#include "CalibPHYSICS.h"
#include "geometry.h"
#include "runDepPar.h"
#include "HandleSIMULATE.h"

extern TFile *file;
extern TTree *Iris;

CalibPHYSICS calPhys;
geometry geoP;
runDep runDepPar; // run dependant parameters
YYHit yd, yu;
CsIHit csi;
S3Hit sd1, sd2, su;
PTrack blP, tlP;
PTrack blDecP, tlDecP1, tlDecP2;
PTrack buP1, buP2, buP3, buP4;
IDet det;
// IPhys phys;

nucleus A, a, B, b, c, d, e, f, decB, decc, decd;

TGenPhaseSpace PS0, PS1;
TLorentzVector LorVb;
TLorentzVector LorVB;
TLorentzVector LorVBdec;
TLorentzVector LorVcdec;
TLorentzVector LorVddec;
TLorentzVector LorVcbu;
TLorentzVector LorVdbu;
TLorentzVector LorVebu;
TLorentzVector LorVfbu;
TVector3 reacPos;

Double_t mA = 0.;
Double_t ma = 0.;
Double_t mB = 0.;
Double_t mBR = 0.;
Double_t mbR = 0.;
Double_t mb = 0.;
Double_t mc = 0.;
Double_t md = 0.;
Double_t me = 0.;
Double_t mf = 0.;
Double_t mBdec = 0., mcdec = 0., mddec = 0.;

Int_t Evnt = 0;
Double_t chck = 0.;
Double_t chck2 = 0.;
Double_t wght = 0.;
Double_t wght2 = 0.;

Double_t ICdE;
YYHit *ipyd = &yd;
YYHit *ipyu = &yu;
CsIHit *ipcsi = &csi;
S3Hit *ipsd1 = &sd1;
S3Hit *ipsd2 = &sd2;
S3Hit *ipsu = &su;
PTrack *iptlP = &tlP;
PTrack *ipblP = &blP;
PTrack *ipblDecP = &blDecP;
PTrack *iptlDecP1 = &tlDecP1;
PTrack *iptlDecP2 = &tlDecP2;
PTrack *ipbuP1 = &buP1;
PTrack *ipbuP2 = &buP2;
PTrack *ipbuP3 = &buP3;
PTrack *ipbuP4 = &buP4;
IDet *ipdet = &det;
// IPhys *ipphys = &phys;

TLorentzVector *LVb = &LorVb;
TLorentzVector *LVB = &LorVB;
TLorentzVector *LVBdec = &LorVBdec;
TLorentzVector *LVcdec = &LorVcdec;
TLorentzVector *LVddec = &LorVddec;
TLorentzVector *LVcbu = &LorVcbu;
TLorentzVector *LVdbu = &LorVdbu;
TLorentzVector *LVebu = &LorVebu;
TLorentzVector *LVfbu = &LorVfbu;

Bool_t isSHTReac = kFALSE;

Bool_t seqdec = kFALSE;
Int_t seqdecN = 0;
Double_t masses2[3];

void clearEvt()
{

	mBR = 0.;
	mbR = 0.;

	tlP.Clear();
	blP.Clear();
	blDecP.Clear();
	tlDecP1.Clear();
	tlDecP2.Clear();
	buP1.Clear();
	buP2.Clear();
	buP3.Clear();
	buP4.Clear();
	yd.Clear();
	yu.Clear();
	csi.Clear();
	sd1.Clear();
	sd2.Clear();
	su.Clear();
	det.Clear();
	// phys.Clear();
	LorVb.Clear();
	LorVB.Clear();
	LorVBdec.Clear();
	LorVcdec.Clear();
	LorVddec.Clear();
	LorVcbu.Clear();
	LorVdbu.Clear();
	LorVebu.Clear();
	LorVfbu.Clear();
	// reacPos;
	PS0.Clear();
	PS1.Clear();

	return;
}

void HandleBOR_SIMULATE(std::string BinPath, std::string CalibFile, std::string OutputFile, Int_t Run)
{
	printf("In HandleBOR_SIMULATE...\n");
	if (CalibFile == "")
		printf("No calibration file specified!\n\n");
	calPhys.Load(CalibFile);
	calPhys.Print();

	// *******************************************************************
	// Set up output file and tree
	file = new TFile(OutputFile.data(), "RECREATE");
	Iris = new TTree("Iris", "Iris simulation");

	Iris->Branch("Evnt", &Evnt, "Evnt/I");
	Iris->Branch("Run", &Run, "Run/I");
	Iris->Branch("tlP.", &iptlP, 32000, 99);
	Iris->Branch("blP.", &ipblP, 32000, 99);
	Iris->Branch("blDecP.", &ipblDecP, 32000, 99);
	Iris->Branch("tlDecP1.", &iptlDecP1, 32000, 99);
	Iris->Branch("tlDecP2.", &iptlDecP2, 32000, 99);
	Iris->Branch("buP1.", &ipbuP1, 32000, 99);
	Iris->Branch("buP2.", &ipbuP2, 32000, 99);
	Iris->Branch("buP3.", &ipbuP3, 32000, 99);
	Iris->Branch("buP4.", &ipbuP4, 32000, 99);
	Iris->Branch("wght", &wght, "wght/D");
	Iris->Branch("yd.", &ipyd, 32000, 99);
	Iris->Branch("yu.", &ipyu, 32000, 99);
	Iris->Branch("csi.", &ipcsi, 32000, 99);
	Iris->Branch("sd1.", &ipsd1, 32000, 99);
	Iris->Branch("sd2.", &ipsd2, 32000, 99);
	Iris->Branch("su.", &ipsu, 32000, 99);
	Iris->Branch("det", &ipdet, 32000, 99);
	// Iris->Branch("phys", &ipphys, 32000, 99);

	// treeFile = new TFile(OutputFile.data(), "RECREATE");
	// tree = input_chain->CloneTree(0);
	// IrisEvent = new TEvent();
	// tree->Branch("IrisEvent", "TEvent", &IrisEvent, 32000, 99);

	if (calPhys.boolRunDepPar)
	{
		runDepPar.setRunDepPar(calPhys.fileRunDepPar); // setting run dependent parameters.
		runDepPar.Print();

		isSHTReac = runDepPar.SHT;

		if (isSHTReac)
		{
			A.getInfo(BinPath, runDepPar.nA);
			A.Print();
			a.getInfo(BinPath, runDepPar.na);
			a.Print();
			B.getInfo(BinPath, runDepPar.nB);
			B.Print();
			b.getInfo(BinPath, runDepPar.nb);
			b.Print();
		}
		else
		{
			A.getInfo(BinPath, runDepPar.nA);
			A.Print();
			a.getInfo(BinPath, runDepPar.nfoil);
			a.Print();
			B.getInfo(BinPath, runDepPar.nB);
			B.Print();
			b.getInfo(BinPath, runDepPar.nfoil);
			b.Print();
		}
		if (runDepPar.N > 2)
		{
			c.getInfo(BinPath, runDepPar.nc);
			c.Print();
		}
		if (runDepPar.N > 3)
		{
			d.getInfo(BinPath, runDepPar.nd);
			d.Print();
		}
		if (runDepPar.N > 4)
		{
			e.getInfo(BinPath, runDepPar.ne);
			e.Print();
		}
		if (runDepPar.N > 5)
		{
			f.getInfo(BinPath, runDepPar.nf);
			f.Print();
		}

		mA = A.mass / 1000.;
		ma = a.mass / 1000.;
		mB = B.mass / 1000. + runDepPar.R1 / 1000.;
		mBR = mB;
		mb = b.mass / 1000. + runDepPar.R2 / 1000.;
		mbR = mb;
		mc = c.mass / 1000.;
		md = d.mass / 1000.;
		me = e.mass / 1000.;
		mf = f.mass / 1000.;
		Printf("print A mass %lf and a mass %lf \n", A.mass, a.mass);
		Printf("print B mass %lf and b mass %lf \n", B.mass, b.mass);

		geoP.ReadGeometry(calPhys.fileGeometry.data());
		geoP.AoZTgt = a.A / a.Z;
		geoP.Print();

		// Check for sequential decays ****************************************
		Double_t S_low = 50.;
		Int_t pick = 0;

		if (B.Sn != 0. && B.Sn < S_low)
		{
			S_low = B.Sn;
			pick = 1;
		}
		if (B.Sp != 0. && B.Sp < S_low)
		{
			S_low = B.Sp;
			pick = 2;
		}
		if (B.S2n != 0. && B.S2n < S_low)
		{
			S_low = B.S2n;
			pick = 3;
		}
		if (B.S2p != 0. && B.S2p < S_low)
		{
			S_low = B.S2p;
			pick = 4;
		}
		printf("\nResonance Energy: %.2lf\tlowest threshold: %.2lf\n", runDepPar.R1, S_low);

		if (S_low < runDepPar.R1)
		{
			switch (pick)
			{
			case 1:
				seqdec = kTRUE;
				printf("\nSequential 1n- decay!\n\n");
				seqdecN = 2;
				decB.getInfo(BinPath, B.N - 1, B.Z);
				decB.Print();
				mBdec = decB.mass / 1000.;
				decc.getInfo(BinPath, "n");
				decc.Print();
				mcdec = decc.mass / 1000.;
				break;
			case 2:
				seqdec = kTRUE;
				printf("\nSequential 1p- decay!\n\n");
				seqdecN = 2;
				decB.getInfo(BinPath, B.N, B.Z - 1);
				decB.Print();
				mBdec = decB.mass / 1000.;
				decc.getInfo(BinPath, "p");
				decc.Print();
				mcdec = decc.mass / 1000.;
				break;
			case 3:
				seqdec = kTRUE;
				printf("\nSequential 2n- decay!\n\n");
				seqdecN = 3;
				decB.getInfo(BinPath, B.N - 2, B.Z);
				decB.Print();
				mBdec = decB.mass / 1000.;
				decc.getInfo(BinPath, "n");
				decc.Print();
				mcdec = decc.mass / 1000.;
				decd.getInfo(BinPath, "n");
				decd.Print();
				mddec = decd.mass / 1000.;
				break;
			case 4:
				seqdec = kTRUE;
				printf("\nSequential 2p- decay!\n\n");
				seqdecN = 3;
				decB.getInfo(BinPath, B.N, B.Z - 2);
				decB.Print();
				mBdec = decB.mass / 1000.;
				decc.getInfo(BinPath, "p");
				decc.Print();
				mcdec = decc.mass / 1000.;
				decd.getInfo(BinPath, "p");
				decd.Print();
				mddec = decd.mass / 1000.;
				break;
			default:
				seqdec = kFALSE;
				break;
			}
			masses2[0] = mBdec;
			masses2[1] = mcdec;
			masses2[2] = mddec;
		}

		printf("Beam energy: %f\n", runDepPar.energy);
		// printf("Target thickness: %f\n",geoP.TargetThickness);

		if (calPhys.booldedx == kTRUE)
		{
			auto dedxstr = calPhys.filededx;
			A.EL.loadIncomingELoss(dedxstr, A.name.data(), geoP.MFoil, geoP.MTgt, A.mass);
			b.EL.loadOutgoingELoss(dedxstr, b.name.data(), geoP.MFoil, geoP.MTgt, b.mass);
			if (!seqdec)
				B.EL.loadOutgoingELoss(dedxstr, B.name.data(), geoP.MFoil, geoP.MTgt, B.mass);
			else
			{
				decB.EL.loadOutgoingELoss(dedxstr, decB.name.data(), geoP.MFoil, geoP.MTgt, decB.mass);
				if (decc.Z > 0)
					decc.EL.loadOutgoingELoss(dedxstr, decc.name.data(), geoP.MFoil, geoP.MTgt, decc.mass);
				if (seqdecN > 2 && decd.Z > 0)
					decd.EL.loadOutgoingELoss(dedxstr, decd.name.data(), geoP.MFoil, geoP.MTgt, decd.mass);
			}
			if (runDepPar.N > 2 && c.Z > 0)
				c.EL.loadOutgoingELoss(dedxstr, c.name.data(), geoP.MFoil, geoP.MTgt, c.mass);
			if (runDepPar.N > 3 && d.Z > 0)
				d.EL.loadOutgoingELoss(dedxstr, d.name.data(), geoP.MFoil, geoP.MTgt, d.mass);
			if (runDepPar.N > 4 && e.Z > 0)
				e.EL.loadOutgoingELoss(dedxstr, e.name.data(), geoP.MFoil, geoP.MTgt, e.mass);
			if (runDepPar.N > 5 && f.Z > 0)
				f.EL.loadOutgoingELoss(dedxstr, f.name.data(), geoP.MFoil, geoP.MTgt, f.mass);
		}
		else
		{
#ifndef USE_CATIMA
			std::cout << "ERROR: dedx tables not supplied!" << std::endl;
			file->Close();
			return;
#endif
			std::cout << "Using dedx tables from catima" << std::endl;
			A.EL.makeCatimaTables(A.A, A.Z, geoP.MFoil, geoP.MTgt, A.mass);
			b.EL.makeCatimaTables(b.A, b.Z, geoP.MFoil, geoP.MTgt, b.mass);
			if (!seqdec)
				B.EL.makeCatimaTables(B.A, B.Z, geoP.MFoil, geoP.MTgt, B.mass);
			else
			{
				decB.EL.makeCatimaTables(decB.A, decB.Z, geoP.MFoil, geoP.MTgt, decB.mass);
				if (decc.Z > 0)
					decc.EL.makeCatimaTables(decc.A, decc.Z, geoP.MFoil, geoP.MTgt, decc.mass);
				if (seqdecN > 2 && decd.Z > 0)
					decd.EL.makeCatimaTables(decd.A, decd.Z, geoP.MFoil, geoP.MTgt, decd.mass);
			}
			if (runDepPar.N > 2 && c.Z > 0)
				c.EL.makeCatimaTables(c.A, c.Z, geoP.MFoil, geoP.MTgt, c.mass);
			if (runDepPar.N > 3 && d.Z > 0)
				d.EL.makeCatimaTables(d.A, d.Z, geoP.MFoil, geoP.MTgt, d.mass);
			if (runDepPar.N > 4 && e.Z > 0)
				e.EL.makeCatimaTables(e.A, e.Z, geoP.MFoil, geoP.MTgt, e.mass);
			if (runDepPar.N > 5 && f.Z > 0)
				f.EL.makeCatimaTables(f.A, f.Z, geoP.MFoil, geoP.MTgt, f.mass);
		}
	}

	// calculateBeamEnergy(A, runDepPar.energy);

	if (calPhys.boolICGates == kFALSE)
	{
		runDepPar.ICmin = 0;
		runDepPar.ICmax = 4096;
	}

	//--------------------------------------------------------------------------------
	printf("End of HandleBOR_SIMULATE\n");
} // HandleBOR_Physics

void HandleSIMULATE(int nsim, std::string dwbaname)
{
	TRandom3 *rndm = new TRandom3(0);

	/*std::array<std::ofstream, static_cast<int>(IrisMaterial::count)> tableAOuts;
	std::array<std::ofstream, static_cast<int>(IrisMaterial::count)> tableaOuts;
	std::array<std::ofstream, static_cast<int>(IrisMaterial::count)> tableBOuts;
	std::array<std::ofstream, static_cast<int>(IrisMaterial::count)> tablebOuts;

	char inumA[20];
	char inuma[20];
	char inumB[20];
	char inumb[20];
	*/

	Double_t BeamSpot = geoP.Bs / 2.355;				  // FWHM->sigma
	Double_t ICLength = 22.9 * 0.00318 * geoP.ICPressure; // cm*mg/cm^3
	const Double_t ICWindow1 = 0.05 * 3.44 * 0.1;		  // mu*g/cm^3*0.1 //new IC window thickness 50nm
	const Double_t ICWindow2 = 0.05 * 3.44 * 0.1;		  // mu*g/cm^3*0.1

	// tlP.nuc = b;
	// blP.nuc = B;

	yd.Init(geoP.YdThickness);
	yu.Init(geoP.YuThickness);
	sd1.Init(0, geoP.Sd1Thickness);
	sd2.Init(1, geoP.Sd2Thickness);
	su.Init(0, geoP.SuThickness);

	// Bool_t LEHit;
	Bool_t HEHit;

	// Int_t LEHitcntr=0;
	Int_t HEHitcntr = 0;

	// Double_t LEeff, HEeff;
	Double_t E_after_IC = 0.;
	Double_t E_before_SSB = 0.;
	Double_t E_before_Tgt = 0.;
	Double_t E_center_Tgt = 0.;
	Double_t E_after_Tgt = 0.;
	Double_t E_before_Foil = 0.;
	Double_t E_center_Foil = 0.;
	Double_t E_after_Foil = 0.;

	TLorentzVector target, beam, Sys;
	TVector3 boostvect, sdshift;

	Double_t wght_max, width1, width2;
	Bool_t allowed;

	Double_t reacX, reacY, reacZ;

	// Calculate energy loss up to center of the target
	Double_t EA = runDepPar.EBAC;
	EA -= eloss(A, 0.5, EA, ICWindow1, IrisMaterial::Si3N4);
	ICdE = eloss(A, 0.586, EA, ICLength, IrisMaterial::C4H10);
	EA -= ICdE;
	EA -= eloss(A, 0.5, EA, ICWindow2, IrisMaterial::Si3N4);
	E_after_IC = EA;

	if (isSHTReac)
	{
		if (geoP.TargetOrientation == 1)
		{
			E_before_Tgt = EA;
			EA -= eloss(A, 1., EA, geoP.TargetThickness / 2., IrisMaterial::Target);
			E_center_Tgt = EA;
			EA -= eloss(A, 1., EA, geoP.TargetThickness / 2., IrisMaterial::Target);
			E_after_Tgt = EA;
		}

		E_before_Foil = EA;
		E_center_Foil = EA - eloss(A, 1. / geoP.AoZFoil, EA, geoP.FoilThickness / 2., IrisMaterial::Foil);
		EA -= eloss(A, 1. / geoP.AoZFoil, EA, geoP.FoilThickness, IrisMaterial::Foil);
		// E_before_Tgt = EA;
		E_after_Foil = EA;
		if (geoP.TargetOrientation == 1)
			E_before_SSB = E_after_Foil;

		if (geoP.TargetOrientation == 0)
		{
			E_before_Tgt = E_after_Foil;
			E_center_Tgt = E_after_Foil - eloss(A, 1., E_after_Foil, geoP.TargetThickness / 2., IrisMaterial::Target);
			E_after_Tgt = E_after_Foil - eloss(A, 1., E_after_Foil, geoP.TargetThickness, IrisMaterial::Target);
			E_before_SSB = E_after_Tgt;
		}
	}
	else
	{
		E_before_Foil = E_after_IC;
		EA -= eloss(A, 1. / geoP.AoZFoil, EA, geoP.FoilThickness / 2., IrisMaterial::Foil);
		E_center_Foil = EA;
		EA -= eloss(A, 1. / geoP.AoZFoil, EA, geoP.FoilThickness / 2., IrisMaterial::Foil);
		E_after_Foil = EA;

		E_before_Tgt = EA;
		E_center_Tgt = EA;
		E_after_Tgt = EA;
		E_before_SSB = E_after_Tgt;

		reacZ = geoP.TargetThickness / 2.;
	}

	EA = EA / 1000.; // convert to GeV for TGenPhaseSpace
	Double_t PA = sqrt(EA * EA + 2 * EA * mA);
	target.SetXYZT(0.0, 0.0, 0.0, ma);
	beam.SetXYZT(0.0, 0.0, PA, mA + EA);
	Sys = beam + target;
	// phys.beamE = EA * 1000.;
	// phys.beamBeta = Sys.Beta();
	// phys.beamGamma = Sys.Gamma();
	// phys.beamEcm = EA * ma * 1000. / (mA + ma);

	printf("\nEnergy after IC window: %.2lf MeV\n", E_after_IC);
	if (geoP.TargetOrientation == 1)
	{
		printf("Energy before target: %.2lf MeV\n", E_before_Tgt);
		printf("Energy at center of target: %.2lf MeV\n", E_center_Tgt);
		printf("Energy at behind target: %.2lf MeV\n", E_after_Tgt);
	}
	printf("Energy before foil: %.2lf MeV\n", E_before_Foil);
	printf("Energy at center of foil: %.2lf MeV\n", E_center_Foil);
	printf("Energy after foil: %.2lf MeV\n", E_after_Foil);
	if (geoP.TargetOrientation == 0)
	{
		printf("Energy before target: %.2lf MeV\n", E_before_Tgt);
		printf("Energy at center of target: %.2lf MeV\n", E_center_Tgt);
		printf("Energy at behind target: %.2lf MeV\n", E_after_Tgt);
	}

	// printf("\nBeta at center of target: %.3lf \n", phys.beamBeta);
	// printf("Gamma at center of target: %.3lf \n", phys.beamGamma);
	// printf("CM Energy at center of target: %.2lf MeV\n\n", phys.beamEcm);

	printf("YY1 detector at distance of %.1lf mm from target, covering theta range from %.2lf to %.2lf\n", geoP.YdDistance, yd.ThetaMin(geoP.YdDistance), yd.ThetaMax(geoP.YdDistance));
	printf("CsI detector at distance of %.1lf mm from target, covering theta range from %.2lf to %.2lf\n", geoP.YdDistance + 11.6, csi.ThetaMin(geoP.YdDistance + 11.6), csi.ThetaMax(geoP.YdDistance + 11.6));
	printf("First S3 detector at distance of %.1lf mm from target, covering theta range from %.2lf to %.2lf\n", geoP.Sd1Distance, sd1.ThetaMin(geoP.Sd1Distance), sd1.ThetaMax(geoP.Sd1Distance));
	printf("Second S3 detector at distance of %.1lf mm from target, covering theta range from %.2lf to %.2lf\n", geoP.Sd1Distance + 14.8, sd2.ThetaMin(geoP.Sd1Distance + 14.8), sd2.ThetaMax(geoP.Sd1Distance + 14.8));
	printf("Upstream YY1 detector at distance of %.1lf mm from target, covering theta range from %.2lf to %.2lf\n", geoP.YuDistance, yu.ThetaMin(geoP.YuDistance), yu.ThetaMax(geoP.YuDistance));
	printf("Upstream S3 detector at distance of %.1lf mm from target, covering theta range from %.2lf to %.2lf\n\n", geoP.SuDistance, su.ThetaMin(geoP.SuDistance), su.ThetaMax(geoP.SuDistance));

	Double_t masses[6] = {mb, mB, mc, md, me, mf};

	Double_t tht = 0.;
	Double_t xsec = 0.;
	Double_t xsec_chck = 0.;
	Double_t xsec_max = 0.;
	Double_t dwba_th[181] = {0.};
	Double_t dwba_xsec[181] = {0.};

	if (dwbaname != "")
	{
		printf("Using DWBA cross section from %s!\n", dwbaname.data());
		xsec_max = load_dwba(dwbaname, dwba_th, dwba_xsec);
	}

	allowed = PS0.SetDecay(Sys, runDepPar.N, masses);

	if (!allowed)
	{
		printf("Impossible decay!\n");
		printf("Exiting...\n");
		exit(0);
	}
	else
	{
		printf("Starting...\n");
	}

	wght_max = PS0.GetWtMax();
	printf("%lf\t%lf\n", wght_max, xsec_max);
	width1 = runDepPar.W1 / 1000.;
	width2 = runDepPar.W2 / 1000.;
	printf("%lf\t%lf\t%lf\t%lf\n", mB, width1, mb, width2);

	Int_t whilecount;
	// Start of event loop
	while (Evnt < nsim)
	{
		if (isSHTReac)
		{
			// reacZ = geoP.FoilThickness/2.;

			reacZ = rndm->Uniform(0, geoP.TargetThickness);
			if (geoP.TargetOrientation == 0)
			{
				EA = E_after_IC - eloss(A, 1. / geoP.AoZFoil, E_before_Foil, geoP.FoilThickness, IrisMaterial::Foil);
			}
			if (geoP.TargetOrientation == 1)
			{
				EA = E_after_IC;
			}
			EA = EA - eloss(A, b.Z / b.A, E_before_Tgt, reacZ, IrisMaterial::Target);
		}
		else
		{

			reacZ = rndm->Uniform(0, geoP.FoilThickness);
			EA = E_after_IC - eloss(A, 1. / geoP.AoZFoil, E_before_Foil, reacZ, IrisMaterial::Foil);
			// reacZ = geoP.TargetThickness/2.;
		}

		EA = EA / 1000.; // convert to GeV for TGenPhaseSpace
		PA = sqrt(EA * EA + 2 * EA * mA);

		target.SetXYZT(0.0, 0.0, 0.0, ma);
		beam.SetXYZT(0.0, 0.0, PA, mA + EA);
		Sys = beam + target;
		boostvect = Sys.BoostVector();

		// phys.beamE = EA * 1000.;
		// phys.beamBeta = Sys.Beta();
		// phys.beamGamma = Sys.Gamma();
		// phys.beamEcm = EA * ma * 1000. / (mA + ma);

		// width = runDepPar.W/1000.;

		wght = 0.;
		clearEvt();
		if (runDepPar.SHAPE == 0)
			mBR = rndm->BreitWigner(mB, width1);
		else if (runDepPar.SHAPE == 1)
			mBR = rndm->Gaus(mB, width1);
		else if (runDepPar.SHAPE == 2)
			mBR = rndm->Uniform(mB - width1, mB + width1);
		else
			mBR = rndm->BreitWigner(mB, width1);
		masses[1] = mBR;
		if (runDepPar.SHAPE == 0)
			mbR = rndm->BreitWigner(mb, width2);
		else if (runDepPar.SHAPE == 1)
			mbR = rndm->Gaus(mb, width2);
		else if (runDepPar.SHAPE == 2)
			mbR = rndm->Uniform(mb - width2, mb + width2);
		else
			mbR = rndm->BreitWigner(mb, width2);
		masses[0] = mbR;
		PS0.SetDecay(Sys, runDepPar.N, masses); // recalculate with resonance energy
		// wght_max=PS0.GetWtMax();

		TLorentzVector *LTmp;
		whilecount = 0;
		do
		{
			wght = PS0.Generate();
			if (wght != wght)
				continue; // catch NaNs
			chck = rndm->Uniform(0, 1);
			if (dwbaname != "")
			{
				LTmp = PS0.GetDecay(0);
				LTmp->Boost(-boostvect);
				tht = TMath::RadToDeg() * LTmp->Theta();
				xsec = eval_theta(tht, dwba_th, dwba_xsec);
				xsec_chck = rndm->Uniform(0, xsec_max);
				LTmp->Boost(boostvect);
			}
			else
			{
				xsec = 1.;
				xsec_chck = 0.;
			}
			whilecount++;
			// printf("%d\t%f\t%f\t%f\n",whilecount,tht,xsec,xsec_chck);
		} while (wght < chck || xsec < xsec_chck);

		LVb = PS0.GetDecay(0);
		LVB = PS0.GetDecay(1);

		TLorentzVector Frag = Sys - *LVb;

		// phys.Qgen = (mA + ma - mb - Frag.M()) * 1000.;

		tlP.T = LVb->Theta();
		blP.T = LVB->Theta();
		tlP.E = (LVb->E() - mb) * 1000.;
		blP.E = (LVB->E() - mB) * 1000.;
		tlP.P = LVb->Phi();
		blP.P = LVB->Phi();

		// Convert angles to degrees for root file
		tlP.Tdeg = TMath::RadToDeg() * tlP.T;
		blP.Tdeg = TMath::RadToDeg() * blP.T;
		tlP.Pdeg = TMath::RadToDeg() * tlP.P;
		blP.Pdeg = TMath::RadToDeg() * blP.P;

		if (seqdec)
		{
			PS1.SetDecay(*LVB, seqdecN, masses2);
			do
			{
				wght2 = PS1.Generate();
				chck2 = rndm->Uniform(0, 1);
				LVBdec = PS1.GetDecay(0);
				LVcdec = PS1.GetDecay(1);
				if (seqdecN > 2)
					LVddec = PS1.GetDecay(2);
			} while (wght2 < chck2);
			blDecP.T = LVBdec->Theta();
			blDecP.E = (LVBdec->E() - mBdec) * 1000.;
			blDecP.P = LVBdec->Phi();
			blDecP.Tdeg = TMath::RadToDeg() * blDecP.T;
			blDecP.Pdeg = TMath::RadToDeg() * blDecP.P;
			tlDecP1.T = LVcdec->Theta();
			tlDecP1.E = (LVcdec->E() - mcdec) * 1000.;
			tlDecP1.P = LVcdec->Phi();
			tlDecP1.Tdeg = TMath::RadToDeg() * tlDecP1.T;
			tlDecP1.Pdeg = TMath::RadToDeg() * tlDecP1.P;
			if (seqdecN > 2)
			{
				tlDecP2.T = LVddec->Theta();
				tlDecP2.E = (LVddec->E() - mddec) * 1000.;
				tlDecP2.P = LVddec->Phi();
				tlDecP2.Tdeg = TMath::RadToDeg() * tlDecP2.T;
				tlDecP2.Pdeg = TMath::RadToDeg() * tlDecP2.P;
			}
		}
		if (runDepPar.N > 2) // 3body
		{
			LVcbu = PS0.GetDecay(2);

			buP1.T = LVcbu->Theta();
			buP1.E = (LVcbu->E() - mc) * 1000.;
			buP1.P = LVcbu->Phi();

			// Convert angles to degrees for root file
			buP1.Tdeg = TMath::RadToDeg() * buP1.T;
			buP1.Pdeg = TMath::RadToDeg() * buP1.P;
		}
		if (runDepPar.N > 3) // 4body
		{
			LVdbu = PS0.GetDecay(3);

			buP2.T = LVdbu->Theta();
			buP2.E = (LVdbu->E() - md) * 1000.;
			buP2.P = LVdbu->Phi();

			// Convert angles to degrees for root file
			buP2.Tdeg = TMath::RadToDeg() * buP2.T;
			buP2.Pdeg = TMath::RadToDeg() * buP2.P;
		}
		if (runDepPar.N > 4) // 5body
		{
			LVdbu = PS0.GetDecay(4);

			buP3.T = LVebu->Theta();
			buP3.E = (LVebu->E() - me) * 1000.;
			buP3.P = LVebu->Phi();

			// Convert angles to degrees for root file
			buP3.Tdeg = TMath::RadToDeg() * buP3.T;
			buP3.Pdeg = TMath::RadToDeg() * buP3.P;
		}
		if (runDepPar.N > 5) // 6body
		{
			LVdbu = PS0.GetDecay(5);

			buP4.T = LVfbu->Theta();
			buP4.E = (LVfbu->E() - mf) * 1000.;
			buP4.P = LVfbu->Phi();

			// Convert angles to degrees for root file
			buP4.Tdeg = TMath::RadToDeg() * buP4.T;
			buP4.Pdeg = TMath::RadToDeg() * buP4.P;
		}

		// XY position on target in mm
		reacX = BeamSpot * rndm->Gaus();
		reacY = BeamSpot * rndm->Gaus();
		double adjustZpos = 0;
		double convertMM = 0;
		// reacZ is in units of mg/cm^2 and needed for energy loss corrections below
		// adjust Z position around middle of target and convert to mm
		if (reacZ < geoP.TargetThickness / 2.)
			adjustZpos = -reacZ;
		else if (reacZ > geoP.TargetThickness / 2.)
			adjustZpos = reacZ - geoP.TargetThickness / 2.;
		else if (reacZ == geoP.TargetThickness / 2.)
			adjustZpos = 0;
		if (geoP.MTgt == "D")
			convertMM = (1. / 0.201) * (10.) * (1. / 1000.);
		else if (geoP.MTgt == "H")
			convertMM = (1. / 0.0867) * (10.) * (1. / 1000.);
		else
			printf("ERROR: Cannot convert target thickness to mm!");
		reacPos.SetXYZ(reacX, reacY, adjustZpos * convertMM); // all in units of mm

		if (!seqdec)
		{
			blP = TgtELoss(blP, B, geoP, reacZ, isSHTReac); // Calculate the energy loss of the particle in Foil and SHT
			HEHit = detHits(blP, B, reacPos, geoP.Mask, geoP.Shield, 0);
			// QvalueCalculate(B, E_center_Tgt, mB, mA, ma, 0);
			tlP = TgtELoss(tlP, b, geoP, reacZ, isSHTReac); // Calculate the energy loss of the particle in Foil and SHT
			// LEHit = detHits(tlP, b, reacPos,geoP.Mask,geoP.Shield, 1);
			detHits(tlP, b, reacPos, geoP.Mask, geoP.Shield, 1);
			// QvalueCalculate(b, E_center_Tgt, mb, mA, ma, 1);
		}
		else
		{
			blDecP = TgtELoss(blDecP, decB, geoP, reacZ, isSHTReac); // Calculate the energy loss of the particle in Foil and SHT
			HEHit = detHits(blDecP, decB, reacPos, geoP.Mask, geoP.Shield, 0);
			// QvalueCalculate(B, E_center_Tgt, mB, mA, ma, 0);
			tlP = TgtELoss(tlP, b, geoP, reacZ, isSHTReac); // Calculate the energy loss of the particle in Foil and SHT
			// LEHit = detHits(tlP, b, reacPos,geoP.Mask,geoP.Shield,1);
			detHits(tlP, b, reacPos, geoP.Mask, geoP.Shield, 1);
			// QvalueCalculate(b, E_center_Tgt, mb, mA, ma, 1);
			if (decc.Z > 0)
			{
				tlDecP1 = TgtELoss(tlDecP1, decc, geoP, reacZ, isSHTReac); // Calculate the energy loss of the particle in Foil and SHT
				detHits(tlDecP1, decc, reacPos, geoP.Mask, geoP.Shield, -1);
			}
			if (seqdecN > 2 && decd.Z > 0)
			{
				tlDecP2 = TgtELoss(tlDecP2, decd, geoP, reacZ, isSHTReac); // Calculate the energy loss of the particle in Foil and SHT
				detHits(tlDecP2, decd, reacPos, geoP.Mask, geoP.Shield, -1);
			}
		}
		if (runDepPar.N > 2 && c.Z > 0)
		{
			buP1 = TgtELoss(buP1, c, geoP, reacZ, isSHTReac); // Calculate the energy loss of the particle in Foil and SHT
			detHits(buP1, c, reacPos, geoP.Mask, geoP.Shield, -1);
		}
		if (runDepPar.N > 3 && d.Z > 0)
		{
			buP2 = TgtELoss(buP2, d, geoP, reacZ, isSHTReac); // Calculate the energy loss of the particle in Foil and SHT
			detHits(buP2, d, reacPos, geoP.Mask, geoP.Shield, -1);
		}
		if (runDepPar.N > 4 && e.Z > 0)
		{
			buP3 = TgtELoss(buP3, e, geoP, reacZ, isSHTReac); // Calculate the energy loss of the particle in Foil and SHT
			detHits(buP3, e, reacPos, geoP.Mask, geoP.Shield, -1);
		}
		if (runDepPar.N > 5 && f.Z > 0)
		{
			buP4 = TgtELoss(buP4, f, geoP, reacZ, isSHTReac); // Calculate the energy loss of the particle in Foil and SHT
			detHits(buP4, f, reacPos, geoP.Mask, geoP.Shield, -1);
		}
		// Calculate energy loss in SSB
		Double_t SSBE = eloss(A, 14. / 28., E_before_SSB, 500. * 2.3212 * 0.1, IrisMaterial::Si);
		// phys.SSBdE = rndm->Gaus(SSBE, 0.05 * SSBE);
		Double_t SSBdE = rndm->Gaus(SSBE, 0.05 * SSBE);

		Bool_t sortEnergies = 1; // sort detector hits by energy. Does not work with S3 Detector.
		setIDet(ICdE, SSBdE, sortEnergies);

		if (HEHit && sd1.GetSize() > 0 && sd2.GetSize() > 0.)
		{
			if (sd1.GetdE(0) > 0. && sd2.GetdE(0) > 0.)
				HEHitcntr++;
		}
		tlP.Ecm = (LVb->E() - mb) * ma * 1000. / (mA + ma);
		blP.Ecm = (LVB->E() - mB) * ma * 1000. / (mA + ma);
		LVb->Boost(-boostvect);
		LVB->Boost(-boostvect);
		tlP.Tcm = TMath::RadToDeg() * (TMath::Pi() - LVb->Theta());
		blP.Tcm = TMath::RadToDeg() * LVB->Theta();

		if (seqdec)
		{
			blDecP.Ecm = (LVBdec->E() - mBdec) * ma * 1000. / (mA + ma);
			LVBdec->Boost(-boostvect);
			blDecP.Tcm = TMath::RadToDeg() * LVBdec->Theta();
			tlDecP1.Ecm = (LVcdec->E() - mcdec) * ma * 1000. / (mA + ma);
			LVcdec->Boost(-boostvect);
			tlDecP1.Tcm = TMath::RadToDeg() * (TMath::Pi() - LVcdec->Theta());
			if (seqdecN > 2)
			{
				tlDecP2.Ecm = (LVddec->E() - mddec) * ma * 1000. / (mA + ma);
				LVddec->Boost(-boostvect);
				tlDecP2.Tcm = TMath::RadToDeg() * LVddec->Theta();
			}
		}
		else if (runDepPar.N > 2) // 3body
		{
			buP1.Ecm = (LVcbu->E() - mc) * ma * 1000. / (mA + ma);
			LVcbu->Boost(-boostvect);
			buP1.Tcm = TMath::RadToDeg() * (TMath::Pi() - LVcbu->Theta());
		}
		else if (runDepPar.N > 3) // 4body
		{
			buP2.Ecm = (LVdbu->E() - md) * ma * 1000. / (mA + ma);
			LVdbu->Boost(-boostvect);
			buP2.Tcm = TMath::RadToDeg() * LVdbu->Theta();
		}
		else if (runDepPar.N > 4) // 3body
		{
			buP3.Ecm = (LVcbu->E() - me) * ma * 1000. / (mA + ma);
			LVebu->Boost(-boostvect);
			buP3.Tcm = TMath::RadToDeg() * (TMath::Pi() - LVebu->Theta());
		}
		else if (runDepPar.N > 5) // 3body
		{
			buP4.Ecm = (LVcbu->E() - mf) * ma * 1000. / (mA + ma);
			LVfbu->Boost(-boostvect);
			buP4.Tcm = TMath::RadToDeg() * LVfbu->Theta();
		}

		// printf("Writing %s: %.6d of %.6d events processed. Last event: %d tries.\r", OutputFile.data(), Evnt, nsim, whilecount);
		Evnt++;
		Iris->Fill();
	}

	rndm->Delete();
}

void HandleEOR_SIMULATE()
{
	printf(" in Simulate EOR\n");
	Iris->AutoSave();
	file->Close();
}
