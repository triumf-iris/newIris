#include <TMath.h>
#include "YYHit.h"
#include "TRandom3.h"
#include "TVector3.h"

ClassImp(YYHit);

YYHit::YYHit()
{
	Thickness[0] = 104.65 * 2.3212 * 0.1; 
	Thickness[1] = 101.15 * 2.3212 * 0.1; 
	Thickness[2] = 106.13 * 2.3212 * 0.1; 
	Thickness[3] = 101.75 * 2.3212 * 0.1; 
	Thickness[4] = 100.05 * 2.3212 * 0.1; 
	Thickness[5] = 105.65 * 2.3212 * 0.1; 
	Thickness[6] = 102.48 * 2.3212 * 0.1; 
	Thickness[7] = 105.84 * 2.3212 * 0.1; 
	Avg_Thickness = 103.46 * 2.3212 * 0.1; 
	mul = 0;
	
	fX.assign(2,NAN);
	fY.assign(2,NAN);
	fZ.assign(2,NAN);
	fThetaCalc.assign(2,NAN);
	fThetaRand.assign(2,NAN);
	fPhiCalc.assign(2,NAN);
	fPhiRand.assign(2,NAN);
	//hit.clear();
	Seg.assign(2,-1);
	Ring.assign(2,-1);
	dE.assign(2,NAN);
	dE_ideal.assign(2,NAN);
}

void YYHit::Init(Double_t th[8])
{
	Thickness[0] = th[0] * 2.3212 * 0.1; 
	Thickness[1] = th[1] * 2.3212 * 0.1; 
	Thickness[2] = th[2] * 2.3212 * 0.1; 
	Thickness[3] = th[3] * 2.3212 * 0.1; 
	Thickness[4] = th[4] * 2.3212 * 0.1; 
	Thickness[5] = th[5] * 2.3212 * 0.1; 
	Thickness[6] = th[6] * 2.3212 * 0.1; 
	Thickness[7] = th[7] * 2.3212 * 0.1; 
	Avg_Thickness = (th[0]+th[1]+th[2]+th[3]+th[4]+th[5]+th[6]+th[7])/8. * 2.3212 * 0.1; 
	//Thickness[-1] = Avg_Thickness;
	mul = 0;
	fX.assign(2,NAN);
	fY.assign(2,NAN);
	fZ.assign(2,NAN);
	fThetaCalc.assign(2,NAN);
	fThetaRand.assign(2,NAN);
	fPhiCalc.assign(2,NAN);
	fPhiRand.assign(2,NAN);
	//hit.clear();
	Seg.assign(2,-1);
	Ring.assign(2,-1);
	dE.assign(2,NAN);
	dE_ideal.assign(2,NAN);
}

void YYHit::Clear(Option_t *option)
{
	mul = 0;
	fX.assign(2,NAN);
	fY.assign(2,NAN);
	fZ.assign(2,NAN);
	fThetaCalc.assign(2,NAN);
	fThetaRand.assign(2,NAN);
	fPhiCalc.assign(2,NAN);
	fPhiRand.assign(2,NAN);
	//hit.clear();
	Seg.assign(2,-1);
	Ring.assign(2,-1);
	dE.assign(2,NAN);
	dE_ideal.assign(2,0);
}

Double_t YYHit::ThetaMin(Double_t YdDistance)
{	
	const Double_t RIn = 50.;	// Inner radius in mm
	Double_t theta_min = TMath::ATan2(RIn,YdDistance)*TMath::RadToDeg();
	return theta_min;
}

Double_t YYHit::ThetaMax(Double_t YdDistance)
{	
	const Double_t ROut = 129.9;	// Outer radius in mm
	Double_t theta_max = TMath::ATan2(ROut,YdDistance)*TMath::RadToDeg();
	return theta_max;
}

