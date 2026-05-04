#ifndef __S3HIT_H
#define __S3HIT_H

#include "TObject.h"
#include "TClass.h"
#include "TRandom3.h"
#include "TVector3.h"
#include "IRISHit.h"
#include "nucleus.h"
#include "EnergyLossManager.h"

class S3Hit : public IRISHit
{
public:
	S3Hit();			//! Create
	virtual ~S3Hit() {} //!

	// S3Hit(const S3Hit &);                          // The copy constructor.
	void Init(Bool_t, Double_t);																	   //! Create
	Bool_t Hit(Double_t theta, Double_t phi, Double_t distance, TVector3 targetPos, Int_t P) override; //!
	Double_t ELoss(nucleus ncl, Double_t E, Double_t theta) override;

	Bool_t GetOrientation() { return fOrientation; }

protected:
private:
	Bool_t fOrientation; // 0 = rings first, 1 = sectors first

	ClassDefOverride(S3Hit, 1);
};

#endif
