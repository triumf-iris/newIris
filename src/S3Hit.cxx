#include <TMath.h>
#include "S3Hit.h"
#include "TRandom3.h"
#include "TVector3.h"
#include "IrisMaterial.h"

ClassImp(S3Hit);

S3Hit::S3Hit()
{
	Orientation = 0; // 0 = rings first, 1 = sectors first
	Thickness = 60.*2.3212*0.1; //um*g/cm^3*0.1
	mul = 0;
	fX.assign(2,NAN);
	fY.assign(2,NAN);
	fZ.assign(2,NAN);
	fPhiCalc.assign(2,NAN);
	fPhiRand.assign(2,NAN);
	fThetaCalc.assign(2,NAN);
	fThetaRand.assign(2,NAN);
	//hit.clear;
	Seg.assign(2,-1);
	Ring.assign(2,-1);
	dE.assign(2,NAN);
	dE_ideal.assign(2,NAN);
}

void S3Hit::Init(Bool_t o, Double_t th)
{
	Orientation = o; // 0 = rings first, 1 = sectors first
	Thickness = th*2.3212*0.1; //um*g/cm^3*0.1
	mul = 0;
	fX.assign(2,NAN);
	fY.assign(2,NAN);
	fZ.assign(2,NAN);
	fPhiCalc.assign(2,NAN);
	fPhiRand.assign(2,NAN);
	fThetaCalc.assign(2,NAN);
	fThetaRand.assign(2,NAN);
	//hit.clear;
	Seg.assign(2,-1);
	Ring.assign(2,-1);
	dE.assign(2,NAN);
	dE_ideal.assign(2,NAN);
}

void S3Hit::Clear(Option_t *option)
{
	mul = 0;
	fX.assign(2,NAN);
	fY.assign(2,NAN);
	fZ.assign(2,NAN);
	fPhiCalc.assign(2,NAN);
	fPhiRand.assign(2,NAN);
	fThetaCalc.assign(2,NAN);
	fThetaRand.assign(2,NAN);
	//hit.clear;
	Seg.assign(2,-1);
	Ring.assign(2,-1);
	dE.assign(2,NAN);
	dE_ideal.assign(2,NAN);
}

Double_t S3Hit::ThetaMin(Double_t S3Distance)
{	
	const Double_t RIn = 11.;	// Inner radius in mm
	Double_t theta_min = TMath::ATan2(RIn,S3Distance)*TMath::RadToDeg();
	return theta_min;
}

Double_t S3Hit::ThetaMax(Double_t S3Distance)
{	
	const Double_t ROut = 35.;	// Outer radius in mm
	Double_t theta_max = TMath::ATan2(ROut,S3Distance)*TMath::RadToDeg();
	return theta_max;
}

Bool_t S3Hit::Hit(Double_t theta, Double_t phi, Double_t S3Distance, TVector3 targetPos, Double_t P)
{
	const Double_t RIn = 11.;	// Inner radius in mm
	const Double_t ROut = 35.;	// Outer radius in mm

	TRandom3 fRandom(0);
	Double_t fX0, fY0, fZ0;
  	Double_t fPhiCalc0, fPhiRand0;
  	Double_t fThetaCalc0, fThetaRand0;
  	Int_t Seg0, Ring0;

	//ring number in S3 and geometric efficiency
	fX0 = S3Distance*tan(theta)*cos(phi);
	fY0 = S3Distance*tan(theta)*sin(phi);
	
	TVector3 partVec(fX0,fY0,S3Distance);
	
	partVec = partVec + targetPos; //taking into account the beam position at the target
	
	fX0 = partVec.X();
	fY0 = partVec.Y();
	fZ0 = partVec.Z();
	theta = partVec.Theta();
	phi = partVec.Phi();

   	// geometric efficiency
	Bool_t hitBool = ((S3Distance*tan(theta)>RIn) && (S3Distance*tan(theta)<ROut));

	if (hitBool){
    	
	  	//hit[mul] = 1;	
		Ring0 = int(S3Distance*tan(theta) - RIn);
		Seg0 = int((TMath::Pi()+phi)*16./TMath::Pi());
 		fPhiCalc0 = -180.+(Seg0*11.25);
 		if (fPhiCalc0<-180.) fPhiCalc0 = fPhiCalc0+360.;
      	Double_t rndm = 0.99*fRandom.Rndm();
 		fPhiRand0 = -180.+(Seg0+rndm)*11.25;
 		if (fPhiRand0<-180.) fPhiRand0 = fPhiRand0+360.;
 		if(Orientation==1){
			Seg0 = 31-Seg0;
			// fPhiCalc[mul] = -fPhiCalc[mul];
			// fPhiRand[mul] = -fPhiRand[mul];
		}
		fThetaCalc0 = TMath::RadToDeg()*atan((RIn+Ring0+0.5)/S3Distance);
 		fThetaCalc0 = (fThetaCalc0>0) ? fThetaCalc0 : fThetaCalc0+180.;
      	rndm = 0.99*fRandom.Rndm();
 		fThetaRand0 = TMath::RadToDeg()*atan((RIn+Ring0+rndm)/S3Distance);
 		fThetaRand0 = (fThetaRand0>0) ? fThetaRand0 : fThetaRand0+180.;
		if(P==0){
		fX[0] 			= fX0;
		fY[0] 			= fY0;
		fZ[0] 			= fZ0;
		fPhiCalc[0]		= fPhiCalc0;
		fPhiRand[0]		= fPhiRand0;
		fThetaCalc[0]	= fThetaCalc0;
		fThetaRand[0]	= fThetaRand0;
		Seg[0]			= Seg0;
		Ring[0]			= Ring0;
		}
		if(P==1){
		fX[1] 			= fX0;
		fY[1] 			= fY0;
		fZ[1] 			= fZ0;
		fPhiCalc[1]		= fPhiCalc0;
		fPhiRand[1]		= fPhiRand0;
		fThetaCalc[1]	= fThetaCalc0;
		fThetaRand[1]	= fThetaRand0;
		Seg[1]			= Seg0;
		Ring[1]			= Ring0;
		}else if (P==-1){
		fX.push_back(fX0);
		fY.push_back(fY0);
		fZ.push_back(fZ0);
		fPhiCalc.push_back(fPhiCalc0);
		fPhiRand.push_back(fPhiRand0);
		fThetaCalc.push_back(fThetaCalc0);
		fThetaRand.push_back(fThetaRand0);
		Seg.push_back(Seg0);
		Ring.push_back(Ring0);}
	}
 	
	return hitBool;
}

