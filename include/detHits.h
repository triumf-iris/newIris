#include "header.h"
#include "EnergyLossManager.h"
#include "IrisMaterial.h"

Bool_t detHits(PTrack tr, nucleus ncl, TVector3 reacPos, Bool_t maskIn, Bool_t shieldIn, Double_t P)
{
	Bool_t mask = maskClear(tr.T,tr.P) || !maskIn;
	Bool_t shield = shieldClear(tr.T,tr.P) || !shieldIn;
	Bool_t forward = (tr.T<TMath::Pi()/2.);
	Bool_t backward = (tr.T>TMath::Pi()/2.);
	Bool_t YYHit = 0;
	Bool_t CsIHit = 0;
	Bool_t Sd1Hit = 0;
	Bool_t Sd2Hit = 0;
	Bool_t YuHit = 0;
	Bool_t SuHit = 0;

	Double_t ETmp = tr.Ebt;
	Double_t ETmpU = tr.Ebt;

	if(mask && shield&&backward){
		YuHit = yu.Hit(tr.T,tr.P,geoPrm.DYYU,reacPos,P) ;
		SuHit = su.Hit(tr.T,tr.P,geoPrm.DS3U,reacPos,P);
		if(YuHit) ETmpU = yu.ELoss(ncl,ETmpU,tr.T,P);
		if(SuHit) ETmpU = su.ELoss(ncl,ETmpU,tr.T,P);
	}	
	
	if(mask && shield&&forward){
		YYHit = yd.Hit(tr.T,tr.P,geoPrm.DYY,reacPos,P) ;
		if(YYHit) CsIHit = csi.Hit(tr.T,tr.P,geoPrm.DYY+11.6,reacPos,P) ;
		Sd1Hit = sd1.Hit(tr.T,tr.P,geoPrm.DS3,reacPos,P);
		if(Sd1Hit) Sd2Hit = sd2.Hit(tr.T,tr.P,geoPrm.DS3+14.8,reacPos,P);
		if(YYHit) ETmp = yd.ELoss(ncl,ETmp,tr.T,P);
		if(CsIHit) ETmp = csi.ELoss(ncl,ETmp,tr.T,P);
		if(Sd1Hit) ETmp = sd1.ELoss(ncl,ETmp,tr.T,P);
		if(Sd2Hit) ETmp = sd2.ELoss(ncl,ETmp,tr.T,P);
	}	
	
	return (mask && shield && YYHit && CsIHit);
}

// Calculate the energy loss of the scattered particles in Foil and SHT
PTrack TgtELoss(PTrack tr, nucleus ncl, geoParams g, Double_t reacZ, Bool_t isSHTReac)
{
	if(isSHTReac){ //Reaction in SHT
		if(g.Orientation==0&&tr.T<TMath::Pi()/2.){ // foil before target, theta<90 deg
	   		tr.FoildE = 0.;	
			tr.TrgtdE = elMan->eloss(ncl,1./g.AoZTgt,tr.E,(g.TTgt-reacZ)/TMath::Cos(tr.T),IrisMaterial::Target);	
		}
		if(g.Orientation==0&&tr.T>TMath::Pi()/2.){ // foil before target, theta>90 deg
			tr.TrgtdE = elMan->eloss(ncl,1./g.AoZTgt,tr.E,reacZ/TMath::Cos(TMath::Pi()-tr.T),IrisMaterial::Target);	
			tr.FoildE = elMan->eloss(ncl,1./g.AoZFoil,tr.E-tr.TrgtdE,g.TFoil/TMath::Cos(TMath::Pi()-tr.T),IrisMaterial::Foil);	
		}
		if(g.Orientation==1&&tr.T<TMath::Pi()/2.){ // foil after target, theta<90 deg
			tr.TrgtdE = elMan->eloss(ncl,1./g.AoZTgt,tr.E,(g.TTgt-reacZ)/TMath::Cos(tr.T),IrisMaterial::Target);	
			tr.FoildE = elMan->eloss(ncl,1./g.AoZFoil,tr.E-tr.TrgtdE,g.TFoil/TMath::Cos(tr.T),IrisMaterial::Foil);	
		}
		if(g.Orientation==1&&tr.T>TMath::Pi()/2.){ // foil after target, theta>90 deg
			tr.TrgtdE = elMan->eloss(ncl,1./g.AoZTgt,tr.E,reacZ/TMath::Cos(TMath::Pi()-tr.T),IrisMaterial::Target);	
	   		tr.FoildE = 0.;	
		}
	}
    else{ //Reaction in foil
        
        if(tr.T<TMath::Pi()/2.){
            tr.TrgtdE = 0.;
            tr.FoildE = elMan->eloss(ncl,1./g.AoZFoil,tr.E,(g.TFoil-reacZ)/TMath::Cos(tr.T),IrisMaterial::Foil);
        }
        if(tr.T>TMath::Pi()/2.){ // foil after target, theta>90 deg
            tr.FoildE = elMan->eloss(ncl,1./g.AoZFoil,tr.E,reacZ/TMath::Cos(TMath::Pi()-tr.T),IrisMaterial::Foil);
            tr.TrgtdE =0;
        }
    }
	
	tr.Ebt = tr.E-tr.FoildE-tr.TrgtdE; // calculate energy of particle after foil and target
	//printf("In: %f\tFoil: %f\tTarget: %f\tLeft: %f\n",tr.E,tr.FoildE,tr.TrgtdE,tr.Ebt);
	return tr;
}

