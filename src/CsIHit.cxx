#include "CsIHit.h"
#include "IrisMaterial.h"

ClassImp(CsIHit)

	CsIHit::CsIHit() : IRISHit()
{
	Thickness = 12000. * 4.51 * 0.1; // um*g/cm^3*0.1;
	RIn = 50.;						 // Inner radius in mm
	ROut = 150.;					 // Outer radius in mm
}

CsIHit::CsIHit(Double_t th) : IRISHit()
{
	Thickness = th * 4.51 * 0.1;
	RIn = 50.;	 // Inner radius in mm
	ROut = 150.; // Outer radius in mm
}

Bool_t CsIHit::Hit(Double_t theta, Double_t phi, Double_t distance, TVector3 targetPos, Int_t P)
{
	const Double_t RIn = 50.;	// Inner radius in mm
	const Double_t ROut = 150.; // Outer radius in mm

	TRandom3 fRandom(0);
	Double_t fX0, fY0, fZ0;
	Double_t fPhiCalc0, fPhiRand0;
	Int_t Seg0;

	Bool_t hitTheta = 0;
	Bool_t hitPhi = 0;
	Bool_t hitBool = 0;							   // return value
	Double_t phiGap = 0.1 * TMath::DegToRad();	   // phi gap between CsI1s in rad
	Double_t phiShift = -103. * TMath::DegToRad(); // shift from the vertical direction for the first YY1
	Double_t phiRel;							   // Relative phi after phishift
	Double_t phiRange = TMath::Pi() / 8 - phiGap;  // phi range for each CsI

	phiRel = phi + phiShift;
	if (phiRel > TMath::Pi())
		phiRel = phiRel - 2 * TMath::Pi();
	if (phiRel < -TMath::Pi())
		phiRel = phiRel + 2 * TMath::Pi();

	// geometric efficiency
	fX0 = distance * tan(theta) * cos(phi);
	fY0 = distance * tan(theta) * sin(phi);

	TVector3 partVec(fX0, fY0, distance);

	partVec = partVec + targetPos; // taking into account the beam position at the target

	fX0 = partVec.X();
	fY0 = partVec.Y();
	fZ0 = partVec.Z();
	theta = partVec.Theta();
	phi = partVec.Phi();

	hitTheta = ((distance * tan(theta) > RIn) && (distance * tan(theta) < ROut));
	Seg0 = int((phiRel + TMath::Pi()) / (TMath::Pi() / 8));
	hitPhi = fabs(phiRel + TMath::Pi() - Seg0 * TMath::Pi() / 8. - TMath::Pi() / 16.) < phiRange / 2.;
	hitBool = (hitPhi && hitTheta);

	if (hitBool)
	{
		fX0 = fX0;
		fY0 = fY0;
		Seg0 = 7 - Seg0;
		if (Seg0 < 0)
			Seg0 = Seg0 + 16;
		fPhiCalc0 = -phiShift * TMath::RadToDeg() - ((Seg0 + 0.5) * 22.5);
		if (fPhiCalc0 < -180.)
			fPhiCalc0 = fPhiCalc0 + 360.;
		if (fPhiCalc0 > 180.)
			fPhiCalc0 = fPhiCalc0 - 360.;

		Double_t random = 0.99 * fRandom.Rndm();
		fPhiRand0 = -phiShift * TMath::RadToDeg() - ((Seg0 + random) * 22.5);
		if (fPhiRand0 < -180.)
			fPhiRand0 = fPhiRand0 + 360.;
		if (fPhiRand0 > 180.)
			fPhiRand0 = fPhiRand0 - 360.;
		fX.push_back(fX0);
		fY.push_back(fY0);
		fZ.push_back(fZ0);
		fPhiCalc.push_back(fPhiCalc0);
		fPhiRand.push_back(fPhiRand0);
		fSeg.push_back(Seg0);
		fChannel.push_back(Seg0);
		fPType.push_back(P);
	}

	return hitBool;
}

Double_t CsIHit::ELoss(nucleus ncl, Double_t E, Double_t theta)
{
	Double_t dE0, dE_ideal0;
	// if(fMul>0 && hit0==1){
	TRandom3 *rndm = new TRandom3(0);
	E -= eloss(ncl, 15. / 31., E, 0.1 * 1.8219 * 0.1 / cos(theta), IrisMaterial::P);
	E -= eloss(ncl, 13. / 27., E, 0.3 * 2.702 * 0.1 / cos(theta), IrisMaterial::Al);
	E -= eloss(ncl, 100. / 192., E, 6. * 1.4 * 0.1 / cos(theta), IrisMaterial::Mylar);
	dE0 = eloss(ncl, 108. / 260., E, Thickness / cos(theta), IrisMaterial::CsI);
	E -= dE0;
	if (dE0 < 0.)
		dE0 = -dE0;
	dE_ideal0 = dE0;
	if (dE0 != 0.)
		dE0 = rndm->Gaus(dE0, 0.0373 * dE0 * sqrt(42 / dE0)); // resolution changed 3.1 % -> 1.8 %, May 2 2017
	if (dE0 < 0.)
		dE0 = 0.;
	if (dE0 > 0)
	{
		fMul++;
	}
	fdE.push_back(dE0);
	fdE_ideal.push_back(dE_ideal0);
	rndm->Delete();
	//}
	return E;
}
