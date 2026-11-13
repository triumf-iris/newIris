#ifndef YYDETECTOR_H
#define YYDETECTOR_H

#include "IrisDetector.h"
#include <vector>

class YYDetector : public IrisDetector<YYDetector>
{
    friend class IrisDetector<YYDetector>;
protected:
    inline static std::vector<DetMat> frontDeadLayer = {
        {{IrisMaterial::Al, 13. / 27.}, 0.1 * 2.702 * 0.1},
        {{IrisMaterial::B, 5. / 11.}, 0.5 * 2.3502 * 0.1}};

    inline static std::vector<DetMat> backDeadLayer = {
        {{IrisMaterial::P, 15. / 31.}, 0.1 * 1.8219 * 0.1},
        {{IrisMaterial::Al, 13. / 27.}, 0.1 * 2.702 * 0.1}};

    inline static DetMat mainLayer = {{IrisMaterial::Si, 14. / 28.}, 100 * 2.3212 * .1};
    inline static std::pair<IrisMaterial, double> mainMaterial = {IrisMaterial::Si, 14. / 28.};
};

#endif
