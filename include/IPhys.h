#ifndef __IPHYS_H
#define __IPHYS_H

#include "TObject.h"
#include <vector>
#include "TLorentzVector.h"


class IPhys : public TObject {
public:
    IPhys();
    virtual ~IPhys();   // <-- must declare this if you have virtual methods

    void Clear(Option_t *option = "") override; //!

    std::vector<Double_t> Q_ydcsi;
    std::vector<Double_t> YdCsIETot;
    std::vector<Double_t> Eb_ydcsi;
    std::vector<Double_t> PB_ydcsi;
  
    std::vector<Double_t> Q_sd;
    std::vector<Double_t> S3Tot;
    std::vector<Double_t> EB_sd;
    std::vector<Double_t> PB_sd;
    
    Double_t Qyu=sqrt(-1.);
    Double_t YuTot=sqrt(-1.);
    Double_t EB_yu;
    Double_t PB_yu=sqrt(-1.);
    
    Double_t Qsu=sqrt(-1.);
    Double_t SuTot=sqrt(-1.);
    Double_t EB_su;
    Double_t PB_su=sqrt(-1.);
    
    Double_t beamE=0.;
    Double_t beamBeta=0.;
    Double_t beamGamma=0.;
    Double_t beamEcm=0.;
    Double_t convertMM=0;
    Double_t adjustZpos=0;
    Double_t SSBdE=0.;
    Double_t Qgen;    

private:
    ClassDefOverride(IPhys, 1)
};

#endif

