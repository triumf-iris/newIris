// Graphsdedx.h

#ifndef GraphsH
#define GraphsH
#include <string>

class Graphsdedx
{
public:
	Graphsdedx();
	virtual ~Graphsdedx() {} //!

	std::string Ag;
	std::string Al;
	std::string B;
	std::string Tgt;
	std::string My;
	std::string P;
	std::string Si;
	std::string SiO2;
	std::string Iso;
	std::string Wndw;

	bool boolAg;
	bool boolAl;
	bool boolB;
	bool boolTgt;
	bool boolMy;
	bool boolP;
	bool boolSi;
	bool boolSiO2;
	bool boolIso;
	bool boolWndw;

	void ReadGraphnames(char *line);
	void Load(std::string filename);
	void Print();
	void Clear();
};

#endif
// end
