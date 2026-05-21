// CalibPHYSICS.h

#ifndef CalibPHYSICS_H
#define CalibPHYSICS_H
#include <string>

// Extern
// extern int gPHYSICSnitems;
class CalibPHYSICS
{
public:
	CalibPHYSICS();
	virtual ~CalibPHYSICS() {} //!

	std::string installPath;
	std::string fileRunList;
	std::string fileGate;
	std::string nameGate;
	std::string fileSdGate;
	std::string nameSdGate;
	std::string fileYuGate;
	std::string nameYuGate;
	std::string fileSuGate;
	std::string nameSuGate;
	std::string fileELoss;
	std::string fileIdedx;
	std::string fileLdedx;
	std::string fileHdedx;
	std::string filededx;
	std::string fileRunDepPar;
	std::string fileGeometry;
	std::string fileTCorrIC;
	int numGate;

	bool boolRunList;
	bool boolICGates;
	bool boolIC;
	bool boolFGate;
	bool boolNGate;
	bool boolNumGate;
	bool boolFSdGate;
	bool boolNSdGate;
	bool boolFYuGate;
	bool boolNYuGate;
	bool boolFSuGate;
	bool boolNSuGate;
	bool boolELoss;
	bool boolIdedx;
	bool boolLdedx;
	bool boolHdedx;
	bool booldedx;
	bool boolRunDepPar;
	bool boolGeometry;
	bool boolEssential;
	bool boolTCorrIC;

	// virtual void ReadCalibPar(char* line);
	void ReadFilenames(char *line);
	void Load(std::string filename);
	void Print();
	void Clear();
};

#endif
// end
