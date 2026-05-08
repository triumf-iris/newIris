 // Author: Alisher Sanetullaev  TRIUMF, 2012/10/01                                          
// Rewritten: Matthias Holl, 2017/09/01

#ifndef __TEVENT_H
#define __TEVENT_H

#include <TClass.h>
#include <TObject.h>

class TEvent : public TObject {
	public:
		TEvent(); //! Create
		TEvent(const TEvent &); //!
		virtual ~TEvent() {} //!
		
		Double_t fEBAC;
		Double_t fmA;
		Double_t fma;
		Double_t fmB;
		Double_t fmb;
		Double_t fkBF;
		Double_t fEBeam;
		Double_t fbetaCM;
		Double_t fgammaCM;
		Double_t fPA;		    
		Double_t fLP;	// Light particle energy
		Double_t fHP;	// Heavy particle energy
		Double_t fEB;	// Measured heavy particle energy

		std::vector<Double_t> Qdet1_tlP;
		std::vector<Double_t> Ex1_tlP;
		std::vector<Double_t> YdCsI1ETot_tlP;
		std::vector<Double_t> EB1_det_tlP;
		std::vector<Double_t> PB1_det_tlP;
		std::vector<Double_t> fEYd1_tlP;
		std::vector<Double_t> fCCsI1_tlP;
		std::vector<Double_t> fECsI1_tlP;
		std::vector<Double_t> fEb1_tlP;
		std::vector<Double_t> fPb1_tlP;
  		std::vector<Double_t> fPby1_tlP;
  		std::vector<Double_t> fPbxcm1_tlP;
		std::vector<Double_t> fThetacm1_tlP;
		std::vector<Double_t> fTheta1_tlP;

		std::vector<Double_t> Qdet2_tlP;
		std::vector<Double_t> Ex2_tlP;
		std::vector<Double_t> YdCsI2ETot_tlP;
		std::vector<Double_t> EB2_det_tlP;
		std::vector<Double_t> PB2_det_tlP; //Calculated heavy particle momentum
		std::vector<Double_t> fEYd2_tlP;
		std::vector<Double_t> fCCsI2_tlP;
		std::vector<Double_t> fECsI2_tlP;
		std::vector<Double_t> fEb2_tlP;
		std::vector<Double_t> fPb2_tlP;
  		std::vector<Double_t> fPby2_tlP;
  		std::vector<Double_t> fPbxcm2_tlP;
		std::vector<Double_t> fThetacm2_tlP;
		std::vector<Double_t> fTheta2_tlP;

		std::vector<Double_t> fPBeam_tlP;
  		std::vector<Double_t> fPResid_tlP;
  		std::vector<Double_t> fA_tlP;
  		std::vector<Double_t> fB_tlP;
  		std::vector<Double_t> fC_tlP;
		std::vector<Double_t> fEB_tlP;
		std::vector<Double_t>TSdThetaCM_tlP;
		std::vector<Double_t>TSdETot_tlP;

		std::vector<Double_t> Qdet1_blP;
		std::vector<Double_t> Ex1_blP;
		std::vector<Double_t> YdCsI1ETot_blP;
		std::vector<Double_t> EB1_det_blP; //Calculated heavy particle energy
		std::vector<Double_t> PB1_det_blP; //Calculated heavy particle momentum
		std::vector<Double_t> fEYd1_blP;
		std::vector<Double_t> fCCsI1_blP; 
		std::vector<Double_t> fECsI1_blP; 
		std::vector<Double_t> fEb1_blP;
  		std::vector<Double_t> fPb1_blP;
  		std::vector<Double_t> fPby1_blP;
  		std::vector<Double_t> fPbxcm1_blP;
		std::vector<Double_t> fThetacm1_blP;
		std::vector<Double_t> fTheta1_blP;
 
		std::vector<Double_t> Qdet2_blP;
		std::vector<Double_t> Ex2_blP;
		std::vector<Double_t> YdCsI2ETot_blP;
		std::vector<Double_t> EB2_det_blP; //Calculated heavy particle energy
		std::vector<Double_t> PB2_det_blP; //Calculated heavy particle momentum
		std::vector<Double_t> fEYd2_blP;
		std::vector<Double_t> fCCsI2_blP; 
		std::vector<Double_t> fECsI2_blP; 
		std::vector<Double_t> fEb2_blP;
  		std::vector<Double_t> fPb2_blP;
  		std::vector<Double_t> fPby2_blP;
  		std::vector<Double_t> fPbxcm2_blP;
		std::vector<Double_t> fThetacm2_blP;
		std::vector<Double_t> fTheta2_blP;
 
		std::vector<Double_t> fPBeam_blP;
  		std::vector<Double_t> fPResid_blP;
  		std::vector<Double_t> fA_blP;
  		std::vector<Double_t> fB_blP;
  		std::vector<Double_t> fC_blP;
		std::vector<Double_t> fEB_blP;
		std::vector<Double_t> TSdThetaCM_blP;
		std::vector<Double_t> TSdETot_blP;
				
		Double_t fEbU;
  		Double_t fPbU;
  		Double_t fPbUy;
  		Double_t fPbUxcm;
		Double_t fEBU; //Calculated heavy particle energy
		Double_t fPBU; //Calculated heavy particle momentum
		Double_t fQvU;
		Double_t fExU;
		Double_t fThetaDU;
		Double_t fThetacmU;
		 
		Double_t fEbUSd;
  		Double_t fPbUSd;
  		Double_t fPbUSdy;
  		Double_t fPbUSdxcm;
		Double_t fEBUSd; //Calculated heavy particle energy
		Double_t fPBUSd; //Calculated heavy particle momentum
		Double_t fQvUSd;
		Double_t fThetacmUSd;
		Double_t fThetaDUSd;
		
		void Clear();	//! Clear values
		
	private:
		Int_t fRun; // Current run number
		ClassDef(TEvent,4) // Version
};
#endif
