#include "IrisDetector.h"
#include "YYDetector.h"
#include "CsIDetector.h"

template <class T>
double IrisDetector<T>::eLossFrontDL(nucleus nuc, double ein, double theta, int direct)
{
    return eLossDL(T::frontDeadLayer, nuc, ein, theta, direct);
}

template <class T>
double IrisDetector<T>::eAddFrontDL(nucleus nuc, double ein, double theta, int direct)
{
    return eAddDL(T::frontDeadLayer, nuc, ein, theta, direct);
}

template <class T>
double IrisDetector<T>::eLossBackDL(nucleus nuc, double ein, double theta, int direct)
{
    return eLossDL(T::backDeadLayer, nuc, ein, theta, direct);
}

template <class T>
double IrisDetector<T>::eAddBackDL(nucleus nuc, double ein, double theta, int direct)
{
    return eAddDL(T::backDeadLayer, nuc, ein, theta, direct);
}

template <class T>
double IrisDetector<T>::eLossMain(nucleus nuc, double ein, double theta, int direct)
{
    return eloss(nuc, T::mainLayer.first.second, ein, T::mainLayer.second / cos(theta), T::mainLayer.first.first);
}

template <class T>
double IrisDetector<T>::eLossMain(nucleus nuc, double ein, double thickness, double theta, int direct)
{
    return eloss(nuc, T::mainMaterial.second, ein, thickness / cos(theta), T::mainMaterial.first);
}

template <class T>
double IrisDetector<T>::eAddMain(nucleus nuc, double ein, double theta, int direct)
{
    return elossFi(nuc, ein, T::mainLayer.second / cos(theta), T::mainLayer.first.first);
}

template <class T>
double IrisDetector<T>::eAddMain(nucleus nuc, double ein, double thickness, double theta, int direct)
{
    return elossFi(nuc, ein, thickness / cos(theta), T::mainMaterial.first);
}

template <class T>
double IrisDetector<T>::eLossAll(nucleus nuc, double ein, double theta, int direct)
{
    double ecurr = ein;
    if (direct == 0)
    {
        ecurr -= eLossFrontDL(nuc, ecurr, theta, direct);
        ecurr -= eLossMain(nuc, ecurr, theta, direct);
        ecurr -= eLossBackDL(nuc, ecurr, theta, direct);
    }
    else
    {
        ecurr -= eLossBackDL(nuc, ecurr, theta, direct);
        ecurr -= eLossMain(nuc, ecurr, theta, direct);
        ecurr -= eLossFrontDL(nuc, ecurr, theta, direct);
    }
    return ein - ecurr;
}

template <class T>
double IrisDetector<T>::eLossAll(nucleus nuc, double ein, double thickness, double theta, int direct)
{
    double ecurr = ein;
    if (direct == 0)
    {
        ecurr -= eLossFrontDL(nuc, ecurr, theta, direct);
        ecurr -= eLossMain(nuc, ecurr, thickness, theta, direct);
        ecurr -= eLossBackDL(nuc, ecurr, theta, direct);
    }
    else
    {
        ecurr -= eLossBackDL(nuc, ecurr, theta, direct);
        ecurr -= eLossMain(nuc, ecurr, thickness, theta, direct);
        ecurr -= eLossFrontDL(nuc, ecurr, theta, direct);
    }
    return ein - ecurr;
}

template <class T>
double IrisDetector<T>::eAddAll(nucleus nuc, double ein, double theta, int direct)
{
    double ecurr = ein;
    if (direct == 0)
    {
        ecurr += eAddBackDL(nuc, ecurr, theta, direct);
        ecurr += eAddMain(nuc, ecurr, theta, direct);
        ecurr += eAddFrontDL(nuc, ecurr, theta, direct);
    }
    else
    {
        ecurr += eAddFrontDL(nuc, ecurr, theta, direct);
        ecurr += eAddMain(nuc, ecurr, theta, direct);
        ecurr += eAddBackDL(nuc, ecurr, theta, direct);
    }
    return ecurr;
}

template <class T>
double IrisDetector<T>::eAddAll(nucleus nuc, double ein, double thickness, double theta, int direct)
{
    double ecurr = ein;
    if (direct == 0)
    {
        ecurr += eAddBackDL(nuc, ecurr, theta, direct);
        ecurr += eAddMain(nuc, ecurr, thickness, theta, direct);
        ecurr += eAddFrontDL(nuc, ecurr, theta, direct);
    }
    else
    {
        ecurr += eAddFrontDL(nuc, ecurr, theta, direct);
        ecurr += eAddMain(nuc, ecurr, thickness, theta, direct);
        ecurr += eAddBackDL(nuc, ecurr, theta, direct);
    }
    return ecurr;
}

template <class T>
double IrisDetector<T>::eLossDL(std::vector<DetMat> layer, nucleus nuc, double ein, double theta, int direct)
{
    double ecurr = ein;
    if (direct == 0)
    {
        for (auto item : layer)
        {
            ecurr -= eloss(nuc, item.first.second, ecurr, item.second / cos(theta), item.first.first);
        }
    }
    else
    {
        for(auto it = layer.rbegin(); it != layer.rend(); it++)
        {
            auto item = *it;
            ecurr -= eloss(nuc, item.first.second, ecurr, item.second / cos(theta), item.first.first);
        }
    }
    return ein - ecurr;
}

template <class T>
double IrisDetector<T>::eAddDL(std::vector<DetMat> layer, nucleus nuc, double ein, double theta, int direct)
{
    double ecurr = ein;
    if (direct == 0)
    {
        for(auto it = layer.rbegin(); it != layer.rend(); it++)
        {
            auto item = *it;
            ecurr += elossFi(nuc, ecurr, item.second / cos(theta), item.first.first);
        }
    }
    else
    {
        for (auto item : layer)
        {
            ecurr += elossFi(nuc, ecurr, item.second / cos(theta), item.first.first);
        }
    }
    return ecurr;
}

template class IrisDetector<YYDetector>;
template class IrisDetector<CsIDetector>;