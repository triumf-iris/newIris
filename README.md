# newIris
Suite of analysis and simulation tools for the IRIS experiment @ TRIUMF

physIris for analysis.
simIris for simulation.


## Download and Installation ##
	
newIris requires ROOT (https://root.cern.ch).

Make required changes into Makefile for MacOS 

To install newIris use

	git clone https://github.com/triumf-iris/newIris
	cd newIris
	make

### Installing with CATIMA ###

simIris supports using [catima](https://github.com/hrosiak/catima) for generating energy loss table automatically based on the geometry and reaction input files. If you want to use this functionality, install catima an your system then compile using the following flags

	make USE_CATIMA=1 CATIMAPATH=[path to catima]

### Installing with treeIris ###

treeIris has extra dependencies required to build which exist on iris0,01,02. Since many personal builds do not require treeIRIS, to avoid needing the dependencies, treeIRIS is an optional build. If needing treeIRIS, compile using the following flags

	make USE_TREEIRIS=1

## Testing simulation and analysis ##
The testMacros/testSimulation folder has an example runSim.sh and runPhys.sh for simulating and analyzing a 12C + D experiment. Configuration files for this test experiment are in the Configurationfiles folder.

Run runSim.sh first to generate the simulated data file using simIris.

	./runSim.sh #of-events

Run runPhys.sh second to analyze the simulated data file using physIris, generating a new ROOT file with Q-values and other physics information.

	./runPhys.sh

## Running physIris ##

From the main newIris folder, you can run physIris with
 
	./bin/physIris /path/to/your/input-folder -c=/path/to/your/config.file -o=/path/to/to/your/output.file 

A configuration file must be provided. See Config Files section below for more information.

## Running simIris ##

From the main newIris folder, you can run simIris with
 
	./bin/simIris -c=/path/to/your/config.file -o=/path/to/to/your/output.file -e=number of events

A configuration file must be provided. See Config Files section below for more information.

## Running treeIris ##

From the main newIris folder, you can run treeIris with
 
	./bin/treeIris /path/to/your/input-file -c=/path/to/your/config.file -o=/path/to/to/your/output.file 

A configuration file must be provided. See Config Files section below for more information.

## The Configuration Files ##

The newIris codes, simIris, physIris, and treeIris, all use a common set of configuration files.

A master configuration file, a geometry file, a run dependent parameters file, and a list of calibration files. Each is detailed below.

### The Master Configuration File ###

The Master Configuration File contains the locations of the geometry and run dependent parameter files. Optionally it can also contain the location of dEdx files, if desired. If dEdx files are not included, CATIMA will be used. An example can be found at
	
	Configurationfiles/phys_config_12C_2H_LISE.txt

### The Geometry File ###

The geometry file contains a set of parameters describing the experimental setup of IRIS. See the example file for more information

	Configurationfiles/geometry.txt

### The Run Dependent Parameter File ###

The reaction parameter file contains information about the reaction for the given runs. See the example for more information

	Configurationfiles/runDepPar_12C_2H.txt

### Calibration File List###

The calibration file list contains the location for the calibration files for the detectors.

	Configurationfiles/calibration.txt

## Energy loss tables##

simIris and physIris both require energy loss tables. Two options exist, files from LISE++ or letting the code use CATIMA to generate the tables. 

### LISE++ ###

simIris and physIris use SRIM  energy loss tables to calculate the energy loss of particles going through detector materials or the target. Tables in a usable format can be created using LISE++.

Include the location to these files in the master configuration file as

	DEDX_X=/path/to/LiseFiles

### CATIMA ###

To run a simulation using catima, omit the DEDX_X line from the master configuration file and it will default to generating tables from catima.

NOTE: must be compiled with catima

## Output file for physIris ##

The resulting output is a root file containing the TTree Iris. In addition to the branches already present in the input files, it contains the following branches:

	fEYY1: dead-layer corrected YY1 energy
	fECsI1: dead-layer corrected CsI1 energy
	fECsI2: dead-layer corrected CsI2 energy
	
	fThetacm1/2/U: center-of-mass angle from CsI1/CsI2/upstream detectors
	fThetaD/DU: downstream/upstream angles
	
	fEBAC: beam energy from accelerator
	fmA/a/B/b: mass of particle A,a,B,b
	fEBeam: beam energy at center of target
	fbetaCM: beam beta
	fgammaCM: beam gamma
	fPA: beam momentum at center of target
	
	fEb1/2/U/USd: Reconstructed light particle energy using CsI1/CsI2/upstream YY1/upstream S3 
	fPb1/2/U/USd: Reconstructed light particle momentum using CsI1/CsI2/upstream YY1/upstream S3 
	fPb1/2/U/USdy: Reconstructed light particle momentum y-component using CsI1/CsI2/upstream YY1/upstream S3
	fPb1/2/U/USdxcm: Reconstructed light particle momentum x-component in center-of-mass using CsI1/CsI2/upstream YY1/upstream S3
 
	fLP:	Light particle energy
	fHP:	Heavy particle energy
	fEB:	Measured heavy particle energy
	
	fEB1/2/U/USd:	Calculated heavy particle energy using CsI1/CsI2/upstream YY1/upstream S3
	fPB1/2/U/USd:	Calculated heavy particle momentum using CsI1/CsI2/upstream YY1/upstream S3
	fQv1/2/U/USd:	Q-value using CsI1/CsI2/upstream YY1/upstream S3
	
Apart from that, the branches TSdETot and TYdCsI1/2ETot, i.e. the dead-layer corrected total energies are filled by physIris. 

A few other variables are maybe left in for debugging of the Q-value calculation:

	fkBF: Ratio of Beam particle mass and 109-Ag foil nucleus mass
	fA/B/C: quadratic equation parameters
		A = kBF-1.;
		B = 2.0*PResid* cos(TMath::DegToRad()*det->TSd1Theta.at(0));
		C = -1.*(kBF+1)*PResid*PResid; 
	fPResid: Momentum of residue
		PResid = sqrt(2.*det->TSdETot*mA);
	fPBeam: Calculated beam momentum after scattering off Ag
		if (A!=0)    PBeam = (sqrt(B*B-4.*A*C)-B)/(2*A);



## Output file for simIris ##

The output root file of simIris contains a tree with the following objects:

	yd, yu, sd1, sd2, su, csi: Containers for detector hits. They contain the detector thickness, and information about the simulated detector hit: multiplicity, X-, Y-, Z-position, angls, git segments, and depostited energy.

	tlP, blP: Contain information about the generated particle tracks. Contains angles T(heta) and P(hi) and energies in the lab and center-of-mass system. Also saves the energy loss in the silver foil and target as well as the resulting energy behind the target (FoildE, TrgtdE, Ebt).

	tlPdec1/2, blPdec1/2: Same as blP and tlP, but only used when the heavy particle at the simulated excitation energy is unbound.

	det: Contains the same information as yd, yu, etc. but in the same format as data sorted with treeIris. Simulated data from simIris can therefore analysed using physIris (https://github.com/iris-triumf/physIris) the same way as real experimental data.


Have fun!!!
