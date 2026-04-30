#include "CsIHit.h"
#include "IrisMaterial.h"

ClassImp(CsIHit);

CsIHit::CsIHit()
{
	Thickness =12000.*4.51*0.1; //um*g/cm^3*0.1;
	mul = 0;
	fX.assign(2,NAN);
	fY.assign(2,NAN);
	fZ.assign(2,NAN);
	fPhiCalc.assign(2,NAN);
	fPhiRand.assign(2,NAN);
	//hit.assign(2,NAN);
	Seg.assign(2,-1);
	dE.assign(2,NAN);
	dE_ideal.assign(2,NAN);
}

CsIHit::CsIHit(Double_t th)
{
	Thickness = th*4.51*0.1;
	mul = 0;
	fX.assign(2,NAN);
	fY.assign(2,NAN);
	fZ.assign(2,NAN);
	fPhiCalc.assign(2,NAN);
	fPhiRand.assign(2,NAN);
	//hit.assign(2,NAN);
	Seg.assign(2,-1);
	dE.assign(2,NAN);
	dE_ideal.assign(2,NAN);
}

void CsIHit::Clear(Option_t *option)
{
	mul = 0;
	fX.assign(2,NAN);
	fY.assign(2,NAN);
	fZ.assign(2,NAN);
	fPhiCalc.assign(2,NAN);
	fPhiRand.assign(2,NAN);
	//hit.assign(2,NAN);
	Seg.assign(2,-1);
	dE.assign(2,NAN);
	dE_ideal.assign(2,NAN);
}

Double_t CsIHit::ThetaMin(Double_t CsIDistance)
{	
	const Double_t RIn = 50.;	// Inner radius in mm
	Double_t theta_min = TMath::ATan2(RIn,CsIDistance)*TMath::RadToDeg();
	return theta_min;
}

Double_t CsIHit::ThetaMax(Double_t CsIDistance)
{	
	const Double_t ROut = 150.;	// Outer radius in mm
	Double_t theta_max = TMath::ATan2(ROut,CsIDistance)*TMath::RadToDeg();
	return theta_max;
}

Bool_t CsIHit::Hit(Double_t theta, Double_t phi, Double_t CsIDistance, TVector3 targetPos, Double_t P)
{
	const Double_t RIn = 50.;	// Inner radius in mm
	const Double_t ROut = 150.;	// Outer radius in mm

	TRandom3 fRandom(0);
	Double_t fX0, fY0, fZ0;
	Double_t fPhiCalc0, fPhiRand0;
	Int_t Seg0;
	
	Bool_t hitTheta = 0;
	Bool_t hitPhi = 0;
	Bool_t hitBool = 0; // return value
	Double_t phiGap = 0.1*TMath::DegToRad(); // phi gap between CsI1s in rad 
	Double_t phiShift = -103.*TMath::DegToRad(); // shift from the vertical direction for the first YY1
	Double_t phiRel; // Relative phi after phishift
	Double_t phiRange = TMath::Pi()/8-phiGap; // phi range for each CsI 

	phiRel = phi+phiShift;
	if (phiRel>TMath::Pi())  phiRel = phiRel - 2*TMath::Pi();
	if (phiRel<-TMath::Pi())  phiRel = phiRel + 2*TMath::Pi();
	
	// geometric efficiency
	fX0 = CsIDistance*tan(theta)*cos(phi);
	fY0 = CsIDistance*tan(theta)*sin(phi);
	
	TVector3 partVec(fX0,fY0,CsIDistance);
	
	partVec = partVec + targetPos; // taking into account the beam position at the target
	
	fX0 = partVec.X();
	fY0 = partVec.Y();
	fZ0 = partVec.Z();
	theta = partVec.Theta();
	phi = partVec.Phi();
	
	hitTheta = ((CsIDistance*tan(theta)>RIn) && (CsIDistance*tan(theta)<ROut));
	Seg0 = int((phiRel+TMath::Pi())/(TMath::Pi()/8));
	hitPhi = fabs(phiRel +TMath::Pi() - Seg0*TMath::Pi()/8.-TMath::Pi()/16.) < phiRange/2.;
  	hitBool = (hitPhi && hitTheta); 
	
	if (hitBool){
	  	fX0 = fX0;
		fY0 = fY0;	
 		Seg0 = 7-Seg0;
		if(Seg0<0) Seg0 = Seg0 + 16;
 		fPhiCalc0 = -phiShift*TMath::RadToDeg()-((Seg0+0.5)*22.5);
 		if (fPhiCalc0<-180.)	fPhiCalc0 = fPhiCalc0+360.;
 		if (fPhiCalc0>180.)	fPhiCalc0 = fPhiCalc0-360.;

      	Double_t random = 0.99*fRandom.Rndm();
 		fPhiRand0 = -phiShift*TMath::RadToDeg()-((Seg0+random)*22.5);
 		if (fPhiRand0<-180.) fPhiRand0 = fPhiRand0+360.;
 		if (fPhiRand0>180.) fPhiRand0 = fPhiRand0-360.;
    	if(P==0){
		fX[0] = fX0;
		fY[0] = fY0;
		fZ[0] = fZ0;
		fPhiCalc[0] = fPhiCalc0;
		fPhiRand[0] = fPhiRand0;
		Seg[0] = Seg0;
		}if(P==1){
		fX[1] = fX0;
		fY[1] = fY0;
		fZ[1] = fZ0;
		fPhiCalc[1] = fPhiCalc0;
		fPhiRand[1] = fPhiRand0;
		Seg[1] = Seg0;
		}else if(P==-1){
		fX.push_back(fX0);
		fY.push_back(fY0);
		fZ.push_back(fZ0);
		fPhiCalc.push_back(fPhiCalc0);
		fPhiRand.push_back(fPhiRand0);
		Seg.push_back(Seg0);
		}
	}
  		
	return hitBool;
}

Double_t CsIHit::ELoss(nucleus ncl, Double_t E, Double_t T, Double_t P)
{
  	Double_t dE0, dE_ideal0;
	//if(mul>0 && hit0==1){
		TRandom3 *rndm = new TRandom3(0);
		E -= elMan->eloss(ncl,15./31.,E,0.1*1.8219*0.1/cos(T),IrisMaterial::P);
		E -= elMan->eloss(ncl,13./27.,E,0.3*2.702*0.1/cos(T),IrisMaterial::Al);
		E -= elMan->eloss(ncl,100./192.,E,6.*1.4*0.1/cos(T),IrisMaterial::Mylar);
		dE0 = elMan->eloss(ncl,108./260.,E,Thickness/cos(T),IrisMaterial::CsI);
		E -= dE0;
		if(dE0<0.) dE0 = -dE0;
		dE_ideal0 = dE0;
		if(dE0!=0.) dE0 = rndm->Gaus(dE0,0.0373*dE0*sqrt(42/dE0)); // resolution changed 3.1 % -> 1.8 %, May 2 2017
		if(dE0<0.) dE0 = 0.;
		if(dE0>0){mul++;}
		if(P==0){
		dE[0] = dE0;
		dE_ideal[0] = dE_ideal0;
		}if(P==1){
		dE[1] = dE0;
		dE_ideal[1] = dE_ideal0;
		}else if(P==-1) {
		dE.push_back(dE0);
		dE_ideal.push_back(dE_ideal0);}
		rndm->Delete();
		//}
	return E;
}