Bool_t YYHit::Hit(Double_t theta, Double_t phi, Double_t YdDistance, TVector3 targetPos,Double_t P)
{	
	const Double_t RIn = 50.;	// Inner radius in mm
	const Double_t ROut = 129.9;	// Outer radius in mm

	TRandom3 fRandom(0);
	Double_t fX0, fY0, fZ0;
	Double_t fThetaCalc0, fThetaRand0, fPhiCalc0, fPhiRand0;
   	Int_t Seg0, Ring0;
	
	Bool_t hitTheta=0;
	Bool_t hitPhi=0;
	Bool_t hitBool=0;
	Double_t phiGap = 4.25*TMath::DegToRad(); // phi gap between YY1s in rad 
	Double_t phiShift =  -103.*TMath::DegToRad();// shift from the vertical direction for the first YY1
	Double_t phiRel; // Relative phi after phishift
	Double_t phiRange = TMath::Pi()/4-phiGap; // ring dependent phi range for each YY1 

	phiRel = phi+phiShift;
	if (phiRel>TMath::Pi())  phiRel = phiRel - 2*TMath::Pi();
	if (phiRel<-TMath::Pi())  phiRel = phiRel + 2*TMath::Pi();
	
	//ring number in YY1 and geometric efficiency
	fX0 = YdDistance*tan(theta)*cos(phi);
	fY0 = YdDistance*tan(theta)*sin(phi);
	
	TVector3 partVec(fX0,fY0,YdDistance);
	partVec = partVec + targetPos; // taking into account the beam position at the target
	
	fX0 = partVec.X();
	fY0 = partVec.Y();
	fZ0 = partVec.Z();
	theta = partVec.Theta();
	phi = partVec.Phi();
	
	Seg0 = int((phiRel+TMath::Pi())/(TMath::Pi()/4));
  	Ring0 = int((YdDistance*tan(theta) - RIn)/5.);
	
	if (Ring0==13) phiRange =  TMath::Pi()/4-phiGap*2; 
	else if (Ring0==14) phiRange =  TMath::Pi()/4-phiGap*3.5; 
	else if (Ring0==15) phiRange =  TMath::Pi()/4-phiGap*5.5; 
	hitPhi = fabs(phiRel+TMath::Pi()- Seg0*TMath::Pi()/4.-TMath::Pi()/8.) < phiRange/2.;
	hitTheta = ((YdDistance*tan(theta)>RIn) && (YdDistance*tan(theta)<ROut));
	hitBool = (hitTheta && hitPhi);
	
	if (hitBool){
   		//mul++;
	  	fX0 = fX0;
		fY0 = fY0;	
  		Ring0 = Ring0;
  		Double_t randomTheta = fRandom.Uniform();
		Double_t randomPhi = fRandom.Uniform();
		Seg0 = 3-Seg0;
		if(Seg0<0) Seg0 = Seg0 + 8;
 		fPhiCalc0 = -phiShift*TMath::RadToDeg()-((Seg0+0.5)*45.);
 		if (fPhiCalc0<-180.)	fPhiCalc0 = fPhiCalc0+360.;
 		if (fPhiCalc0>180.)	fPhiCalc0 = fPhiCalc0-360.;
 		fThetaCalc0 = TMath::RadToDeg()*atan((50.+(Ring0*5.)+2.5)/YdDistance);
 		fThetaCalc0 = (fThetaCalc0>0) ? fThetaCalc0 : fThetaCalc0+180.;
 		fThetaRand0 = TMath::RadToDeg()*atan((50.+(Ring0*5.)+5.*randomTheta)/YdDistance);
 		fThetaRand0 = (fThetaRand0>0) ? fThetaRand0 : fThetaRand0+180.;
		fPhiRand0 = fPhiCalc0 +(randomPhi - 0.5) * phiRange * TMath::RadToDeg();
		if (fPhiRand0 < -180.) fPhiRand0 += 360.;
		if (fPhiRand0 >  180.) fPhiRand0 -= 360.;
		

   		if(P==0){
		fX[0]         = fX0;
		fY[0]         = fY0;
		fZ[0]         = fZ0;
		fThetaCalc[0] = fThetaCalc0;
		fThetaRand[0] = fThetaRand0;
		fPhiCalc[0]   = fPhiCalc0;
		fPhiRand[0]   = fPhiRand0;
		Seg[0]        = Seg0;
		Ring[0]       = Ring0;}
		else if(P==1){
		fX[1]         = fX0;
		fY[1]         = fY0;
		fZ[1]         = fZ0;
		fThetaCalc[1] = fThetaCalc0;
		fThetaRand[1] = fThetaRand0;
		fPhiCalc[1]   = fPhiCalc0;
		fPhiRand[1]   = fPhiRand0;
		Seg[1]        = Seg0;
		Ring[1]       = Ring0;}
		else if (P==-1){
		fX.push_back(fX0);
		fY.push_back(fY0);
		fZ.push_back(fZ0);
		fThetaCalc.push_back(fThetaCalc0);
		fThetaRand.push_back(fThetaRand0);
		fPhiCalc.push_back(fPhiCalc0);
		fPhiRand.push_back(fPhiRand0);
		Seg.push_back(Seg0);
		Ring.push_back(Ring0);}
	}
  	
	return hitBool;
}

Double_t YYHit::ELoss(nucleus ncl, Double_t E, Double_t Theta, Double_t P)
{
	Double_t T = (Theta<TMath::Pi()/2.) ? Theta : TMath::Pi()-Theta;
  	Double_t dE0, dE_ideal0;
	TRandom3 *rndm = new TRandom3(0);
	Int_t sector;
	if (P==-1) sector = Seg.size() - 1;
	else sector = Seg[P];
	E -= elMan->eloss(ncl,13./27.,E,0.1*2.702*0.1/cos(T),IrisMaterial::Al);
  	E -= elMan->eloss(ncl,5./10.,E,0.05*2.3502*0.1/cos(T),IrisMaterial::B);
  	dE0 = elMan->eloss(ncl,14./28.,E,Thickness[sector]/cos(T),IrisMaterial::Si);
  	dE_ideal0 = elMan->eloss(ncl,14./28.,E,Avg_Thickness/cos(T),IrisMaterial::Si);
	E = E-dE0;
	if(dE0<0.) dE0 = -dE0;
  	if(dE0!=0.) dE0 = rndm->Gaus(dE0,0.0023*dE0*sqrt(5.5/dE0)); //0.3% of 5.5 - S1752
	if(dE0<0.) dE0 = 0.;
	if(dE0>0){mul++;}
	if(P==0){dE[0]=dE0;
	dE_ideal[0]=dE_ideal0;}
	else if(P==1){dE[1]=dE0;
	dE_ideal[1]=dE_ideal0;}
	else if (P==-1){dE.push_back(dE0);
	dE_ideal.push_back(dE_ideal0);}
	rndm->Delete();
	return E;
}


