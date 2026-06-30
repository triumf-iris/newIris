// Eloss.h
#ifndef EneLoss_H
#define EneLoss_H

#include <TMath.h>
#include <TRandom3.h>
#include <TGraph.h>
#include "nucleus.h"

double eval(double, std::array<double, 100>, std::array<double, 100>);
double eloss(nucleus, double, double, double, IrisMaterial);
double eloss(nucleus, double, double, double, std::array<double, 100>, std::array<double, 100>);
double elossFi(nucleus, double, double, IrisMaterial);
double elossFi(double, double, std::array<double, 100>, std::array<double, 100>);
#endif
// end
