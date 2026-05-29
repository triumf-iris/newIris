#ifndef __CsI1PARTICLE_H
#define __CsI1PARTICLE_H

#include "TObject.h"
#include "TClass.h"
#include "TRandom3.h"
#include "TVector3.h"
#include "IRISHit.h"
#include "nucleus.h"
/****CsI hit***/

class CsIHit : public IRISHit
{
public:
	CsIHit();			 //! Create
	CsIHit(Double_t);	 //! Create
	virtual ~CsIHit() {} //!

	Bool_t Hit(Double_t theta, Double_t phi, Double_t distance, TVector3 targetPos, Int_t P) override; //!
	Double_t ELoss(nucleus ncl, Double_t E, Double_t theta) override;

protected:
private:

	ClassDefOverride(CsIHit, 1);
};

#endif
