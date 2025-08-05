#include "Easing.h"
#include <DirectXMath.h>

using namespace DirectX;

namespace Easing
{

    double Easing::EaseInSine(double x)
    {
        return 1 - cos(((x) * XM_PI) / 2.0f);
    }

    double Easing::EaseOutSine(double x)
    {
        return  sin((x * XM_PI) / 2);
    }

    double Easing::EaseInOutSine(double x)
    {
        return  -(cos(XM_PI * x) - 1) / 2;
    }

    double Easing::EaseInQuad(double x)
    {
        return x * x;
    }

    double Easing::EaseOutQuad(double x)
    {
        return 1 - (1 - x) * (1 - x);
    }

    double Easing::EaseInOutQuad(double x)
    {
        return x < 0.5 ? 2 * x * x : 1 - pow(-2 * x + 2, 2) / 2;
    }

    double Easing::EaseInCubic(double x)
    {
        return x * x * x;
    }

    double Easing::EaseOutCubic(double x)
    {
        return 1 - pow(1 - x, 3);
    }

    double Easing::EaseInOutCubic(double x)
    {
        return x < 0.5 ? 4 * x * x * x : 1 - pow(-2 * x + 2, 3) / 2;
    }

    double Easing::EaseInQuart(double x)
    {
        return x * x * x * x;
    }

    double Easing::EaseOutQuart(double x)
    {
        return 1 - pow(1 - x, 4);
    }

    double Easing::EaseInOutQuart(double x)
    {
        return x < 0.5 ? 8 * x * x * x * x : 1 - pow(-2 * x + 2, 4) / 2;
    }

    double Easing::EaseInQuint(double x)
    {
        return x * x * x * x * x;
    }

    double Easing::EaseOutQuint(double x)
    {
        return 1 - pow(1 - x, 5);
    }

    double Easing::EaseInOutQuint(double x)
    {
        return x < 0.5 ? 16 * x * x * x * x * x : 1 - pow(-2 * x + 2, 5) / 2;
    }

    double Easing::EaseInExpo(double x)
    {
        return x == 0 ? 0 : pow(2, 10 * x - 10);
    }

    double Easing::EaseOutExpo(double x)
    {
        return x == 1 ? 1 : 1 - pow(2, -10 * x);
    }

    double Easing::EaseInOutExpo(double x)
    {
        return x == 0
            ? 0
            : x == 1
            ? 1
            : x < 0.5 ? pow(2, 20 * x - 10) / 2
            : (2 - pow(2, -20 * x + 10)) / 2;
    }

    double Easing::EaseInCirc(double x)
    {
        return 1 - sqrt(1 - pow(x, 2));
    }

    double Easing::EaseOutCirc(double x)
    {
        return sqrt(1 - pow(x - 1, 2));
    }

    double Easing::EaseInOutCirc(double x)
    {
        return x < 0.5
            ? (1 - sqrt(1 - pow(2 * x, 2))) / 2
            : (sqrt(1 - pow(-2 * x + 2, 2)) + 1) / 2;
    }

    double Easing::EaseInBack(double x)
    {
        double c1 = 1.70158f;
        double c3 = c1 + 1;

        return c3 * x * x * x - c1 * x * x;
    }

    double Easing::EaseOutBack(double x)
    {
        double c1 = 1.70158;
        double c3 = c1 + 1;

        return 1 + c3 * pow(x - 1, 3) + c1 * pow(x - 1, 2);
    }

    double Easing::EaseInOutBack(double x)
    {
        double c1 = 1.70158;
        double c2 = c1 * 1.525;

        return x < 0.5
            ? (pow(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2
            : (pow(2 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2;
    }

    double Easing::EaseInElastic(double x)
    {
        double c4 = (2 * XM_PI) / 3;

        return x == 0
            ? 0
            : x == 1
            ? 1
            : -pow(2, 10 * x - 10) * sin((x * 10 - 10.75) * c4);
    }

    double Easing::EaseOutElastic(double x)
    {
        double c4 = (2 * XM_PI) / 3;

        return x == 0
            ? 0
            : x == 1
            ? 1
            : pow(2, -10 * x) * sin((x * 10 - 0.75) * c4) + 1;
    }

    double Easing::EaseInOutElastic(double x)
    {
        double c5 = (2 * XM_PI) / 4.5;

        return x == 0
            ? 0
            : x == 1
            ? 1
            : x < 0.5
            ? -(pow(2, 20 * x - 10) * sin((20 * x - 11.125) * c5)) / 2
            : (pow(2, -20 * x + 10) * sin((20 * x - 11.125) * c5)) / 2 + 1;

    }

    double Easing::EaseInBounce(double x)
    {
        return 1 - EaseOutBounce(1 - x);
    }

    double Easing::EaseOutBounce(double x)
    {
        double n1 = 7.5625;
        double d1 = 2.75;

        if (x < 1 / d1) {
            return n1 * x * x;
        }
        else if (x < 2 / d1) {
            return n1 * (x -= 1.5 / d1) * x + 0.75;
        }
        else if (x < 2.5 / d1) {
            return n1 * (x -= 2.25 / d1) * x + 0.9375;
        }
        else {
            return n1 * (x -= 2.625 / d1) * x + 0.984375;
        }
    }

    double Easing::EaseInOutBounce(double x)
    {
        return x < 0.5
            ? (1 - EaseOutBounce(1 - 2 * x)) / 2
            : (1 + EaseOutBounce(2 * x - 1)) / 2;
    }

    double Complement(double _Min, double _Max, double _Eased)
    {
        //値 = 下限値 + (上限値 - 下限値) * 割合
        return _Min + (_Max - _Min) * _Eased;
    }
}