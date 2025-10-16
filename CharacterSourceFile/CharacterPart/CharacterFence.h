#pragma once
#include"CharacterParams.h"
#include"../../Engine/GameObject.h"
#include "../../InterfaceSourceFile/IVFXEventListener.h"
#include"../../InterfaceSourceFile/IChargeEventListner.h"
#include"../../InterfaceSourceFile/IMovementEventListener.h"

class CharacterFence :
    public GameObject
{    
private:

    IVFXEventListener* VFXListener_;
    IChargeEventListener* ChargeListener_;
    IMovementEventListener* MovementListener_;
public:

    //----------柵関係----------
    CharacterFence(GameObject* parent);
    virtual ~CharacterFence() = default;

    void Initialize() override {};
    void Update() override {};
    void Draw() override {};
    void Release() override {};

    // リスナーを後から設定する
    void SetEventListener(IVFXEventListener* _VfxListener, 
        IChargeEventListener* _ChargeListener, IMovementEventListener* _MovementListener);

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

