#include "header.h"
#include "eloss.h"
#include "geometry.h"
#include "IrisMaterial.h"

Bool_t detHits(PTrack tr, nucleus ncl, TVector3 reacPos, Bool_t maskIn, Bool_t shieldIn, Int_t P)
{
	Bool_t mask = maskClear(tr.T, tr.P) || !maskIn;
	Bool_t shield = shieldClear(tr.T, tr.P) || !shieldIn;
	Bool_t forward = (tr.T < TMath::Pi() / 2.);
	Bool_t backward = (tr.T > TMath::Pi() / 2.);
	Bool_t YYHit = 0;
	Bool_t CsIHit = 0;
	Bool_t Sd1Hit = 0;
	Bool_t Sd2Hit = 0;
	Bool_t YuHit = 0;
	Bool_t SuHit = 0;

	Double_t ETmp = tr.Ebt;
	Double_t ETmpU = tr.Ebt;

	//std::cout << "Ebt = " << ETmp << std::endl;

	if (mask && shield && backward)
	{
		YuHit = yu.Hit(tr.T, tr.P, geoP.YuDistance, reacPos, P);
		SuHit = su.Hit(tr.T, tr.P, geoP.SuDistance, reacPos, P);
		if (YuHit)
			ETmpU = yu.ELoss(ncl, ETmpU, tr.T);
		if (SuHit)
			ETmpU = su.ELoss(ncl, ETmpU, tr.T);
	}

	if (mask && shield && forward)
	{
		YYHit = yd.Hit(tr.T, tr.P, geoP.YdDistance, reacPos, P);
		if (YYHit)
			CsIHit = csi.Hit(tr.T, tr.P, geoP.YdDistance + 11.6, reacPos, P);
		Sd1Hit = sd1.Hit(tr.T, tr.P, geoP.Sd1Distance, reacPos, P);
		if (Sd1Hit)
			Sd2Hit = sd2.Hit(tr.T, tr.P, geoP.Sd1Distance + 14.8, reacPos, P);
		if (YYHit)
			ETmp = yd.ELoss(ncl, ETmp, tr.T);
		if (CsIHit)
			ETmp = csi.ELoss(ncl, ETmp, tr.T);
		if (Sd1Hit)
			ETmp = sd1.ELoss(ncl, ETmp, tr.T);
		if (Sd2Hit)
			ETmp = sd2.ELoss(ncl, ETmp, tr.T);
	}

	return (mask && shield && YYHit && CsIHit);
}

// Calculate the energy loss of the scattered particles in Foil and SHT
PTrack TgtELoss(PTrack tr, nucleus ncl, geometry g, Double_t reacZ, Bool_t isSHTReac)
{
	//std::cout << "tr.E = " << tr.E << std::endl;
	if (isSHTReac)
	{ // Reaction in SHT
		if (g.TargetOrientation == 0 && tr.T < TMath::Pi() / 2.)
		{ // foil before target, theta<90 deg
			tr.FoildE = 0.;
			tr.TrgtdE = eloss(ncl, 1. / g.AoZTgt, tr.E, (g.TargetThickness - reacZ) / TMath::Cos(tr.T), IrisMaterial::Target);
		}
		if (g.TargetOrientation == 0 && tr.T > TMath::Pi() / 2.)
		{ // foil before target, theta>90 deg
			tr.TrgtdE = eloss(ncl, 1. / g.AoZTgt, tr.E, reacZ / TMath::Cos(TMath::Pi() - tr.T), IrisMaterial::Target);
			tr.FoildE = eloss(ncl, 1. / g.AoZFoil, tr.E - tr.TrgtdE, g.FoilThickness / TMath::Cos(TMath::Pi() - tr.T), IrisMaterial::Foil);
		}
		if (g.TargetOrientation == 1 && tr.T < TMath::Pi() / 2.)
		{ // foil after target, theta<90 deg
			tr.TrgtdE = eloss(ncl, 1. / g.AoZTgt, tr.E, (g.TargetThickness - reacZ) / TMath::Cos(tr.T), IrisMaterial::Target);
			tr.FoildE = eloss(ncl, 1. / g.AoZFoil, tr.E - tr.TrgtdE, g.FoilThickness / TMath::Cos(tr.T), IrisMaterial::Foil);
		}
		if (g.TargetOrientation == 1 && tr.T > TMath::Pi() / 2.)
		{ // foil after target, theta>90 deg
			tr.TrgtdE = eloss(ncl, 1. / g.AoZTgt, tr.E, reacZ / TMath::Cos(TMath::Pi() - tr.T), IrisMaterial::Target);
			tr.FoildE = 0.;
		}
	}
	else
	{ // Reaction in foil

		if (tr.T < TMath::Pi() / 2.)
		{
			tr.TrgtdE = 0.;
			tr.FoildE = eloss(ncl, 1. / g.AoZFoil, tr.E, (g.FoilThickness - reacZ) / TMath::Cos(tr.T), IrisMaterial::Foil);
		}
		if (tr.T > TMath::Pi() / 2.)
		{ // foil after target, theta>90 deg
			tr.FoildE = eloss(ncl, 1. / g.AoZFoil, tr.E, reacZ / TMath::Cos(TMath::Pi() - tr.T), IrisMaterial::Foil);
			tr.TrgtdE = 0;
		}
	}

	//std::cout << "tr.Ebt: " << tr.Ebt << "; tr.FoildE: " << tr.FoildE <<"; tr.TrgtdE: " << tr.TrgtdE << std::endl;

	tr.Ebt = tr.E - tr.FoildE - tr.TrgtdE; // calculate energy of particle after foil and target
	// printf("In: %f\tFoil: %f\tTarget: %f\tLeft: %f\n",tr.E,tr.FoildE,tr.TrgtdE,tr.Ebt);
	return tr;
}

