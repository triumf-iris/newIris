// Run dependent parameters in BOR AS
#include "runDepPar.h"
#include <cstring>

runDep::runDep()
{
}

void runDep::setRunDepPar(std::string filename)
{
	ICmin = 0;
	ICmax = 0;
	Q = 0.;
	bool_runPar = false;

	char buffer[256];
	FILE *parFile;
	parFile = fopen(filename.data(), "r");

	if (parFile == NULL)
	{
		perror("Error opening file");
	}
	printf("Reading config file '%s'\n", filename.data());

	while (!feof(parFile))
	{
		if (!fgets(buffer, 256, parFile))
			break;
		printf("%s", buffer);

		char *val = strchr(buffer, '=');
		if (!val)
			printf("Missing = in input parFile, line: '%s'\n", buffer);
		*val = 0;

		// trim param name
		char *trm = val - 1;
		while (*trm <= 32)
			*(trm--) = 0;

		val++;
		if (*val == 0)
			printf("Value missing for parameter %s", buffer);

		// parse float parameter (if any)
		double v;
		int vi;
		char cval[256];
		std::string strval;
		sscanf(val, "%lf", &v);
		sscanf(val, "%d", &vi);
		sscanf(val, "%s", cval);
		strval = cval;

		if (strcmp(buffer, "EBAC") == 0)
		{
			EBAC = v;
			energy = v;
		}
		if (strcmp(buffer, "ICMIN") == 0)
			ICmin = v;
		if (strcmp(buffer, "ICMAX") == 0)
			ICmax = v;
		if (strcmp(buffer, "A") == 0)
			nA = strval;
		if (strcmp(buffer, "a") == 0)
			na = strval;
		if (strcmp(buffer, "B") == 0)
			nB = strval;
		if (strcmp(buffer, "b") == 0)
			nb = strval;
		if (strcmp(buffer, "c") == 0)
			nc = strval;
		if (strcmp(buffer, "d") == 0)
			nd = strval;
		if (strcmp(buffer, "e") == 0)
			ne = strval;
		if (strcmp(buffer, "f") == 0)
			nf = strval;
		if (strcmp(buffer, "foil") == 0)
			nfoil = strval;
		if (strcmp(buffer, "Q") == 0)
			Q = v;
		if (strcmp(buffer, "RUNPAR") == 0)
		{
			runPar = strval;
			bool_runPar = true;
		}
		if (strcmp(buffer, "SHT") == 0)
			SHT = vi;
		if (strcmp(buffer, "N") == 0)
			N = vi;
		if (strcmp(buffer, "R1") == 0)
			R1 = v;
		if (strcmp(buffer, "R2") == 0)
			R2 = v;
		if (strcmp(buffer, "W1") == 0)
			W1 = v;
		if (strcmp(buffer, "W2") == 0)
			W2 = v;
		if (strcmp(buffer, "SHAPE") == 0)
			SHAPE = v;
	}
	fclose(parFile);
}

void runDep::Print()
{
	printf("\n*** Run dependant parameters:\n");
	printf("Reaction %s(%s,%s)%s\n", nA.data(), na.data(), nb.data(), nB.data());
	printf("Energy: %f (%f at center of target)\n", EBAC, energy);
	printf("IC energy gate: min=%f\tmax=%f\n\n", ICmin, ICmax);
	printf("Q value for ground state%lf\n", Q);

	if (bool_runPar == true)
		printf("Run dependant energy and target thickness from %s\n", runPar.data());
	else
		printf("No run dependant energy and target thickness.\n");
}
