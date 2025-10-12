#pragma once
#include"../../Engine/GameObject.h"
class CharacterCharge :
    public GameObject
{
public:
 
    //----------チャージ----------

    /// <summary>
    /// 加速度の加算
    /// </summary>
    void Charging();

    /// <summary>
    /// 蓄積したTmpAccele_を実際に加速度に代入
    /// </summary>
    void ChargeRelease();

    /// <summary>
    /// TmpAccele_を0にする
    /// </summary>
    void ChargeReset();

    /// <summary>
    /// チャージ中の矢印位置をセット
    /// </summary>
    void SetArrow();

    /// <summary>
    /// 矢印モデルの描画
    /// </summary>
    void DrawArrow();
};

