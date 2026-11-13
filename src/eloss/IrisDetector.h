#ifndef IRISDETECTOR_H
#define IRISDETECTOR_H

#include "eloss.h"
#include "IrisMaterial.h"
#include <vector>
#include <utility>

using DetMat = std::pair<std::pair<IrisMaterial, double>, double>;

template <class T>
class IrisDetector
{
public:
    static double eLossFrontDL(nucleus nuc, double ein, double theta, int direct); // direct = 0 for particle going front to back; direct = 1 for back to front
    static double eAddFrontDL(nucleus nuc, double ein, double theta, int direct);  // direct = 0 for particle going front to back; direct = 1 for back to front
    static double eLossBackDL(nucleus nuc, double ein, double theta, int direct);  // direct = 0 for particle going front to back; direct = 1 for back to front
    static double eAddBackDL(nucleus nuc, double ein, double theta, int direct);   // direct = 0 for particle going front to back; direct = 1 for back to front
    static double eLossMain(nucleus nuc, double ein, double theta, int direct);    // direct = 0 for particle going front to back; direct = 1 for back to front
    static double eLossMain(nucleus nuc, double ein, double thickness, double theta, int direct);    // direct = 0 for particle going front to back; direct = 1 for back to front
    static double eAddMain(nucleus nuc, double ein, double theta, int direct);     // direct = 0 for particle going front to back; direct = 1 for back to front
    static double eAddMain(nucleus nuc, double ein, double thickness, double theta, int direct);     // direct = 0 for particle going front to back; direct = 1 for back to front
    static double eLossAll(nucleus nuc, double ein, double theta, int direct);     // direct = 0 for particle going front to back; direct = 1 for back to front
    static double eLossAll(nucleus nuc, double ein, double thickness, double theta, int direct);     // direct = 0 for particle going front to back; direct = 1 for back to front
    static double eAddAll(nucleus nuc, double ein, double theta, int direct);      // direct = 0 for particle going front to back; direct = 1 for back to front
    static double eAddAll(nucleus nuc, double ein, double thickness, double theta, int direct);      // direct = 0 for particle going front to back; direct = 1 for back to front
    static double eLossDL(std::vector<DetMat> layer, nucleus nuc, double ein, double theta, int direct);
    static double eAddDL(std::vector<DetMat> layer, nucleus nuc, double ein, double theta, int direct);
};

#endif
