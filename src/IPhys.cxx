#include "IPhys.h"
#include "header.h"

ClassImp(IPhys)

IPhys::IPhys() {
    Clear();
}

IPhys::~IPhys() {}   // <-- must define destructor

void IPhys::Clear(Option_t *option) {
    
    Q_ydcsi.assign(2, NAN);
    YdCsIETot.assign(2, NAN);
    Eb_ydcsi.assign(2, NAN);
    PB_ydcsi.assign(2, NAN);

	Q_sd.assign(2, NAN); 
    S3Tot.assign(2, NAN);
    EB_sd.assign(2, NAN); 
    PB_sd.assign(2, NAN);

    Qyu=sqrt(-1.);
    YuTot=sqrt(-1.);
    //EB_yu;
    PB_yu=sqrt(-1.);

    Qsu=sqrt(-1.);
    SuTot=sqrt(-1.);
    //EB_su;
    PB_su=sqrt(-1.);
    
    beamE=0.;
    beamBeta=0.;
    beamGamma=0.;
    beamEcm=0.;
    convertMM=0;
    adjustZpos=0;
    SSBdE=0.;
    Qgen=sqrt(-1.);    

}
