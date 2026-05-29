#include <stdlib.h>
#include <fstream>
#include "TFile.h"
#include "TTree.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TLorentzVector.h"
#include "TRandom3.h"
#include "TGenPhaseSpace.h"
#include "TStopwatch.h"
#include "eloss.h"

#include "header.h"
#include "IPhys.h"

void QvalueCalculate(nucleus ncl, Double_t E_center_Tgt, Double_t mc, Double_t mA, Double_t ma, Int_t P)
{
	for (int i = 0; i < yd.GetSize(); i++)
	{
		if (yd.GetPType(i) == P && yd.GetdE(i) > 0)
		{
			// LEHitcntr++;
			Double_t cosTheta = TMath::Cos(yd.GetThetaCalc(i) * TMath::DegToRad());
			Double_t Eb = 0;
			if (csi.GetdE(i) > 0.001)
			{
				Eb = csi.GetdE(i);
				Eb = Eb + elossFi(ncl, Eb, 0.1 * 1.4 * 6. / cosTheta, IrisMaterial::Mylar); // Mylar
				Eb = Eb + elossFi(ncl, Eb, 0.1 * 2.702 * 0.3 / cosTheta, IrisMaterial::Al); // 0.3 u Al
				Eb = Eb + elossFi(ncl, Eb, 0.1 * 1.822 * 0.1 / cosTheta, IrisMaterial::P);
			} // 0.1Phosphorus
			Eb += yd.GetdE(i);																	// use measured Yd // change june28
			Eb = Eb + elossFi(ncl, Eb, 0.1 * 2.3502 * 0.05 / cosTheta, IrisMaterial::B); // 0.05 u B
			Eb = Eb + elossFi(ncl, Eb, 0.1 * 2.702 * 0.1 / cosTheta, IrisMaterial::Al);	// 0.1 u Al
			if (geoPrm.Orientation == 1)
			{
				Eb = Eb + elossFi(ncl, Eb, geoPrm.TFoil / cosTheta, IrisMaterial::Foil);
			}
			Eb = Eb + elossFi(ncl, Eb, geoPrm.TTgt / 2. / cosTheta, IrisMaterial::Target); // deuteron energy  in mid target midtarget
			phys.YdCsIETot[P] = Eb;
			Eb = Eb / 1000.;
			Double_t E_center = E_center_Tgt / 1000.;
			Double_t Pb = sqrt(Eb * Eb + 2. * Eb * mc);
			Double_t P_center = sqrt(E_center * E_center + 2 * E_center * mA);
			// EB_det = EA+mA+ma-Eb-mc;
			Double_t EB = E_center + mA + ma - Eb - mc;
			phys.Eb_ydcsi[P] = EB;
			Double_t PB = sqrt(P_center * P_center + Pb * Pb - 2. * P_center * Pb * cosTheta);
			phys.PB_ydcsi[P] = PB;
			Double_t Q = mA + ma - mc - sqrt(EB * EB - PB * PB);
			phys.Q_ydcsi[P] = Q * 1000.;
		}
	}

	for (int i = 0; i < sd1.GetSize(); i++)
	{
		if (sd1.GetPType(i) == P && sd1.GetdE(i) > 0)
		{
			Double_t Eb = 0;
			Double_t cosTheta = TMath::Cos(sd1.GetThetaCalc(i) * TMath::DegToRad());
			// Sd2 ring side
			if (sd2.GetdE(i) > 0.001)
			{
				Double_t Eb = sd2.GetdE(i);
				Eb = Eb + elossFi(ncl, Eb, 0.1 * 1.822 * 0.5 / cosTheta, IrisMaterial::P);
				Eb = Eb + elossFi(ncl, Eb, 0.1 * 2.7 * 0.3 / cosTheta, IrisMaterial::Al);
				Eb = Eb + elossFi(ncl, Eb, 0.1 * 2.7 * 0.3 / cosTheta, IrisMaterial::Al);
				Eb = Eb + elossFi(ncl, Eb, 0.1 * 1.822 * 0.5 / cosTheta, IrisMaterial::P);
			}
			Eb += sd1.GetdE(i);																	// use measured Sd // change june2
			Eb = Eb + elossFi(ncl, Eb, 0.1 * 2.35 * 0.5 / cosTheta, IrisMaterial::B);	// boron junction implant
			Eb = Eb + elossFi(ncl, Eb, 0.1 * 2.7 * 0.3 / cosTheta, IrisMaterial::Al);	// first metal
			Eb = Eb + elossFi(ncl, Eb, 0.1 * 2.65 * 2.5 / cosTheta, IrisMaterial::SiO2); // SiO2
			Eb = Eb + elossFi(ncl, Eb, 0.1 * 2.7 * 1.5 / cosTheta, IrisMaterial::Al);	// second metal
			if (geoPrm.Orientation == 1)
			{
				Eb = Eb + elossFi(ncl, Eb, geoPrm.TFoil / cosTheta, IrisMaterial::Foil);
			}
			Eb = Eb + elossFi(ncl, Eb, geoPrm.TTgt / 2. / cosTheta, IrisMaterial::Target); // deuteron energy  in mid target midtarget
			phys.S3Tot[P] = Eb;
			Eb = Eb / 1000.;
			Double_t E_center = E_center_Tgt / 1000.;
			Double_t Pb = sqrt(Eb * Eb + 2. * Eb * mc);
			Double_t P_center = sqrt(E_center * E_center + 2 * E_center * mA);
			Double_t EB = E_center + mA + ma - Eb - mc;
			phys.EB_sd[P] = EB;
			Double_t PB = sqrt(P_center * P_center + Pb * Pb - 2. * P_center * Pb * cosTheta);
			phys.PB_sd[P] = PB;
			Double_t Q = mA + ma - mc - sqrt(EB * EB - PB * PB);
			phys.Q_sd[P] = Q * 1000.;
		}
	}

	/*if(yu.dE[1]>0){

		Double_t Eb = yu.dE[1];
		Double_t cosTheta = TMath::Cos(yu.fThetaCalc[1]*TMath::DegToRad());
		Eb= Eb+elossFi(Eb,0.1*2.3502*0.05/cosTheta,ncl.EL.eB,ncl.EL.dedxB); //0.05 u B
		Eb= Eb+elossFi(Eb,0.1*2.702*0.1/cosTheta,ncl.EL.eAl,ncl.EL.dedxAl); //0.1 u Al
		if(geoPrm.Orientation==0){ Eb=Eb+elossFi(Eb,geoPrm.TFoil/cosTheta,ncl.EL.eFoil,ncl.EL.dedxFoil);}
		Eb= Eb+elossFi(Eb,geoPrm.TTgt/2./cosTheta,ncl.EL.eTgt,ncl.EL.dedxTgt);
		phys.YuTot=Eb;
		Eb= Eb/1000.;
		Double_t E_center = E_center_Tgt/1000.;
		Double_t Pb = sqrt(Eb*Eb+2.*Eb*mc);
		Double_t P_center = sqrt(E_center*E_center+2*E_center*mA);
		//EB_det = EA+mA+ma-Eb-mc;
		Double_t EB = E_center+mA+ma-Eb-mc;
		phys.EB_yu = EB;
		Double_t PB = sqrt(P_center*P_center+Pb*Pb-2.*P_center*Pb*cosTheta);
		phys.PB_yu = PB;
		Double_t Q = mA+ma-mc-sqrt(EB*EB-PB*PB);
		phys.Qyu = Q*1000.;

	}
	if(su.dE[1]>0){

		Double_t Eb = su.dE[1];
		Double_t cosTheta = TMath::Cos(yu.fThetaCalc[1]*TMath::DegToRad());
		Eb = Eb+elossFi(Eb,0.1*2.35*0.5/cosTheta,ncl.EL.eB,ncl.EL.dedxB); //boron junction implant
		Eb = Eb+elossFi(Eb,0.1*2.7*0.3/cosTheta,ncl.EL.eAl,ncl.EL.dedxAl); //first metal
		Eb = Eb+elossFi(Eb,0.1*2.65*2.5/cosTheta,ncl.EL.eSiO2,ncl.EL.dedxSiO2); //SiO2
		Eb = Eb+elossFi(Eb,0.1*2.7*1.5/cosTheta,ncl.EL.eAl,ncl.EL.dedxAl); //second metal
		if(geoPrm.Orientation==0){ Eb=Eb+elossFi(Eb,geoPrm.TFoil/cosTheta,ncl.EL.eFoil,ncl.EL.dedxFoil);}
		Eb= Eb+elossFi(Eb,geoPrm.TTgt/2./cosTheta,ncl.EL.eTgt,ncl.EL.dedxTgt);
		phys.SuTot=Eb;
		Eb= Eb/1000.;
		Double_t E_center = E_center_Tgt/1000.;
		Double_t Pb = sqrt(Eb*Eb+2.*Eb*mc);
		Double_t P_center = sqrt(E_center*E_center+2*E_center*mA);
		//EB_det = EA+mA+ma-Eb-mc;
		Double_t EB = E_center+mA+ma-Eb-mc;
		phys.EB_su = EB;
		Double_t PB = sqrt(P_center*P_center+Pb*Pb-2.*P_center*Pb*cosTheta);
		phys.PB_su = PB;
		Double_t Q = mA+ma-mc-sqrt(EB*EB-PB*PB);
		phys.Qsu = Q*1000.;

	}*/
}
