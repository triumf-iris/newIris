// Eloss.h
#ifndef EneLoss_H
#define EneLoss_H

#include <TMath.h>
#include <TRandom3.h>
#include <TGraph.h>
#include "nucleus.h"

Double_t eval(Double_t, Double_t[100], Double_t[100]);
Double_t eloss(nucleus, Double_t, Double_t, Double_t, Double_t[100], Double_t[100]);
Double_t elossFi(Double_t, Double_t, Double_t[100], Double_t[100]);

void loadELoss(std::string, Double_t[100], Double_t[100], Double_t);
//Double_t eval(Double_t, Double_t[100], Double_t[100]);
Double_t eloss(Double_t, Double_t, TGraph *);
Double_t eloss(Double_t, Double_t, Double_t[100], Double_t[100]);
// Double_t simEloss(nucleus, Double_t, Double_t, Double_t, Double_t[100], Double_t[100]);
Double_t erem(Double_t, TGraph *);
Double_t elossFi(Double_t, Double_t, TGraph *);
//Double_t elossFi(Double_t, Double_t, Double_t[100], Double_t[100]);
Double_t thickness(Double_t, Double_t, TGraph *);//initial energy and final energy are given as arguments, calculates target thickness 
Double_t thickness(Double_t, Double_t, Double_t[100], Double_t[100]);

Double_t fncBethe(Double_t *x, Double_t *par); //Bethe like function for fitting    

double eval(double, std::array<double, 100>, std::array<double, 100>);
double eloss(nucleus, double, double, double, IrisMaterial);
double eloss(nucleus, double, double, double, std::array<double, 100>, std::array<double, 100>);
double elossFi(nucleus, double, double, IrisMaterial);
double elossFi(double, double, std::array<double, 100>, std::array<double, 100>);
#endif
// end
