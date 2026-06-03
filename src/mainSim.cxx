// simIris
// Simulation software for the IRIS experiment @TRIUMF

#include <stdlib.h>
#include <fstream>
#include <array>
#include <stdio.h>
#include <sys/time.h>
#include <iostream>
#include <assert.h>
#include <signal.h>
#include <string>

#include "TStopwatch.h"

#include "HandleSIMULATE.h"

// Global Variables
std::string gOutputFile = "";
std::string gCalibFile = "";
std::string gDWBAFile = "";
int gNsim = 1E3;

TFile *file = NULL;
TTree *Iris = NULL;

void help()
{
	printf("\nUsage:\n");
	printf("\n./bin/simIris [-h] [-c][-o][-e][-d]\n");
	printf("\n");
	printf("\t-h: print this help message\n");
	printf("\t-o/-o=/--output=: Path of output file.\n");
	printf("\t-c/-c=/--config=: Path of main configuration file.\n");
	printf("\t-d/-d=/--dwba=: Path of DWBA cross section file\n");
	printf("\t-e/-e=/--events=: Number of events to simulate\n");
	printf("\n");
	exit(1);
}

int Process(std::string dirname)
{
	printf("---- BEGIN PROCESSING ---- \n");
	if (dirname.empty())
	{
		printf("Can't determine path of executable");
		return -1;
	}
	else
	{
		TStopwatch timer;
		timer.Start();
		printf("Running %s.\n", dirname.data());
		printf("Writing data to %s.\n", gOutputFile.data());
		std::string IRISdir = dirname;
		IRISdir.erase(IRISdir.end() - 11, IRISdir.end());
		HandleBOR_SIMULATE(IRISdir, gCalibFile, gOutputFile, 0);
		printf("Running %s.\n", dirname.data());
		HandleSIMULATE(gNsim, gDWBAFile);
		HandleEOR_SIMULATE();
		printf("\n\n********************\n");
		Double_t time = timer.RealTime();
		printf("\nDone. %lf s\n", time);
		printf("\nOutput written to %s \n", gOutputFile.data());
		printf("\n\n********************\n");
		return 0;
	}
}

int main(int argc, char *argv[])
{
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);

	signal(SIGILL, SIG_DFL);
	signal(SIGBUS, SIG_DFL);
	signal(SIGSEGV, SIG_DFL);

	char *endptr;

	std::string binpath(argv[0]);
	printf("%s\n", binpath.data());

	std::vector<std::string> args;
	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "-h") == 0)
			help(); // does not return
		args.push_back(argv[i]);
	}

	bool have_output = false;
	// bool have_calib= false;

	for (unsigned int i = 1; i < args.size(); i++) // loop over the commandline options
	{
		const char *arg = args[i].c_str();

		if (strncmp(arg, "-o=", 3) == 0)
		{ // Output file
			gOutputFile = arg + 3;
			have_output = true;
		}
		else if (strncmp(arg, "-o", 2) == 0)
		{ // Output file
			gOutputFile = arg + 2;
			have_output = true;
		}
		else if (strncmp(arg, "--output=", 9) == 0)
		{ // Output file
			gOutputFile = arg + 9;
			have_output = true;
		}
		else if (strncmp(arg, "-c=", 3) == 0)
		{ // Calibration file
			gCalibFile = arg + 3;
		}
		else if (strncmp(arg, "-c", 2) == 0)
		{ // Calibration file
			gCalibFile = arg + 2;
		}
		else if (strncmp(arg, "--config=", 9) == 0)
		{ // Calibration file
			gCalibFile = arg + 9;
		}
		else if (strncmp(arg, "-d=", 3) == 0)
		{
			gDWBAFile = arg + 3;
		}
		else if (strncmp(arg, "-d", 2) == 0)
		{ // Calibration file
			gDWBAFile = arg + 2;
		}
		else if (strncmp(arg, "--dwba=", 7) == 0)
		{
			gDWBAFile = arg + 7;
		}
		else if (strncmp(arg, "-e=", 3) == 0)
		{
			gNsim = strtol(arg + 3, &endptr, 10); // converting string to number
		}
		else if (strncmp(arg, "-e", 2) == 0)
		{
			gNsim = strtol(arg + 2, &endptr, 10); // converting string to number
		}
		else if (strncmp(arg, "--events=", 9) == 0)
		{
			gNsim = strtol(arg + 9, &endptr, 10); // converting string to number
		}
		else if (strcmp(arg, "-h") == 0)
			help(); // does not return
		else if (arg[0] == '-')
			help(); // does not return
	}

	if (have_output == true)
	{
		Process(binpath);
	}
	else
	{
		printf("No output file specified!\n");
		printf("Use -o=/path/to/your/output-file!\n");
	}
	return 0;
}
