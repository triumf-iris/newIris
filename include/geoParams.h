// geoParams.h

#ifndef geoParams_H
#define geoParams_H
#include <string>

//Extern
//extern int gMesytecnitems;
class geoParams {
	public:
		geoParams(); 
		virtual ~geoParams() {} //! 
		
		double Bs; 
		double ICPressure; 
		double TFoil; 
		double AoZFoil; 
		double TTgt; 
		double AoZTgt; 
		double DYY; 
		double TYY[8]; 
		double DYYU; 
		double TYYU[8]; 
		double DS3; 
		double TS3[2];
		double DS3U; 
		double TS3U;
		std::string MFoil;
		std::string MTgt;	
		bool Mask;
		bool Shield;
		bool Orientation;

		//virtual void ReadCalibPar(char* line);
		virtual void ReadParams(char* line);
		virtual void Load(std::string filename);
		virtual void Print() const;
		virtual void Clear();
};

#endif
// end
