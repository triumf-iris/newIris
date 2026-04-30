// nucleus.h
#ifndef nucleus_H
#define nucleus_H
#include <string>
#include "dedx.h"

class nucleus {
	public:
		nucleus(); 
		virtual ~nucleus() {}; //! 

		std::string name; 
		int N;
		int Z;
		int A; 
		double mass; 
		double Sp; 
		double Sn; 
		double S2p; 
		double S2n; 
		dedx EL; 

		//virtual void ReadCalibPar(char* line);
		void getInfo(std::string,std::string);
		void getInfo(std::string, int, int);
		void ReadFile(std::string, int, std::string);
		//virtual void SetFilenames(std::string);
		void Print() const;
		void Clear();
};

#endif
// end