Double_t S3Hit::ELoss(nucleus ncl, Double_t E, Double_t Theta, Double_t P)
{
  	Double_t dE0, dE_ideal0;
	Double_t T = (Theta<TMath::Pi()/2.) ? Theta : TMath::Pi()-Theta;
	TRandom3 *rndm = new TRandom3(0);
	
	if(Orientation==0){ // rings first
		E -= elMan->eloss(ncl,13./27.,E,0.1*2.702*1.5/cos(T),IrisMaterial::Al); //first metal
		E -= elMan->eloss(ncl,30./60.,E,0.1*2.65*3.5/cos(T),IrisMaterial::SiO2); //SiO2
		E -= elMan->eloss(ncl,13./27.,E,0.1*2.702*0.3/cos(T),IrisMaterial::Al); //second metal
		E -= elMan->eloss(ncl,5./10.,E,0.1*2.3502*0.5/cos(T),IrisMaterial::B); //boron junction implant 		
		dE0 = elMan->eloss(ncl,14./28.,E,Thickness/cos(T),IrisMaterial::Si);
   		E -= dE0;
		E -= elMan->eloss(ncl,15./31.,E,0.1*1.822*0.5/cos(T),IrisMaterial::P); //phosphorus implant
		E -= elMan->eloss(ncl,13./27.,E,0.1*2.702*0.3/cos(T),IrisMaterial::Al); //metal
		// E -= dE0;
		// if(dE0<0.) dE0 = -dE0;
		// dE_ideal0 = dE0;
		// if(dE0!=0.) dE0 = rndm->Gaus(dE0,0.01*dE0);
		// if(dE0<0.) dE0 = -dE0;
	}
	else{ // sectors first
		E -= elMan->eloss(ncl,13./27.,E,0.1*2.702*0.3/cos(T),IrisMaterial::Al); //metal
		E -= elMan->eloss(ncl,15./31.,E,0.1*1.822*0.5/cos(T),IrisMaterial::P); //phosphorus implant
		dE0 = elMan->eloss(ncl,14./28.,E,Thickness/cos(T),IrisMaterial::Si);	
   		E -= dE0;
		// if(dE0<0.) dE0 = -dE0;
		// dE_ideal0 = dE0;
		// if(dE0!=0.) dE0 = rndm->Gaus(dE0,0.01*dE0);
		// if(dE0<0.) dE0 = -dE0;
	}
	//E -= dE0;
	if(dE0<0.) dE0 = -dE0;
	dE_ideal0 = dE0;
	if(dE0!=0.) dE0 = rndm->Gaus(dE0,0.0225*dE0);
	if(dE0<0.) dE0 = -dE0;
	if(dE0>0){mul++;}
	if(P==0){dE[0] = dE0;
	dE_ideal[0] = dE_ideal0;}
	if(P==1){dE[1] = dE0;
	dE_ideal[1] = dE_ideal0;}
	else if (P==-1){dE.push_back(dE0);
	dE_ideal.push_back(dE_ideal0);}
	rndm->Delete();
	return E;
}

