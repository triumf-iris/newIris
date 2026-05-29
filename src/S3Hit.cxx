#include <TMath.h>
#include "S3Hit.h"
#include "TRandom3.h"
#include "TVector3.h"
#include "IrisMaterial.h"

ClassImp(S3Hit)

S3Hit::S3Hit() : IRISHit()
{
	fOrientation = 0;				// 0 = rings first, 1 = sectors first
	Thickness = 60. * 2.3212 * 0.1; // um*g/cm^3*0.1
	RIn = 11.;						// Inner radius in mm
	ROut = 35.;						// Outer radius in mm
}

void S3Hit::Init(Bool_t o, Double_t th)
{
	fOrientation = o;			   // 0 = rings first, 1 = sectors first
	Thickness = th * 2.3212 * 0.1; // um*g/cm^3*0.1
	Clear();
}

Bool_t S3Hit::Hit(Double_t theta, Double_t phi, Double_t distance, TVector3 targetPos, Int_t P)
{
	const Double_t RIn = 11.;  // Inner radius in mm
	const Double_t ROut = 35.; // Outer radius in mm

	TRandom3 fRandom(0);
	Double_t fX0, fY0, fZ0;
	Double_t fPhiCalc0, fPhiRand0;
	Double_t fThetaCalc0, fThetaRand0;
	Int_t Seg0, Ring0;

	// ring number in S3 and geometric efficiency
	fX0 = distance * tan(theta) * cos(phi);
	fY0 = distance * tan(theta) * sin(phi);

	TVector3 partVec(fX0, fY0, distance);

	partVec = partVec + targetPos; // taking into account the beam position at the target

	fX0 = partVec.X();
	fY0 = partVec.Y();
	fZ0 = partVec.Z();
	theta = partVec.Theta();
	phi = partVec.Phi();

	// geometric efficiency
	Bool_t hitBool = ((distance * tan(theta) > RIn) && (distance * tan(theta) < ROut));

	if (hitBool)
	{

		// hit[fMul] = 1;
		Ring0 = int(distance * tan(theta) - RIn);
		Seg0 = int((TMath::Pi() + phi) * 16. / TMath::Pi());
		fPhiCalc0 = -180. + (Seg0 * 11.25);
		if (fPhiCalc0 < -180.)
			fPhiCalc0 = fPhiCalc0 + 360.;
		Double_t rndm = 0.99 * fRandom.Rndm();
		fPhiRand0 = -180. + (Seg0 + rndm) * 11.25;
		if (fPhiRand0 < -180.)
			fPhiRand0 = fPhiRand0 + 360.;
		if (fOrientation == 1)
		{
			Seg0 = 31 - Seg0;
			// fPhiCalc[fMul] = -fPhiCalc[fMul];
			// fPhiRand[fMul] = -fPhiRand[fMul];
		}
		fThetaCalc0 = TMath::RadToDeg() * atan((RIn + Ring0 + 0.5) / distance);
		fThetaCalc0 = (fThetaCalc0 > 0) ? fThetaCalc0 : fThetaCalc0 + 180.;
		rndm = 0.99 * fRandom.Rndm();
		fThetaRand0 = TMath::RadToDeg() * atan((RIn + Ring0 + rndm) / distance);
		fThetaRand0 = (fThetaRand0 > 0) ? fThetaRand0 : fThetaRand0 + 180.;
		fX.push_back(fX0);
		fY.push_back(fY0);
		fZ.push_back(fZ0);
		fPhiCalc.push_back(fPhiCalc0);
		fPhiRand.push_back(fPhiRand0);
		fThetaCalc.push_back(fThetaCalc0);
		fThetaRand.push_back(fThetaRand0);
		fSeg.push_back(Seg0);
		fRing.push_back(Ring0);
		fChannel.push_back(Ring0);
		fNo.push_back(Seg0);
		fPType.push_back(P);
	}

	return hitBool;
}

Double_t S3Hit::ELoss(nucleus ncl, Double_t E, Double_t theta)
{
	Double_t dE0, dE_ideal0;
	Double_t T = (theta < TMath::Pi() / 2.) ? theta : TMath::Pi() - theta;
	TRandom3 *rndm = new TRandom3(0);

	if (fOrientation == 0)
	{																						 // rings first
		E -= eloss(ncl, 13. / 27., E, 0.1 * 2.702 * 1.5 / cos(T), IrisMaterial::Al);	 // first metal
		E -= eloss(ncl, 30. / 60., E, 0.1 * 2.65 * 3.5 / cos(T), IrisMaterial::SiO2); // SiO2
		E -= eloss(ncl, 13. / 27., E, 0.1 * 2.702 * 0.3 / cos(T), IrisMaterial::Al);	 // second metal
		E -= eloss(ncl, 5. / 10., E, 0.1 * 2.3502 * 0.5 / cos(T), IrisMaterial::B);	 // boron junction implant
		dE0 = eloss(ncl, 14. / 28., E, Thickness / cos(T), IrisMaterial::Si);
		E -= dE0;
		E -= eloss(ncl, 15. / 31., E, 0.1 * 1.822 * 0.5 / cos(T), IrisMaterial::P);	// phosphorus implant
		E -= eloss(ncl, 13. / 27., E, 0.1 * 2.702 * 0.3 / cos(T), IrisMaterial::Al); // metal
		// E -= dE0;
		// if(dE0<0.) dE0 = -dE0;
		// dE_ideal0 = dE0;
		// if(dE0!=0.) dE0 = rndm->Gaus(dE0,0.01*dE0);
		// if(dE0<0.) dE0 = -dE0;
	}
	else
	{																						// sectors first
		E -= eloss(ncl, 13. / 27., E, 0.1 * 2.702 * 0.3 / cos(T), IrisMaterial::Al); // metal
		E -= eloss(ncl, 15. / 31., E, 0.1 * 1.822 * 0.5 / cos(T), IrisMaterial::P);	// phosphorus implant
		dE0 = eloss(ncl, 14. / 28., E, Thickness / cos(T), IrisMaterial::Si);
		E -= dE0;
		// if(dE0<0.) dE0 = -dE0;
		// dE_ideal0 = dE0;
		// if(dE0!=0.) dE0 = rndm->Gaus(dE0,0.01*dE0);
		// if(dE0<0.) dE0 = -dE0;
	}
	// E -= dE0;
	if (dE0 < 0.)
		dE0 = -dE0;
	dE_ideal0 = dE0;
	if (dE0 != 0.)
		dE0 = rndm->Gaus(dE0, 0.0225 * dE0);
	if (dE0 < 0.)
		dE0 = -dE0;
	if (dE0 > 0)
	{
		fMul++;
	}
	fdE.push_back(dE0);
	fdE_ideal.push_back(dE_ideal0);
	rndm->Delete();
	return E;
}
