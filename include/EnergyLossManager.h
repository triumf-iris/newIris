#ifndef ENERGYLOSSMANAGER_H
#define ENERGYLOSSMANAGER_H

#include <TMath.h>
#include <TRandom3.h>
#include "nucleus.h"
#include "IrisMaterial.h"

class EnergyLossManager
{
public:
    EnergyLossManager();
    double eval(double, std::array<double, 100>, std::array<double, 100>);
    double eloss(nucleus, double, double, double, IrisMaterial);
    double eloss_Lise(nucleus, double, double, double, std::array<double, 100>, std::array<double, 100>);
    double elossFi(nucleus, double, double, IrisMaterial);
    double elossFi_Lise(double, double, std::array<double, 100>, std::array<double, 100>);
};

#endif
