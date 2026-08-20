// header_6Li.h if the header file name is changed you need to modify it in H2Monitor.C,plottarget.C and SSBThickness.C as well 

#ifndef CONFIG_H
#define CONFIG_H

#include "runlist_12C.h"
//#include <ROOT/TThreadExecutor.hxx>

// Isotope variable
std::string isotope = "12C"; //Isotope selected in IC to find target thickness
std::string TgtName = "2H"; // Target name for getting correct energyloss table in H2Monitor.C
std::string Xnumber = "S1701"; //Experiment number needed to navigate to folders 
std::string treeDir = "/home/iris/curtis/NewIris/newIRIS/workMacros/"; //file path for treeIris files

std::string CalibFile = "/home/iris/curtis/NewIris/newIRIS/Configurationfiles/phys_config_12C_2H_CATIMA.txt";

Int_t ICBin = 100;
Int_t ICMin = 1000;
Int_t ICMax = 2500;
Int_t Sd1ring = 1;
Int_t Sd1Bin = 100;
Int_t Sd1Min = 20;
Int_t Sd1Max = 150;
Int_t tgt_stopin_Sd1 = 1; //0 if particle does not stop in Sd1
Int_t Sd2ring = 1;
Int_t Sd2Bin = 20;
Int_t Sd2Min = 150;
Int_t Sd2Max = 180;
//Double_t Theta = 1.61;

//Double_t isotopemass = 40961.825 ; //mass of incoming beam
Double_t TrgtD = 0; // target density; 0.201 for D2, 0.0868 for H2; Derived from "a" in runDepPar 
Int_t upstream=0; //if target upstream silver foil, use 1.
Double_t TBE0=215.44; //Beam energy in S3 without target
//For SSB plot

Double_t SSBBin = 500.;  // SSB expected binsize
Double_t SSBMin = 1000.;
Double_t SSBMax = 3000.;  // SSB max and minimum range
//Double_t E= 471.5; //Beam energy before IC. 




Int_t nRuns = sizeof(runs) / sizeof(runs[0]);

std::string outputFilename = "/home/iris/curtis/NewIris/newIRIS/workMacros/" + Xnumber + "/H2Monitor/results12Cd.txt"; // output file will have measured values 
std::string targetoutput = "/home/iris/curtis/NewIris/newIRIS/workMacros/" + Xnumber + "/H2Monitor/target12Cd.txt"; // output file will have measured values 


std::string rootOutputFile = "/home/iris/curtis/NewIris/newIRIS/workMacros/" + Xnumber + "/H2Monitor/results12Cd.root"; // Will have output histograms 

std::string processGate1 = ".x /home/iris/curtis/NewIris/newIRIS/workMacros/" + Xnumber + "/gates/12C_protons.C"; // To run the script that contains particle ID gate as TCutG
std::string processGate2 = ".x /home/iris/curtis/NewIris/newIRIS/workMacros/" + Xnumber + "/gates/12C_deuterons.C"; // To run the script that contains particle ID gate as TCutG
std::string processGate3 = "";// To run the script that contains particle ID gate as TCutG
std::string processGate4 = ""; // To run the script that contains particle ID gate as TCutG

#endif // CONFIG_H
