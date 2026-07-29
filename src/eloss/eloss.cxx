#include "eloss.h"

double eval(double in, std::array<double, 100> x, std::array<double, 100> y)
{
	double dxin = 0., dx = 0., dy = 0., e = 0.;
	if (in <= 0.)
	{
		e = 0.;
	}
	else if (in < x[0])
	{
		e = y[0] * in / x[0];
	}
	else if (in > x[99])
	{
		dxin = in - x[99];
		dx = x[99] - x[98];
		dy = y[99] - y[98];
		e = y[99] + dy * dxin / dx;
	}
	else
	{
		for (int i = 1; i < 100; i++)
		{
			if (in > x[i - 1] && in < x[i])
			{
				dxin = in - x[i - 1];
				dx = x[i] - x[i - 1];
				dy = y[i] - y[i - 1];
				e = y[i - 1] + dy * dxin / dx;
				break;
			}
		}
	}
	return e;
}

double eloss(nucleus P, double TZoverA, double ein, double th, IrisMaterial material) // initial energy and thickness are given as arguments
{
	return eloss(P, TZoverA, ein, th, P.EL.GetE(material), P.EL.GetDeDx(material));
}

// Make it a method for a particle class.
double eloss(nucleus P, double TZoverA, double ein, double th, std::array<double, 100> x, std::array<double, 100> y) // initial energy and thickness are given as arguments
{

	double k;
	double Bohr;
	double sgm;
	double strg;

	if (ein == 0.)
		return 0;
	if (th == 0.)
		return 0.;

	TRandom3 *rndm = new TRandom3(0);
	// Energy loss calculation including energy Straggling
	double dx = th / 100.; // in mg/cm2
	Bohr = TMath::Sqrt(0.157 * dx * P.Z * P.Z * TZoverA) / 1000.;
	double de = 0;	 // energy loss
	double en = ein; // the energy variable
	for (int i = 0; i < 100; i++)
	{
		de = (dx * eval(en, x, y)); // energy loss in dx
		if (de > en)
		{
			en = 0.;
			break;
		}
		k = 1.1 + 0.47 * TMath::Log10(en / double(P.A));
		sgm = k * Bohr;
		strg = rndm->Gaus(de, sgm);
		de = (strg > 0.) ? strg : 0.;
		if (de > en)
		{
			en = 0.;
			break;
		}
		en = en - de; // energy remaining after dx
	}
	rndm->Delete();
	return ein - en;
}

double elossFi(nucleus P, double efi, double th, IrisMaterial material) // final energy and thickness are given as arguments
{
	return elossFi(efi, th, P.EL.GetE(material), P.EL.GetDeDx(material));
}

double elossFi(double efi, double th, std::array<double, 100> x, std::array<double, 100> y) // final energy and thickness are given as arguments
{
	if (th == 0)
		return efi;
	// Energy loss calculation
	double dx = th / 100.; // in
	double de = 0;		   // energy loss
	double en = efi;	   // the energy variable
	double pos = 0.;	   // the position variable

	while (pos <= th)
	{
		de = (dx * eval(en, x, y)) / 2.; //
		en = en + de;
		de = (dx * eval(en, x, y)) / 2.; // energy loss in dx
		en = en + de;					 // energy remaining after dx
		pos = pos + dx;
	}
	return en - efi;
}

double thickness(nucleus P, double ein, double efi, IrisMaterial material)
{
	return thickness(ein, efi, P.EL.GetE(material), P.EL.GetDeDx(material));
}

double thickness(double ein, double efi, std::array<double, 100> x, std::array<double, 100> y) // initial energy and final energy are given as arguments, calculates target thickness
{

	// for (Int_t )
	if (ein <= efi)
		return 0.0;
	// Energy loss calculation
	Double_t dx = 0;
	Double_t de = (ein - efi) / 1000.; // energy loss step in MeV
	//  Double_t de = 0.01; //energy loss step in MeV
	Double_t en = ein; // the energy variable
	Double_t th = 0.;  // the thickness variable

	// Integrate numerically

	while (en > efi)
	{
		dx = de / eval(en, x, y) / 2.; //
		th = th + dx;
		en = en - de;

		dx = de / eval(en, x, y) / 2.; //
		th = th + dx;
	}
	return th;
}
