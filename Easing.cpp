#include "Easing.h"
#include"Engine/SceneManager.h"
#include"Engine/CsvReader.h"

namespace
{
    enum CommonEasingIndex
    {
        i_adjust = 0,
        i_squared,
        i_cubed,
    };

    enum easeOutBackIndex
    {
        i_overshoot = 0,
    };

    double Adjust = 0.0;//イージングの補正値(基本1.0)
    int Squared = 0;//2乗する際の指数 pow()に使用
    int Cubed = 0;//3乗する際の指数 pow()に使用

    struct EaseOutBack
    {
        double OverShoot = 0.0;//イージング曲線の跳ね返り具合
    };
    EaseOutBack EaseOutBackParam;
}

double Easing::easeOutBack(double x)
{
    const double c1 = EaseOutBackParam.OverShoot;
    const double c3 = c1 + Adjust;

    return Adjust + c3 * pow(x - Adjust, Cubed) + c1 * pow(x - Adjust, Squared);
}

double Easing::easeOutCubic(double x)
{
   return 1.0 - pow(Adjust - x, Cubed);
}

double Easing::calculateScale(float _MaxScale, float _EasingCount)
{
    //tmp = 1 + (目標スケール - 1) * easeOutBack(x);
    double tmp = Adjust + (_MaxScale - Adjust) * Easing::easeOutBack(_EasingCount);
    return tmp;
}

void Easing::SetSCVEasing()
{
    CsvReader csveasing;
    csveasing.Load("CSVdata\\EasingData.csv");

    std::string ease = "CommonEasing";
    if (csveasing.IsGetParamName(ease))
    {
        std::vector<float> v = csveasing.GetParam(ease);
        Adjust = static_cast<int>(v[i_adjust]);
        Squared = static_cast<int>(v[i_squared]);
        Cubed = static_cast<int>(v[i_cubed]);
    }

    std::string easeout = "EaseOutBackParam";
    if (csveasing.IsGetParamName(easeout))
    {
        std::vector<float> v = csveasing.GetParam(easeout);
        EaseOutBackParam.OverShoot = v[i_overshoot];
    }
}
