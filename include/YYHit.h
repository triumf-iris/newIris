#ifndef __YYHIT_H
#define __YYHIT_H

#include "TObject.h"
#include "TClass.h"
#include "TRandom3.h"
#include "TVector3.h"
#include "IRISHit.h"
#include "nucleus.h"
#include "EnergyLossManager.h"

class YYHit : public IRISHit
{
public:
	YYHit();			//! Create
	virtual ~YYHit() {} //!

	// YYHit(const YYHit &);                          // The copy constructor.
	void Init(Double_t[8]);																			   //! Create
	Bool_t Hit(Double_t theta, Double_t phi, Double_t distance, TVector3 targetPos, Int_t P) override; //!
	Double_t ELoss(nucleus ncl, Double_t E, Double_t theta) override;

protected:
private:
	Double_t SegThickness[8]; // in um

	ClassDefOverride(YYHit, 1);
};

#endif
