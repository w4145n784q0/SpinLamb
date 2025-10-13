#pragma once
#include"CharacterParams.h"
#include"../../Engine/GameObject.h"
class CharacterFence :
    public GameObject
{    
public:
    //----------柵関係----------

    /// <summary>
    /// 各方向の柵から法線ベクトルを取得しNormalArrayを初期化
    /// </summary>
    void GetWireNormal();

    /// <summary>
    /// 柵に接触した際の計算処理　柵の法線で計算
    /// </summary>
    /// <param name="_normal">反射される方向(接触した柵の法線ベクトル)</param>
    void FenceReflect(XMVECTOR _normal);

    /// <summary>
    /// 監視する対象(配列)に柵にヒットしたことを通知
    /// </summary>
    void NotifyFenceHit();

    /// <summary>
    /// ダメージ後の無敵時間の計算
    /// </summary>
    void InvincibilityTimeCalculation();
};

