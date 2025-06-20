#include "Easing.h"
#include"Engine/SceneManager.h"

double Easing::easeOutBack(double x)
{
    const double c1 = 1.70158;
    const double c3 = c1 + 1;

    return 1 + c3 * pow(x - 1, 3) + c1 * pow(x - 1, 2);
}

double Easing::calculateScale(float _MaxScale, float _EasingCount)
{
    //tmp = 1 + (目標スケール - 1) * easeOutBack(x);
    double tmp = 1.0f + (_MaxScale - 1.0f) * Easing::easeOutBack(_EasingCount);
    return tmp;
}

void Easing::SetSCVEasing()
{

}
