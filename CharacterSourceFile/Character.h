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


//プレイヤー,敵クラスの共通事項クラス
class Character :
    public GameObject
{
protected:
    //----------モジュール群----------

    //各モジュールのポインタ 処理ごとに分割
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

    //使用するパラメータ(CharacterParams)のポインタ
    //protectedにして自身か継承先のみアクセス可能にする(モジュールのparams_と混ざらないように)
    std::unique_ptr<CharacterParams> params_;

public:
    Character(GameObject* parent);
    Character(GameObject* parent, const std::string& name);

    virtual ~Character();

    //更新(継承先の共通処理のみ行う)
    void Update() override;

    //描画(継承先の共通描画のみ行う)
    void Draw() override;

    //以下の関数はモジュール内で別モジュールの処理を呼び出すために使う
    //最大チャージエフェクトを出すイベント
    void OnFullChargeVFX(XMFLOAT3 _pos) 
    {
        vfx_->SetFullChargeEffect(_pos);
    }

    //軌跡エフェクトを出すイベント
    void OnAttackLocusVFX(XMFLOAT3 _pos) 
    {
        vfx_->SetAttackLocusEffect(_pos);
    }

    //接触エフェクトを出すイベント
    void OnHitVFX(XMFLOAT3 _pos) 
    {
        vfx_->SetHitEffect(_pos);
    }

    //柵接触エフェクトを出すイベント
    void OnFenceHitVFX(XMFLOAT3 _pos) 
    {
        vfx_->SetFenceHitEffect(_pos);
    }

    //チャージ量(TmpAccele_)を0にするイベント
    void OnChargeReset() 
    {
        charge_->ChargeReset();
    }

    //X軸回転を行うイベント
    void OnMoveRotateX() 
    {
        rotate_->MoveRotateX();
    }

    //高速X軸回転を行うイベント
    void OnFastRotateX() 
    {
        rotate_->FastRotateX();
    }

    //X軸回転停止を行うイベント
    void OnRotateXStop() 
    {
        rotate_->RotateXStop();
    }

    //移動確定判定を行うイベント
    bool OnIsOutsideStage(DirectX::XMFLOAT3 _position) 
    {
        return movement_->IsOutsideStage(_position);
    }

    //加速度を0にするイベント
    void OnAccelerationStop() 
    {
        movement_->AccelerationStop();
    }

    //移動確定処理を行うイベント
    void OnMoveConfirm() 
    {
		movement_->MoveConfirm();
    }

    //共通パラメータ群のゲッター関数
    CharacterParams* GetParams() const { return params_.get(); }

    //各モジュールのゲッター関数
    CharacterMovement* GetModuleMovement() const { return movement_.get(); }
    CharacterVFX* GetModuleVFX() const { return vfx_.get(); }
    CharacterAir* GetModuleAir() const { return air_.get(); }
    CharacterCharge* GetModuleCharge() const { return charge_.get(); }
    CharacterRotate* GetModuleRotate() const { return rotate_.get(); }
    CharacterHitStop* GetModuleHitStop() const { return hitstop_.get(); }
    CharacterHit* GetModuleHit() const { return hit_.get(); }
    CharacterFence* GetModuleFence() const { return fence_.get(); }
    CharacterWait* GetModuleWait() const { return wait_.get(); }
    CharacterModelBlink* GetModuleModelBlink() const { return modeldraw_.get(); }
    CharacterObserver* GetModuleObserver() const { return observer_.get(); }

};