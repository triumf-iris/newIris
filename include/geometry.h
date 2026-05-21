// experiment geometry
#ifndef geometry_H
#define geometry_H
#include <string>

class geometry
{
public:
	geometry();
	virtual ~geometry() {} //!

	double TargetThickness;
	int TargetOrientation;
	double FoilThickness;
	double xShift;
	double yShift;
	double YdThickness[8];
	double YdDistance;
	double YuThickness[8];
	double YuDistance;
	double YdInnerRadius;
	double YdOuterRadius;
	double Sd1Thickness;
	double Sd1Distance;
	double Sd2Thickness;
	double Sd2Distance;
	double SuThickness;
	double SuDistance;
	double SdInnerRadius;
	double SdOuterRadius;
	std::string MFoil;
	std::string MTgt;	

	void ReadGeometry(std::string);
	void Print();
};
#endif
// end
