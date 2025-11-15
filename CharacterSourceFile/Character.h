#pragma once
#include "../Engine/GameObject.h"
#include <memory>

//部品分けしたヘッダファイル
#include "CharacterPart/CharacterParams.h"
#include "CharacterPart/CharacterModelBlink.h"
#include "CharacterPart/CharacterVfx.h"
#include "CharacterPart/CharacterShadow.h"
#include "CharacterPart/CharacterAir.h"
#include "CharacterPart/CharacterForward.h"
#include "CharacterPart/CharacterMovement.h"
#include "CharacterPart/CharacterRotate.h"
#include "CharacterPart/CharacterCharge.h"
#include"CharacterPart/CharacterHitStop.h"
#include "CharacterPart/CharacterHit.h"
#include "CharacterPart/CharacterFence.h"
#include"CharacterPart/CharacterWait.h"
#include "CharacterPart/CharacterCsvLoader.h"
#include "CharacterPart/CharacterObserver.h"
#include"CharacterPart/CharacterDebugPanel.h"

//インターフェース

#include"../InterfaceSourceFile/IVFXEventListener.h"
#include"../InterfaceSourceFile/IChargeEventListner.h"
#include"../InterfaceSourceFile/IRotateEventListner.h"
#include"../InterfaceSourceFile/IMovementEventListener.h"


//プレイヤー,敵クラスの共通事項クラス
class Character :
    public GameObject, 
    public IVFXEventListener, public IChargeEventListener,
    public IRotateEventListener, public IMovementEventListener
{
public:
    //----------モジュール群----------

    std::unique_ptr<CharacterModelBlink>    modeldraw_;
    std::unique_ptr<CharacterVFX>           vfx_;
    std::unique_ptr<CharacterShadow>        shadow_;
    std::unique_ptr<CharacterAir>           air_;
    std::unique_ptr<CharacterForward>       forward_;
    std::unique_ptr<CharacterMovement>      movement_;
    std::unique_ptr<CharacterRotate>        rotate_;
    std::unique_ptr<CharacterCharge>        charge_;
    std::unique_ptr<CharacterHitStop>       hitstop_;
    std::unique_ptr<CharacterHit>           hit_;
    std::unique_ptr<CharacterFence>         fence_;
	std::unique_ptr<CharacterWait>          wait_;
    std::unique_ptr<CharacterCsvLoader>     csvload_;
    std::unique_ptr<CharacterObserver>      observer_;
    std::unique_ptr<CharacterDebugPanel>    debugpanel_;

protected:

    //使用するパラメータ(CharacterParams)のポインタ
    //protectedにして継承先のみアクセス可能にする(モジュールのparams_と混ざらないように)
    std::unique_ptr<CharacterParams> params_;

public:
    Character(GameObject* parent);
    Character(GameObject* parent, const std::string& name);

    virtual ~Character();

    //更新(継承先の共通処理のみ行う)
    void Update() override;

    //描画(継承先の共通描画のみ行う)
    void Draw() override;

    //チャージ中エフェクトを出すイベント
    void OnChargeVFX() override
    {
        vfx_->SetFullChargeEffect(); 
    }

    //最大チャージエフェクトを出すイベント
    void OnFullChargeVFX() override
    {
        vfx_->SetFullChargeEffect();
    }

    //軌跡エフェクトを出すイベント
    void OnAttackLocusVFX() override
    {
        vfx_->SetAttackLocusEffect();
    }

    //接触エフェクトを出すイベント
    void OnHitVFX() override
    {
        vfx_->SetHitEffect();
    }

    //柵接触エフェクトを出すイベント
    void OnFenceHitVFX() override
    {
        vfx_->SetFenceHitEffect();
    }

    //チャージ量(TmpAccele_)を0にするイベント
    void OnChargeReset() override
    {
        charge_->ChargeReset();
    }

    //X軸回転を行うイベント
    void OnMoveRotateX() override
    {
        rotate_->MoveRotateX();
    }

    //高速X軸回転を行うイベント
    void OnFastRotateX() override
    {
        rotate_->FastRotateX();
    }

    //X軸回転停止を行うイベント
    void OnRotateXStop() override
    {
        rotate_->RotateXStop();
    }

    //移動確定判定を行うイベント
    bool OnIsOutsideStage(DirectX::XMFLOAT3 _position) override
    {
        return movement_->IsOutsideStage(_position);
    }

    //加速度を0にするイベント
    void OnAccelerationStop() override
    {
        movement_->AccelerationStop();
    }

    //移動確定処理を行うイベント
    void OnMoveConfirm() override
    {
		movement_->MoveConfirm();
    }

    //共通パラメータ群のゲッター関数
    CharacterParams* GetParams() const {
        return params_.get();
    }

};