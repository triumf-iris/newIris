#ifndef CSIDETECTOR_H
#define CSIDETECTOR_H

#include "IrisDetector.h"
#include <vector>

class CsIDetector : public IrisDetector<CsIDetector>
{
    friend class IrisDetector<CsIDetector>;
protected:
    inline static std::vector<DetMat> frontDeadLayer = {
        {{IrisMaterial::P, 15. / 31.}, 0.1 * 1.8219 * 0.1},
        {{IrisMaterial::Al, 13. / 27.}, 0.3 * 2.702 * 0.1},
        {{IrisMaterial::Mylar, 100. / 192.}, 6. * 1.4 * 0.1}};

    inline static std::vector<DetMat> backDeadLayer = {};

    inline static DetMat mainLayer = {{IrisMaterial::CsI, 108. / 260.}, 12000. * 4.51 * 0.1};
    inline static std::pair<IrisMaterial, double> mainMaterial = {IrisMaterial::CsI, 108. / 260.};
};

#endif