void setIDet(Double_t ICdE, Double_t SSBdE, Bool_t sortEnergies)
{
	if(yd.mul>0)
	{
  		det.TYdMul = yd.mul;
		for(size_t i=0; i<yd.dE.size(); i++){ // yd.dE.size() used instead of Mul to consider Mul=2 and Yd.size() = 3
			det.TYdEnergy.push_back(yd.dE[i]);
  			det.TYdTheta.push_back(yd.fThetaRand[i]);// Yd theta angle
			det.TYdPhi.push_back(yd.fPhiRand[i]);// Yd theta angle                                                                        
			det.TYdChannel.push_back(yd.Seg[i]*16+yd.Ring[i]);
			det.TYdNo.push_back(yd.Seg[i]);
			det.TYdRing.push_back(yd.Ring[i]);
		}

	if(sortEnergies==1){
	Bool_t have_swapped = true;
		while(have_swapped == true){
			for (size_t x=0; x<det.TYdEnergy.size(); x++){
				have_swapped = false;
				for(size_t y=0; y<det.TYdEnergy.size()-1; y++){
					if(det.TYdEnergy[y]<det.TYdEnergy[y+1]){
						std::swap(det.TYdEnergy[y],det.TYdEnergy[y+1]);
						std::swap(det.TYdTheta[y],det.TYdTheta[y+1]);
						std::swap(det.TYdPhi[y],det.TYdPhi[y+1]);
						std::swap(det.TYdChannel[y],det.TYdChannel[y+1]);
						std::swap(det.TYdNo[y],det.TYdNo[y+1]);
						std::swap(det.TYdRing[y],det.TYdRing[y+1]);
						have_swapped = true;
					}
				}
			}
		}
	}
	}

	det.TCsI1Energy.resize(yd.dE.size(),NAN);
	det.TCsI1Channel.resize(yd.dE.size(),-1);
	det.TCsI1Phi.resize(yd.dE.size(),NAN);
	det.TCsI2Energy.resize(yd.dE.size(),NAN);
	det.TCsI2Channel.resize(yd.dE.size(),-1);
	det.TCsI2Phi.resize(yd.dE.size(),NAN);
	if(csi.mul>0 && det.TYdMul>0)
	{
		for(size_t i=0; i<csi.dE.size(); i++){
			for(size_t l=0;l<det.TYdEnergy.size();l++){
				if(((csi.Seg[i]/2)-det.TYdNo.at(l))==0){
					if(csi.dE[i]>0 && det.TYdEnergy.at(l)>0){
					det.TCsI1Mul++;
					det.TCsI2Mul++;
					det.TCsI1Energy.at(l)=csi.dE[i];
					det.TCsI2Energy.at(l)=csi.dE[i];
					det.TCsI1Channel.at(l)=csi.Seg[i];
					det.TCsI2Channel.at(l)=csi.Seg[i];
					det.TCsI1Phi.at(l)=csi.fPhiRand[i];
					det.TCsI2Phi.at(l)=csi.fPhiRand[i];
					det.TYdPhi.at(l)=csi.fPhiRand[i];}
				}
			}
		}
	}

	det.TSSBEnergy=SSBdE;
   	det.TICEnergy.push_back(ICdE);
	det.TICChannel.push_back(15);

	if(sd1.mul>0)
	{
		det.TSd1rMul=sd1.mul;
		for(Int_t i=0; i<det.TSd1rMul; i++){
  			det.TSd1rEnergy.push_back(sd1.dE[i]);
			det.TSd1rChannel.push_back(sd1.Ring[i]);
  			det.TSd1Theta.push_back(sd1.fThetaRand[i]);
		}
		det.TSd1sMul=sd1.mul;
		for(Int_t i=0; i<det.TSd1sMul; i++){
  			det.TSd1sEnergy.push_back(sd1.dE[i]);
			det.TSd1sChannel.push_back(sd1.Seg[i]);
  			det.TSd1Phi.push_back(sd1.fPhiRand[i]);
		}
		if(sortEnergies==1){
			Bool_t have_swapped = true;
			while(have_swapped == true){
			for (size_t x=0; x<det.TSd1rEnergy.size(); x++)
			{
				have_swapped = false;
				for(size_t y=0; y<det.TSd1rEnergy.size()-1; y++){
					if(det.TSd1rEnergy[y]<det.TSd1rEnergy[y+1]){
						std::swap(det.TSd1rEnergy[y],det.TSd1rEnergy[y+1]);
						std::swap(det.TSd1rChannel[y],det.TSd1rChannel[y+1]);
						std::swap(det.TSd1Theta[y],det.TSd1Theta[y+1]);
						std::swap(det.TSd1sEnergy[y],det.TSd1sEnergy[y+1]);
						std::swap(det.TSd1sChannel[y],det.TSd1sChannel[y+1]);
						std::swap(det.TSd1Phi[y],det.TSd1Phi[y+1]);
						have_swapped = true;
						}
					}
				}
			}
		}
	}

	det.TSd2rEnergy.resize(sd1.dE.size(),NAN);
	det.TSd2rChannel.resize(sd1.dE.size(),-1);
	det.TSd2Theta.resize(sd1.dE.size(),NAN);
	det.TSd2sEnergy.resize(sd1.dE.size(),NAN);
	det.TSd2sChannel.resize(sd1.dE.size(),-1);
	det.TSd2Phi.resize(sd1.dE.size(),NAN);
	if(det.TSd1rMul>0 && sd2.mul>0)
	{
		det.TSd2rMul = sd2.dE.size();
		for(size_t i=0; i<sd2.dE.size(); i++){ if(sd2.dE[i]>0){
			bool RingMatch=0, SectorMatch=0;
			for(size_t l=0;l<det.TSd1rEnergy.size();l++){ if (det.TSd1rEnergy.at(l)>0) {
				if((sd2.Ring[i]-det.TSd1rChannel.at(l))>=0 && (sd2.Ring[i]-det.TSd1rChannel.at(l))<=2){
					det.TSd2rEnergy.at(l)=sd2.dE[i];
					det.TSd2rChannel.at(l)=sd2.Ring[i];
					det.TSd2Theta.at(l)=sd2.fThetaRand[i];
					RingMatch=1;
					}
					int sd2Seg = sd2.Seg[i];
					if(sd2.Orientation != sd1.Orientation) {
						sd2Seg = 31 - sd2Seg;
					}
				if ((sd2Seg-det.TSd1sChannel.at(l))==-1 || (sd2Seg-det.TSd1sChannel.at(l))==0 || (sd2Seg-det.TSd1sChannel.at(l))==1 || (sd2Seg-det.TSd1sChannel.at(l))==31 || (sd2Seg-det.TSd1sChannel.at(l))==-31){
					det.TSd2sEnergy.at(l)=sd2.dE[i];
					det.TSd2sChannel.at(l)=sd2Seg;
					det.TSd2Phi.at(l)=sd2.fPhiRand[i];
					SectorMatch=1;
				}	
			}}
			if(RingMatch==0){
					det.TSd2rEnergy.push_back(sd2.dE[i]);
					det.TSd2rChannel.push_back(sd2.Ring[i]);
  					det.TSd2Theta.push_back(sd2.fThetaRand[i]);
			}
			if(SectorMatch==0){
					det.TSd2sEnergy.push_back(sd2.dE[i]);
					det.TSd2sChannel.push_back(sd2.Seg[i]);
  					det.TSd2Phi.push_back(sd2.fPhiRand[i]);
			}
		}}
	}
	

	if(yu.mul>0)
	{
  		det.TYuMul = yu.mul;
		for(size_t i=0; i<yd.dE.size(); i++){
			det.TYuEnergy.push_back(yu.dE[i]);
  			det.TYuTheta.push_back(yu.fThetaRand[i]);// Yu theta angle                                                                       
			det.TYuChannel.push_back(yu.Seg[i]*16+yu.Ring[i]);
			det.TYuNo.push_back(yu.Seg[i]);
			det.TYuRing.push_back(yu.Ring[i]);
		}

	if(yu.mul>1 && sortEnergies==1) {
	Bool_t have_swapped = true;
		while(have_swapped == true){
			for (size_t x=0; x<det.TYuEnergy.size(); x++){
				have_swapped = false;
				for(size_t y=0; y<det.TYuEnergy.size()-1; y++){
					if(det.TYuEnergy[y]<det.TYuEnergy[y+1]){
						std::swap(det.TYuEnergy[y],det.TYuEnergy[y+1]);
						std::swap(det.TYuTheta[y],det.TYuTheta[y+1]);
						std::swap(det.TYuPhi[y],det.TYuPhi[y+1]);
						std::swap(det.TYuChannel[y],det.TYuChannel[y+1]);
						std::swap(det.TYuNo[y],det.TYuNo[y+1]);
						std::swap(det.TYuRing[y],det.TYuRing[y+1]);
						have_swapped = true;
					}
				}
			}
		}
	}
	}

	
	if(su.mul>0)
	{
		det.TSurMul=su.mul;
		for(Int_t i=0; i<det.TSd1rMul; i++){
  			det.TSurEnergy.push_back(su.dE[i]);
			det.TSurChannel.push_back(su.Ring[i]);
  			det.TSuTheta.push_back(su.fThetaRand[i]);
		}
		det.TSusMul=su.mul;
		for(Int_t i=0; i<det.TSd1sMul; i++){
  			det.TSusEnergy.push_back(su.dE[i]);
			det.TSusChannel.push_back(su.Seg[i]);
  			det.TSuPhi.push_back(su.fPhiRand[i]);
		}
	if(su.mul>1){
	Bool_t have_swapped = true;
		while(have_swapped == true){
			for (size_t x=0; x<det.TSurEnergy.size(); x++){
				have_swapped = false;
				for(size_t y=0; y<det.TSurEnergy.size()-1; y++){
					if(det.TSurEnergy[y]<det.TSurEnergy[y+1]){
						std::swap(det.TSurEnergy[y],det.TSurEnergy[y+1]);
						std::swap(det.TSurChannel[y],det.TSurChannel[y+1]);
						std::swap(det.TSuTheta[y],det.TSuTheta[y+1]);
						std::swap(det.TSusEnergy[y],det.TSusEnergy[y+1]);
						std::swap(det.TSusChannel[y],det.TSusChannel[y+1]);
						std::swap(det.TSuPhi[y],det.TSuPhi[y+1]);
						have_swapped = true;
					}
				}
			}
		}
	}
	}
}
