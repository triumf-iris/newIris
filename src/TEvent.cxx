// Author: Alisher Sanetullaev  TRIUMF, 2012/10/01                                           
// Rewritten: Matthias Holl, 2017/09/01
#include <math.h>
#include "TEvent.h"

TEvent::TEvent() {
	//TEvent::Class()->IgnoreTObjectStreamer();
	TEvent::Clear();
	
}

TEvent::TEvent(const TEvent &evt) : TObject(evt) //
{
	// -- Copy constructor.                                                                
	((TEvent&)evt).Copy(*this);
}

void TEvent::Clear(){

	fEBAC = sqrt(-1.);
	fmA = sqrt(-1.);
	fma = sqrt(-1.);
	fmB = sqrt(-1.);
	fmb = sqrt(-1.);
	fkBF = sqrt(-1.);
	fEBeam = sqrt(-1.);
	fbetaCM = sqrt(-1.);
	fgammaCM = sqrt(-1.);
	fPA = sqrt(-1.);
	fLP=0; //Light particle energy
	fHP=0; //Heavy particle energy
	fEB=0; //Measured heavy particle energy
	
	Qdet1_tlP.clear();
	Ex1_tlP.clear();
	YdCsI1ETot_tlP.clear();
	EB1_det_tlP.clear();
	PB1_det_tlP.clear(); //Calculated heavy particle momentum
	fEYd1_tlP.clear();
	fCCsI1_tlP.clear();
	fECsI1_tlP.clear();
	fEb1_tlP.clear();
	fPb1_tlP.clear();
  	fPby1_tlP.clear();
  	fPbxcm1_tlP.clear();
	fThetacm1_tlP.clear();
	fTheta1_tlP.clear();

	Qdet2_tlP.clear();
	Ex2_tlP.clear();
	YdCsI2ETot_tlP.clear();
	EB2_det_tlP.clear();
	PB2_det_tlP.clear(); //Calculated heavy particle momentum
	fEYd2_tlP.clear();
	fCCsI2_tlP.clear();
	fECsI2_tlP.clear();
	fEb2_tlP.clear();
	fPb2_tlP.clear();
  	fPby2_tlP.clear();
  	fPbxcm2_tlP.clear();
	fThetacm2_tlP.clear();
	fTheta2_tlP.clear();

	fPBeam_tlP.clear();
  	fPResid_tlP.clear();
  	fA_tlP.clear();
  	fB_tlP.clear();
  	fC_tlP.clear();
	fEB_tlP.clear();
	TSdThetaCM_tlP.clear();
	TSdETot_tlP.clear();

	Qdet1_blP.clear();
	Ex1_blP.clear();
	YdCsI1ETot_blP.clear();
	EB1_det_blP.clear(); //Calculated heavy particle energy
	PB1_det_blP.clear(); //Calculated heavy particle momentum
	fEYd1_blP.clear();
	fCCsI1_blP.clear(); 
	fECsI1_blP.clear(); 
	fEb1_blP.clear();
  	fPb1_blP.clear();
  	fPby1_blP.clear();
  	fPbxcm1_blP.clear();
	fThetacm1_blP.clear();
	fTheta1_blP.clear();
	
	Qdet2_blP.clear();
	Ex2_blP.clear();
	YdCsI2ETot_blP.clear();
	EB2_det_blP.clear(); //Calculated heavy particle energy
	PB2_det_blP.clear(); //Calculated heavy particle momentum
	fEYd2_blP.clear();
	fCCsI2_blP.clear(); 
	fECsI2_blP.clear(); 
	fEb2_blP.clear();
  	fPb2_blP.clear();
  	fPby2_blP.clear();
  	fPbxcm2_blP.clear();
	fThetacm2_blP.clear();
	fTheta2_blP.clear();

	fPBeam_blP.clear();
  	fPResid_blP.clear();
  	fA_blP.clear();
  	fB_blP.clear();
  	fC_blP.clear();
	fEB_blP.clear();
	TSdThetaCM_blP.clear();
	TSdETot_blP.clear();

	//Upstream

	fEbU = sqrt(-1.);
  	fPbU = sqrt(-1.);
  	fPbUy = sqrt(-1.);
  	fPbUxcm = sqrt(-1.);
	fEBU=0; //Calculated heavy particle energy
	fPBU=0; //Calculated heavy particle momentum
	fQvU=0;
	fExU = 0 ;
	fThetaDU=0;
	fThetacmU=0;
	
	fEbUSd = sqrt(-1.);
  	fPbUSd = sqrt(-1.);
  	fPbUSdy = sqrt(-1.);
  	fPbUSdxcm = sqrt(-1.);
	fEBUSd=0; //Calculated heavy particle energy
	fPBUSd=0; //Calculated heavy particle momentum
	fQvUSd=0;
	fThetacmUSd=0;
	fThetaDUSd=0;


	
	
}

