// reacParams.h

#ifndef reacParams_H
#define reacParams_H
#include <string>

//Extern
//extern int gMesytecnitems;
class reacParams {
	public:
		reacParams(); 
		virtual ~reacParams() {} //! 

		int N;

		// Reaction A(a,b)B, 
		std::string A; 
		std::string a;
		std::string B;
		std::string b;
		std::string c;
		std::string d;
		std::string e;
		std::string f;
    std::string foil;

		double E; // Beam energy
		double R1; // Resonance energy
		double R2; // Resonance energy
		double W1; // Resonance width
		double W2; // Resonance width
		double SHAPE; // Resonance shape 0=Breit-Wigner, 1=Gaussian, 2=Square
		
		bool SHT; // Reaction in solid Hydrogen/Deuterium target?
		
		//virtual void ReadCalibPar(char* line);
		virtual void ReadParams(char* line);
		virtual void Load(std::string filename);
		virtual void Print() const;
		virtual void Clear();
};

#endif
// end