void setIDet(Double_t ICdE, Double_t SSBdE, Bool_t sortEnergies)
{
	if (yd.GetMul() > 0)
	{
		det.TYdMul = yd.GetMul();
		for (int i = 0; i < yd.GetSize(); i++)
		{ // yd.dE.size() used instead of Mul to consider Mul=2 and Yd.size() = 3
			det.TYdEnergy.push_back(yd.GetdE(i));
			det.TYdTheta.push_back(yd.GetThetaRand(i)); // Yd theta angle
			det.TYdPhi.push_back(yd.GetPhiRand(i));		// Yd theta angle
			det.TYdChannel.push_back(yd.GetChannel(i));
			det.TYdNo.push_back(yd.GetNo(i));
			det.TYdRing.push_back(yd.GetRing(i));
		}

		if (sortEnergies == 1)
		{
			Bool_t have_swapped = true;
			while (have_swapped == true)
			{
				have_swapped = false;
				for (size_t y = 0; y < det.TYdEnergy.size() - 1; y++)
				{
					if (det.TYdEnergy[y] < det.TYdEnergy[y + 1])
					{
						std::swap(det.TYdEnergy[y], det.TYdEnergy[y + 1]);
						std::swap(det.TYdTheta[y], det.TYdTheta[y + 1]);
						std::swap(det.TYdPhi[y], det.TYdPhi[y + 1]);
						std::swap(det.TYdChannel[y], det.TYdChannel[y + 1]);
						std::swap(det.TYdNo[y], det.TYdNo[y + 1]);
						std::swap(det.TYdRing[y], det.TYdRing[y + 1]);
						have_swapped = true;
					}
				}
			}
		}
	}

	int ydSize = yd.GetSize();

	det.TCsI1Energy.resize(ydSize, NAN);
	det.TCsI1Channel.resize(ydSize, -1);
	det.TCsI1Phi.resize(ydSize, NAN);
	det.TCsI2Energy.resize(ydSize, NAN);
	det.TCsI2Channel.resize(ydSize, -1);
	det.TCsI2Phi.resize(ydSize, NAN);
	if (csi.GetMul() > 0 && det.TYdMul > 0)
	{
		for (int i = 0; i < csi.GetSize(); i++)
		{
			for (size_t l = 0; l < det.TYdEnergy.size(); l++)
			{
				if (((csi.GetSeg(i) / 2) - det.TYdNo.at(l)) == 0)
				{
					if (csi.GetdE(i) > 0 && det.TYdEnergy.at(l) > 0)
					{
						det.TCsI1Mul++;
						det.TCsI2Mul++;
						det.TCsI1Energy.at(l) = csi.GetdE(i);
						det.TCsI2Energy.at(l) = csi.GetdE(i);
						det.TCsI1Channel.at(l) = csi.GetSeg(i);
						det.TCsI2Channel.at(l) = csi.GetSeg(i);
						det.TCsI1Phi.at(l) = csi.GetPhiRand(i);
						det.TCsI2Phi.at(l) = csi.GetPhiRand(i);
						det.TYdPhi.at(l) = csi.GetPhiRand(i);
					}
				}
			}
		}
	}

	det.TSSBEnergy = SSBdE;
	det.TICEnergy.push_back(ICdE);
	det.TICChannel.push_back(15);

	if (sd1.GetMul() > 0)
	{
		det.TSd1rMul = sd1.GetMul();
		for (Int_t i = 0; i < det.TSd1rMul; i++)
		{
			det.TSd1rEnergy.push_back(sd1.GetdE(i));
			det.TSd1rChannel.push_back(sd1.GetRing(i));
			det.TSd1Theta.push_back(sd1.GetThetaRand(i));
		}
		det.TSd1sMul = sd1.GetMul();
		for (Int_t i = 0; i < det.TSd1sMul; i++)
		{
			det.TSd1sEnergy.push_back(sd1.GetdE(i));
			det.TSd1sChannel.push_back(sd1.GetSeg(i));
			det.TSd1Phi.push_back(sd1.GetPhiRand(i));
		}
		if (sortEnergies == 1)
		{
			Bool_t have_swapped = true;
			while (have_swapped == true)
			{
				have_swapped = false;
				for (size_t y = 0; y < det.TSd1rEnergy.size() - 1; y++)
				{
					if (det.TSd1rEnergy[y] < det.TSd1rEnergy[y + 1])
					{
						std::swap(det.TSd1rEnergy[y], det.TSd1rEnergy[y + 1]);
						std::swap(det.TSd1rChannel[y], det.TSd1rChannel[y + 1]);
						std::swap(det.TSd1Theta[y], det.TSd1Theta[y + 1]);
						std::swap(det.TSd1sEnergy[y], det.TSd1sEnergy[y + 1]);
						std::swap(det.TSd1sChannel[y], det.TSd1sChannel[y + 1]);
						std::swap(det.TSd1Phi[y], det.TSd1Phi[y + 1]);
						have_swapped = true;
					}
				}
			}
		}
	}

	Int_t sd1Size = sd1.GetSize();

	det.TSd2rEnergy.resize(sd1Size, NAN);
	det.TSd2rChannel.resize(sd1Size, -1);
	det.TSd2Theta.resize(sd1Size, NAN);
	det.TSd2sEnergy.resize(sd1Size, NAN);
	det.TSd2sChannel.resize(sd1Size, -1);
	det.TSd2Phi.resize(sd1Size, NAN);
	if (det.TSd1rMul > 0 && sd2.GetMul() > 0)
	{
		det.TSd2rMul = sd2.GetSize();
		for (int i = 0; i < sd2.GetSize(); i++)
		{
			if (sd2.GetdE(i) > 0)
			{
				bool RingMatch = 0, SectorMatch = 0;
				for (size_t l = 0; l < det.TSd1rEnergy.size(); l++)
				{
					if (det.TSd1rEnergy.at(l) > 0)
					{
						if ((sd2.GetRing(i) - det.TSd1rChannel.at(l)) >= 0 && (sd2.GetRing(i) - det.TSd1rChannel.at(l)) <= 2)
						{
							det.TSd2rEnergy.at(l) = sd2.GetdE(i);
							det.TSd2rChannel.at(l) = sd2.GetRing(i);
							det.TSd2Theta.at(l) = sd2.GetThetaRand(i);
							RingMatch = 1;
						}
						int sd2Seg = sd2.GetSeg(i);
						if (sd2.GetOrientation() != sd1.GetOrientation())
						{
							sd2Seg = 31 - sd2Seg;
						}
						if ((sd2Seg - det.TSd1sChannel.at(l)) == -1 || (sd2Seg - det.TSd1sChannel.at(l)) == 0 || (sd2Seg - det.TSd1sChannel.at(l)) == 1 || (sd2Seg - det.TSd1sChannel.at(l)) == 31 || (sd2Seg - det.TSd1sChannel.at(l)) == -31)
						{
							det.TSd2sEnergy.at(l) = sd2.GetdE(i);
							det.TSd2sChannel.at(l) = sd2Seg;
							det.TSd2Phi.at(l) = sd2.GetPhiRand(i);
							SectorMatch = 1;
						}
					}
				}
				if (RingMatch == 0)
				{
					det.TSd2rEnergy.push_back(sd2.GetdE(i));
					det.TSd2rChannel.push_back(sd2.GetRing(i));
					det.TSd2Theta.push_back(sd2.GetThetaRand(i));
				}
				if (SectorMatch == 0)
				{
					det.TSd2sEnergy.push_back(sd2.GetdE(i));
					det.TSd2sChannel.push_back(sd2.GetSeg(i));
					det.TSd2Phi.push_back(sd2.GetPhiRand(i));
				}
			}
		}
	}

	if (yu.GetMul() > 0)
	{
		det.TYuMul = yu.GetMul();
		for (int i = 0; i < yd.GetSize(); i++)
		{
			det.TYuEnergy.push_back(yu.GetdE(i));
			det.TYuTheta.push_back(yu.GetThetaRand(i)); // Yu theta angle
			det.TYuChannel.push_back(yu.GetChannel(i));
			det.TYuNo.push_back(yu.GetNo(i));
			det.TYuRing.push_back(yu.GetRing(i));
		}

		if (yu.GetMul() > 1 && sortEnergies == 1)
		{
			Bool_t have_swapped = true;
			while (have_swapped == true)
			{
				have_swapped = false;
				for (size_t y = 0; y < det.TYuEnergy.size() - 1; y++)
				{
					if (det.TYuEnergy[y] < det.TYuEnergy[y + 1])
					{
						std::swap(det.TYuEnergy[y], det.TYuEnergy[y + 1]);
						std::swap(det.TYuTheta[y], det.TYuTheta[y + 1]);
						std::swap(det.TYuPhi[y], det.TYuPhi[y + 1]);
						std::swap(det.TYuChannel[y], det.TYuChannel[y + 1]);
						std::swap(det.TYuNo[y], det.TYuNo[y + 1]);
						std::swap(det.TYuRing[y], det.TYuRing[y + 1]);
						have_swapped = true;
					}
				}
			}
		}
	}

	if (su.GetMul() > 0)
	{
		det.TSurMul = su.GetMul();
		for (Int_t i = 0; i < det.TSd1rMul; i++)
		{
			det.TSurEnergy.push_back(su.GetdE(i));
			det.TSurChannel.push_back(su.GetRing(i));
			det.TSuTheta.push_back(su.GetThetaRand(i));
		}
		det.TSusMul = su.GetMul();
		for (Int_t i = 0; i < det.TSd1sMul; i++)
		{
			det.TSusEnergy.push_back(su.GetdE(i));
			det.TSusChannel.push_back(su.GetSeg(i));
			det.TSuPhi.push_back(su.GetPhiRand(i));
		}
		if (su.GetMul() > 1)
		{
			Bool_t have_swapped = true;
			while (have_swapped == true)
			{
				have_swapped = false;
				for (size_t y = 0; y < det.TSurEnergy.size() - 1; y++)
				{
					if (det.TSurEnergy[y] < det.TSurEnergy[y + 1])
					{
						std::swap(det.TSurEnergy[y], det.TSurEnergy[y + 1]);
						std::swap(det.TSurChannel[y], det.TSurChannel[y + 1]);
						std::swap(det.TSuTheta[y], det.TSuTheta[y + 1]);
						std::swap(det.TSusEnergy[y], det.TSusEnergy[y + 1]);
						std::swap(det.TSusChannel[y], det.TSusChannel[y + 1]);
						std::swap(det.TSuPhi[y], det.TSuPhi[y + 1]);
						have_swapped = true;
					}
				}
			}
		}
	}
}
