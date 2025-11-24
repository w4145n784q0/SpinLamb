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

    //----------継承先で使用する仮想関数----------

    //継承先特有の当たり判定処理
    virtual void OwnCharacterCollision() {};
    virtual void OwnFenceCollision() {};

    //自身の特定の状態を返す処理(両方のステートに共通してあるもの)
    //IsCharacterStateAttack()のみ、現状Playerだけで使っている
    virtual bool IsCharacterStateAttack() { return false; }
    virtual bool IsCharacterStateHitStop() { return false; };
    virtual bool IsCharacterStateHit() { return false; };
    virtual bool IsCharacterStateFenceHit() { return false; };


    //以下の関数はステートクラス内やモジュール内から別モジュールの処理を呼び出すために使う
    //関数全部は記述せず使用する関数のみ記述
    //GetModuleからでも可能だが、ステートクラスやモジュール側が他モジュールの存在を
    //知らなくても使えるようにして、結合度を下げるためここに記述

    //-----VFX関連-----

    //チャージ中エフェクトを出すイベント
    void OnChargingEffect(std::string _path, XMFLOAT3 _pos)
    {
        vfx_->SetChargingEffect(_path, _pos);
    }

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

    //着地エフェクトを出すイベント
    void OnLandingVFX(XMFLOAT3 _pos)
    {
        vfx_->SetLandingEffect(_pos);
    }

    //-----チャージ関連-----
    //チャージ量(TmpAccele_)を0にするイベント

    //攻撃加速度の上昇イベント
    void OnCharging()
    {
        charge_->Charging();
    }

    //チャージ量解放イベント
    void OnChargeRelease()
    {
        charge_->ChargeRelease();
    }

    //チャージ量(TmpAccele)のリセットイベント
    void OnChargeReset() 
    {
        charge_->ChargeReset();
    }

    //チャージ用矢印モデルの位置をセットするイベント
    void OnSetArrow()
    {
        charge_->SetArrow();
    }

    //矢印モデルの描画イベント
    void OnDrawArrow()
    {
        charge_->DrawArrow();
    }

    //-----空中関連-----
    //ジャンプ開始イベント
    void OnSetJump()
    {
        air_->SetJump();
    }

    //-----回転関連-----
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

    //-----移動関連-----
    //通常移動イベント
    void OnMoveUpdate(XMVECTOR _input)
    {
        movement_->MoveUpdate(_input);
    }

    //ダッシュ攻撃を行うイベント
    void OnCharacterAttackMove(XMVECTOR _direction)
    {
        movement_->CharacterAttackMove(_direction);
    }

    //通常時の摩擦減速処理イベント
    void OnFrictionNormalDeceleration()
    {
        movement_->FrictionNormalDeceleration();
    }

    //ダッシュ攻撃時の摩擦減速処理イベント
    void OnFrictionAttackDeceleration()
    {
        movement_->FrictionAttackDeceleration();
    }

    //移動確定判定を行うイベント
    bool OnIsOutsideStage(DirectX::XMFLOAT3 _position)
    {
        return movement_->IsOutsideStage(_position);
    }

    //移動確定処理を行うイベント
    void OnMoveConfirm()
    {
        movement_->MoveConfirm();
    }

    //加速度が0.0以下か判定するイベント
    bool OnIsAcceleStop()
    {
        return movement_->IsAcceleStop();
    }

    //加速度を0にするイベント
    void OnAccelerationStop() 
    {
        movement_->AccelerationStop();
    }

    //攻撃時のアニメーション更新イベント
    void OnUpdateDashImage()
    {
        movement_->UpdateDashImage();
    }

    //攻撃時のアニメーション更新イベント
    void OnDrawDashImage()
    {
        movement_->DrawDashImage();
    }

    //-----ヒットストップ関連-----
    //ヒットストップ時間の加算イベント
    void OnHitStopTimerAdd()
    {
        hitstop_->HitStopTimerAdd();
    }

    //ヒットストップ時間終了判定イベント
    bool OnIsTimeOverHitStopTime()
    {
        return hitstop_->IsTimeOverHitStopTime();
    }

    //ヒットストップ時間のリセットイベント
    void OnHitStopTimerReset()
    {
        hitstop_->HitStopTimerReset();
    }

    //-----被弾・柵に接触関連-----
    //ノックバックイベント
    void OnKnockBack()
    {
        hit_->KnockBack();
    }

    //ノックバック終了判定イベント
    bool OnIsKnockBackEnd()
    {
        return hit_->IsKnockBackEnd();
    }

    //ノックバック量のリセットイベント
    void OnKnockBackVelocityReset()
    {
        hit_->KnockBackVelocityReset();
    }

    //柵との反射処理イベント
    void OnFenceReflect(XMVECTOR _dir)
    {
        fence_->FenceReflect(_dir);
    }

    //-----待機関連-----
    //待機時間の加算イベント
    void OnWaitTimerAdd()
    {
        wait_->WaitTimerAdd();
    }

    //待機終了判定イベント
    bool OnIsTimeOverWaitTime()
    {
        return wait_->IsTimeOverWaitTime();
    }

    //待機時間のリセットイベント
    void OnWaitTimeReset()
    {
        wait_->WaitTimeReset();
    }

    //-----オブザーバー関連-----
    //監視対象を追加イベント
    void OnAddObserver(IGameObserver* _observer)
    {
        observer_->AddObserver(_observer);
    }

    //共通パラメータのゲッター関数
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