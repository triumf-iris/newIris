// CalibMesytec.h

#ifndef CalibMesytec_H
#define CalibMesytec_H
#include <string>

// Extern
// extern int gMesytecnitems;
class CalibMesytec
{
public:
	CalibMesytec();
	virtual ~CalibMesytec() {} //!

	std::string installPath;
	std::string fileGeometry;
	std::string fileELoss;
	std::string fileIC;
	std::string fileTr;
	std::string fileCsI1;
	std::string fileCsI2;
	std::string fileSd1r;
	std::string fileSd1s;
	std::string fileSd2r;
	std::string fileSd2s;
	std::string fileSur;
	std::string fileSus;
	std::string fileYd;
	std::string fileYu;
	std::string fileZdx;
	std::string fileZdy;
	std::string fileTCorrIC;
	std::string fileTCorrSi;

	bool boolGeometry;
	bool boolELoss;
	bool boolIC;
	bool boolTr;
	bool boolCsI1;
	bool boolCsI2;
	bool boolSd1r;
	bool boolSd1s;
	bool boolSd2r;
	bool boolSd2s;
	bool boolSur;
	bool boolSus;
	bool boolYd;
	bool boolYu;
	bool boolZdx;
	bool boolZdy;
	bool boolTCorrIC;
	bool boolTCorrSi;

	virtual void ReadFilenames(char *line);
	virtual void Load(std::string filename);
	virtual void Print();
	virtual void Clear();
	//		ClassDef(CalibMesytec,1)
};

#endif
// end
