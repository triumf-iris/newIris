#include <TMath.h>
#include "YYHit.h"
#include "TRandom3.h"
#include "TVector3.h"

ClassImp(YYHit)

YYHit::YYHit() : IRISHit()
{
	SegThickness[0] = 104.65 * 2.3212 * 0.1;
	SegThickness[1] = 101.15 * 2.3212 * 0.1;
	SegThickness[2] = 106.13 * 2.3212 * 0.1;
	SegThickness[3] = 101.75 * 2.3212 * 0.1;
	SegThickness[4] = 100.05 * 2.3212 * 0.1;
	SegThickness[5] = 105.65 * 2.3212 * 0.1;
	SegThickness[6] = 102.48 * 2.3212 * 0.1;
	SegThickness[7] = 105.84 * 2.3212 * 0.1;
	Thickness = 103.46 * 2.3212 * 0.1;

	RIn = 50.;	  // Inner radius in mm
	ROut = 129.9; // Outer radius in mm
}

void YYHit::Init(Double_t th[8])
{
	Clear();
	SegThickness[0] = th[0] * 2.3212 * 0.1;
	SegThickness[1] = th[1] * 2.3212 * 0.1;
	SegThickness[2] = th[2] * 2.3212 * 0.1;
	SegThickness[3] = th[3] * 2.3212 * 0.1;
	SegThickness[4] = th[4] * 2.3212 * 0.1;
	SegThickness[5] = th[5] * 2.3212 * 0.1;
	SegThickness[6] = th[6] * 2.3212 * 0.1;
	SegThickness[7] = th[7] * 2.3212 * 0.1;
	Thickness = (th[0] + th[1] + th[2] + th[3] + th[4] + th[5] + th[6] + th[7]) / 8. * 2.3212 * 0.1;
}

Bool_t YYHit::Hit(Double_t theta, Double_t phi, Double_t distance, TVector3 targetPos, Int_t P)
{
	TRandom3 fRandom(0);
	Double_t fX0, fY0, fZ0;
	Double_t fThetaCalc0, fThetaRand0, fPhiCalc0, fPhiRand0;
	Int_t Seg0, Ring0;

	Bool_t hitTheta = 0;
	Bool_t hitPhi = 0;
	Bool_t hitBool = 0;
	Double_t phiGap = 4.25 * TMath::DegToRad();	   // phi gap between YY1s in rad
	Double_t phiShift = -103. * TMath::DegToRad(); // shift from the vertical direction for the first YY1
	Double_t phiRel;							   // Relative phi after phishift
	Double_t phiRange = TMath::Pi() / 4 - phiGap;  // ring dependent phi range for each YY1

	phiRel = phi + phiShift;
	if (phiRel > TMath::Pi())
		phiRel = phiRel - 2 * TMath::Pi();
	if (phiRel < -TMath::Pi())
		phiRel = phiRel + 2 * TMath::Pi();

	// ring number in YY1 and geometric efficiency
	fX0 = distance * tan(theta) * cos(phi);
	fY0 = distance * tan(theta) * sin(phi);

	TVector3 partVec(fX0, fY0, distance);
	partVec = partVec + targetPos; // taking into account the beam position at the target

	fX0 = partVec.X();
	fY0 = partVec.Y();
	fZ0 = partVec.Z();
	theta = partVec.Theta();
	phi = partVec.Phi();

	Seg0 = int((phiRel + TMath::Pi()) / (TMath::Pi() / 4));
	Ring0 = int((distance * tan(theta) - RIn) / 5.);

	if (Ring0 == 13)
		phiRange = TMath::Pi() / 4 - phiGap * 2;
	else if (Ring0 == 14)
		phiRange = TMath::Pi() / 4 - phiGap * 3.5;
	else if (Ring0 == 15)
		phiRange = TMath::Pi() / 4 - phiGap * 5.5;
	hitPhi = fabs(phiRel + TMath::Pi() - Seg0 * TMath::Pi() / 4. - TMath::Pi() / 8.) < phiRange / 2.;
	hitTheta = ((distance * tan(theta) > RIn) && (distance * tan(theta) < ROut));
	hitBool = (hitTheta && hitPhi);

	if (hitBool)
	{
		// fMul++;
		fX0 = fX0;
		fY0 = fY0;
		Ring0 = Ring0;
		Double_t randomTheta = fRandom.Uniform();
		Double_t randomPhi = fRandom.Uniform();
		Seg0 = 3 - Seg0;
		if (Seg0 < 0)
			Seg0 = Seg0 + 8;
		fPhiCalc0 = -phiShift * TMath::RadToDeg() - ((Seg0 + 0.5) * 45.);
		if (fPhiCalc0 < -180.)
			fPhiCalc0 = fPhiCalc0 + 360.;
		if (fPhiCalc0 > 180.)
			fPhiCalc0 = fPhiCalc0 - 360.;
		fThetaCalc0 = TMath::RadToDeg() * atan((50. + (Ring0 * 5.) + 2.5) / distance);
		fThetaCalc0 = (fThetaCalc0 > 0) ? fThetaCalc0 : fThetaCalc0 + 180.;
		fThetaRand0 = TMath::RadToDeg() * atan((50. + (Ring0 * 5.) + 5. * randomTheta) / distance);
		fThetaRand0 = (fThetaRand0 > 0) ? fThetaRand0 : fThetaRand0 + 180.;
		fPhiRand0 = fPhiCalc0 + (randomPhi - 0.5) * phiRange * TMath::RadToDeg();
		if (fPhiRand0 < -180.)
			fPhiRand0 += 360.;
		if (fPhiRand0 > 180.)
			fPhiRand0 -= 360.;

		fX.push_back(fX0);
		fY.push_back(fY0);
		fZ.push_back(fZ0);
		fThetaCalc.push_back(fThetaCalc0);
		fThetaRand.push_back(fThetaRand0);
		fPhiCalc.push_back(fPhiCalc0);
		fPhiRand.push_back(fPhiRand0);
		fSeg.push_back(Seg0);
		fRing.push_back(Ring0);
		fChannel.push_back(Seg0 * 16 + Ring0);
		fNo.push_back(Seg0);
		fPType.push_back(P);
	}

	return hitBool;
}

Double_t YYHit::ELoss(nucleus ncl, Double_t E, Double_t theta)
{
	Double_t T = (theta < TMath::Pi() / 2.) ? theta : TMath::Pi() - theta;
	Double_t dE0, dE_ideal0;
	TRandom3 *rndm = new TRandom3(0);
	Int_t sector;
	sector = fSeg.size() - 1;
	E -= elMan->eloss(ncl, 13. / 27., E, 0.1 * 2.702 * 0.1 / cos(T), IrisMaterial::Al);
	E -= elMan->eloss(ncl, 5. / 10., E, 0.05 * 2.3502 * 0.1 / cos(T), IrisMaterial::B);
	dE0 = elMan->eloss(ncl, 14. / 28., E, SegThickness[sector] / cos(T), IrisMaterial::Si);
	dE_ideal0 = elMan->eloss(ncl, 14. / 28., E, Thickness / cos(T), IrisMaterial::Si);
	E = E - dE0;
	if (dE0 < 0.)
		dE0 = -dE0;
	if (dE0 != 0.)
		dE0 = rndm->Gaus(dE0, 0.0023 * dE0 * sqrt(5.5 / dE0)); // 0.3% of 5.5 - S1752
	if (dE0 < 0.)
		dE0 = 0.;
	if (dE0 > 0)
	{
		fMul++;
	}
	fdE.push_back(dE0);
	fdE_ideal.push_back(dE_ideal0);
	rndm->Delete();
	return E;
}
