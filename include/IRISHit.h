#ifndef __IRISHIT_H
#define __IRISHIT_H

#include "TObject.h"
#include "TClass.h"
#include "TRandom3.h"
#include "TVector3.h"
#include "nucleus.h"
#include "eloss.h"

class IRISHit : public TObject
{
public:
	IRISHit();			  //! Create
	virtual ~IRISHit() {} //!

	Double_t ThetaMin(Double_t distance);															  //!
	Double_t ThetaMax(Double_t distance);															  //!
	virtual Bool_t Hit(Double_t theta, Double_t phi, Double_t distance, TVector3 targetPos, Int_t P) { return false; } //!
	virtual Double_t ELoss(nucleus ncl, Double_t E, Double_t theta) { return 0; }
	void Clear(Option_t *option = "") override; //!

	Int_t GetMul() { return fMul; }
	Int_t GetSize() { return fdE.size(); }

	std::vector<Double_t> GetX() { return fX; }
	Double_t GetX(int i) { return fX[i]; }
	std::vector<Double_t> GetY() { return fY; }
	Double_t GetY(int i) { return fY[i]; }
	std::vector<Double_t> GetZ() { return fZ; }
	Double_t GetZ(int i) { return fZ[i]; }
	std::vector<Double_t> GetThetaCalc() { return fThetaCalc; }
	Double_t GetThetaCalc(int i) { return fThetaCalc[i]; }
	std::vector<Double_t> GetThetaRand() { return fThetaRand; }
	Double_t GetThetaRand(int i) { return fThetaRand[i]; }
	std::vector<Double_t> GetPhiCalc() { return fPhiCalc; }
	Double_t GetPhiCalc(int i) { return fPhiCalc[i]; }
	std::vector<Double_t> GetPhiRand() { return fPhiRand; }
	Double_t GetPhiRand(int i) { return fPhiRand[i]; }
	std::vector<Double_t> GetdE() { return fdE; }
	Double_t GetdE(int i) { return fdE[i]; }
	std::vector<Double_t> GetdE_ideal() { return fdE_ideal; }
	Double_t GetdE_ideal(int i) { return fdE_ideal[i]; }
	std::vector<Int_t> GetSeg() { return fSeg; }
	Int_t GetSeg(int i) { return fSeg[i]; }
	std::vector<Int_t> GetRing() { return fRing; }
	Int_t GetRing(int i) { return fRing[i]; }
	std::vector<Int_t> GetChannel() { return fChannel; }
	Int_t GetChannel(int i) { return fChannel[i]; }
	std::vector<Int_t> GetNo() { return fNo; }
	Int_t GetNo(int i) { return fNo[i]; }
	std::vector<Int_t> GetPType() { return fPType; }
	Int_t GetPType(int i) { return fPType[i]; }

protected:
	Double_t RIn;
	Double_t ROut;
	Double_t Thickness; // in um
	Int_t fMul;
	std::vector<Double_t> fX;
	std::vector<Double_t> fY;
	std::vector<Double_t> fZ;		  // should be equal to distance to YY1
	std::vector<Double_t> fThetaCalc; // Hitd theta (using YdRing)
	std::vector<Double_t> fThetaRand; // Hitd theta (using YdRing)
	std::vector<Double_t> fPhiCalc;	  // Hitd phi (using Seg)
	std::vector<Double_t> fPhiRand;	  // Hitd phi (using Seg)
	std::vector<Double_t> fdE;		  // Energy loss
	std::vector<Double_t> fdE_ideal;  // Energy loss
	std::vector<Int_t> fSeg;
	std::vector<Int_t> fRing;
	std::vector<Int_t> fChannel;
	std::vector<Int_t> fNo;
	std::vector<Int_t> fPType;

private:
	ClassDefOverride(IRISHit, 1);
};

#endif
