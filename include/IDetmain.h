// IDet.h

#ifndef IDet_H
#define IDet_H
#include <vector>
#include <TObject.h>
#include <TClass.h>

class IDet : public TObject {
 	public:
  		IDet(); //! Create
		virtual ~IDet() {} //! 
		
		Int_t TYdMul;
		std::vector<Int_t> TYdChannel;
		std::vector<Double_t> TYdEnergy;
		std::vector<Int_t> TYdADC;
		std::vector<Int_t> TYdNo;
		std::vector<Int_t> TYdRing;
		std::vector<Int_t> TYdNeighbour;
		std::vector<Double_t> TYdTheta;// Yd theta angle
		std::vector<Double_t> TYdPhi;// Yd Phi angle

		Int_t TCsI1Mul;
		std::vector<Int_t> TCsI1Channel;
		std::vector<Double_t> TCsI1Energy;
		std::vector<Double_t> TCsI2Energy;
		std::vector<Double_t> TCsI1Phi;
		std::vector<Double_t> TCsI1ADC;
		
		std::vector<Double_t> TYdCsI1ETot;
		std::vector<Double_t> TYdCsI2ETot;
		std::vector<Double_t> TYdCsI3ETot;

		Int_t TSSBADC;
		Double_t TSSBEnergy;

		Int_t TScADC;
		Double_t TScEnergy;
	
		std::vector<Int_t> TTrADC;
		std::vector<Double_t> TTrEnergy;
		
		std::vector<Int_t> TICChannel;
		std::vector<Double_t> TICEnergy;
		std::vector<Double_t> TICADC;

		Int_t TSd1rMul;
		std::vector<Int_t> TSd1rChannel;
		std::vector<Double_t> TSd1rEnergy;
		std::vector<Int_t> TSd1rNeighbour;
		std::vector<Int_t> TSd1rADC;
		
		Int_t TSd1sMul;
		std::vector<Int_t> TSd1sChannel;
		std::vector<Double_t> TSd1sEnergy;
		std::vector<Int_t> TSd1sNeighbour;
		std::vector<Int_t> TSd1sADC;
		
		Int_t TSd2rMul;
		std::vector<Int_t> TSd2rChannel;
		std::vector<Double_t> TSd2rEnergy;
		std::vector<Int_t> TSd2rNeighbour;
		std::vector<Int_t> TSd2rADC;
		Double_t TSd2rEnergyCal;
		
		Int_t TSd2sMul;
		std::vector<Int_t> TSd2sChannel;
		std::vector<Double_t> TSd2sEnergy;
		std::vector<Int_t> TSd2sNeighbour;
		std::vector<Int_t> TSd2sADC;
		
		Double_t TSdETot;
		std::vector<Double_t> TSd1Theta;
		std::vector<Double_t> TSd2Theta;
		Double_t TSdThetaCM;
		std::vector<Double_t> TSd1Phi;
		std::vector<Double_t> TSd2Phi;
		
	void Clear();
	ClassDef(IDet,2)
};

#endif
// end
