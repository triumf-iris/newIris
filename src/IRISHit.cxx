#include <TMath.h>
#include "IRISHit.h"
#include "TRandom3.h"
#include "TVector3.h"

ClassImp(IRISHit)

	IRISHit::IRISHit()
{
	Clear();
}

void IRISHit::Clear(Option_t *option)
{
	fMul = 0;

	fX.clear();
	fY.clear();
	fZ.clear();
	fThetaCalc.clear();
	fThetaRand.clear();
	fPhiCalc.clear();
	fPhiRand.clear();
	fdE.clear();
	fdE_ideal.clear();
	fSeg.clear();
	fRing.clear();
	fChannel.clear();
	fNo.clear();
	fPType.clear();
}

Double_t IRISHit::ThetaMin(Double_t distance)
{
	Double_t theta_min = TMath::ATan2(RIn, distance) * TMath::RadToDeg();
	return theta_min;
}

Double_t IRISHit::ThetaMax(Double_t distance)
{
	Double_t theta_max = TMath::ATan2(ROut, distance) * TMath::RadToDeg();
	return theta_max;
}
