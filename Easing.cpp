#include "Easing.h"
#include"Engine/SceneManager.h"
#include"Engine/CsvReader.h"

namespace
{
    //イージング処理全般に使用する値を初期化する際のインデックス
    enum CommonEasingIndex
    {
        i_adjust = 0,
        i_squared,
        i_cubed,
    };

    //easeOutBackに使用する際のインデックス
    enum easeOutBackIndex
    {
        i_overshoot = 0,
    };

    //イージングの補正値(基本1.0)
    double Adjust = 0.0;

    //2乗する際の指数 pow()に使用
    int Squared = 0;

    //3乗する際の指数 pow()に使用
    int Cubed = 0;

    //EaseOutBackの演出用の構造体
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

    //1 + c3 *  (x - 1)^3 + c1 * (x - 1)^2;
    return Adjust + c3 * pow(x - Adjust, Cubed) + c1 * pow(x - Adjust, Squared);
}

double Easing::easeOutCubic(double x)
{
    //1 - (1 - x, 3)^3;
   return Adjust - pow(Adjust - x, Cubed);
}

double Easing::calculateScale(float _MaxScale, float _EasingCount)
{
    //tmp = 1 + (目標スケール - 1) * easeOutBack(x);
    double tmp = Adjust + (_MaxScale - Adjust) * Easing::easeOutBack(_EasingCount);
    return tmp;
}

void Easing::SetCSVEasing()
{
    //csvファイル読み込む
    CsvReader csveasing;
    csveasing.Load("CSVdata\\EffectData\\EasingData.csv");

    //csvファイルの各0列目の文字列を取得
    std::string easeInit= "CommonEasing";

    //0列目の文字列を渡し、その行のパラメータを取得
    std::vector<float> easeInitData = csveasing.GetParam(easeInit);

    //初期化の順番はcsvの各行の順番に合わせる
    //vの添え字はnamespaceで宣言した列挙型を使用
    Adjust = static_cast<double>(easeInitData[i_adjust]);
    Squared = static_cast<int>(easeInitData[i_squared]);
    Cubed = static_cast<int>(easeInitData[i_cubed]);

    //csvファイルの各0列目の文字列を取得
    std::string easeout = "EaseOutBackParam";

    //0列目の文字列を渡し、その行のパラメータを取得
    std::vector<float> easeoutData = csveasing.GetParam(easeout);

    //初期化の順番はcsvの各行の順番に合わせる
    //vの添え字はnamespaceで宣言した列挙型を使用
    EaseOutBackParam.OverShoot = easeoutData[i_overshoot];
}
