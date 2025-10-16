#pragma once
#include "../Engine/GameObject.h"

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
#include "CharacterPart/CharacterHit.h"
#include "CharacterPart/CharacterFence.h"
#include "CharacterPart/CharacterCsvLoader.h"
#include "CharacterPart/CharacterObserver.h"

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

    CharacterModelBlink* blink_;
    CharacterVFX*        vfx_;
    CharacterShadow*     shadow_;
    CharacterAir*        air_;
    CharacterForward*    forward_;
    CharacterMovement*   movement_;
    CharacterRotate*     rotate_;
    CharacterCharge*     charge_;
    CharacterHit*        hit_;
    CharacterFence*      fence_;
    CharacterCsvLoader*  csvload_;
    CharacterObserver*   observer_;

protected:

    //----------共通パラメータ群----------
    EndParam EndParam_;
    SoundParam SoundParam_;
    InitializeParam InitParam_;
    MoveParam MoveParam_;
    RotateParam RotateParam_;
    JumpParam JumpParam_;
    HitParam HitParam_;
    FenceHitParam FenceHitParam_;
    ShadowParam ShadowParam_;
    EffectParam EffectParam_;

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

    //----------初期化----------

    /// <summary>
    /// CSVから各ステータスを読み込みする
    /// Characterクラス限定
    /// </summary>
    /// <param name="_path">csvファイルのパス</param>
    //void SetCSVStatus(std::string _path);

    /// <summary>
    /// 各方向の柵から法線ベクトルを取得しNormalArrayを初期化
    /// </summary>
    /// void GetWireNormal();

    /// <summary>
    /// 自身の位置を初期位置に設定
    /// </summary>
    //void SetStartPosition() { this->transform_.position_ = InitParam_.StartPosition_; }

    /// <summary>
    /// 矢印トランスフォームの初期化
    /// </summary>
    // void InitArrow();

    /// <summary>
    /// 自身を監視する対象を配列に追加
    /// BattleSceneでのみ行われる
    /// </summary>
    /// <param name="_observer">追加する監視対象</param>
    //void AddObserver(IGameObserver* _observer);

    /// <summary>
    /// 自身を監視する対象を配列から削除
    /// </summary>
    /// <param name="_observer">削除する監視対象</param>
    //void RemoveObserver(IGameObserver* _observer);

    //----------描画----------

    /// <summary>
    /// キャラクターモデル描画(ダメージ時の点滅表現等行う)
    /// </summary>
    /// <param name="_handle">モデルハンドル</param>
    /// <param name="_transform">描画位置のトランスフォーム</param>
    /// void DrawCharacterModel(int _handle, Transform _transform);

    /// <summary>
    /// キャラクター共通のImGuiを描画 個別要素は派生先で記述
    /// </summary>
    ///void DrawCharacterImGui();

    //----------移動----------

    ///// <summary>
    ///// キャラクターの移動処理
    ///// </summary>
    ///// <param name="_direction">動かす方向</param>
    //void CharacterMove(XMVECTOR _direction);

    ///// <summary>
    ///// 移動ベクトルをつくる(方向ベクトルが必要)
    ///// </summary>
    //void CreateMoveVector();

    ///// <summary>
    ///// 場外判定
    ///// </summary>
    ///// <param name="_position">更新予定の位置</param>
    ///// <returns>移動可能か</returns>
    //bool IsOutsideStage(XMFLOAT3 _position);

    ///// <summary>
    ///// 移動確定処理
    ///// </summary>
    //void MoveConfirm();

    /// <summary>
    /// Y軸の回転行列をベクトルに変換
    /// </summary>
    /// <param name="_rotY">Y軸に回転したい角度（Degree）</param>
    /// <param name="_front">正面ベクトル(ローカル空間)</param>
    /// <returns>変形したベクトル（ワールド空間）</returns>
    /// XMVECTOR RotateVecFront(float _rotY, XMVECTOR _front);

    /// <summary>
    /// 正面ベクトルを更新
    /// </summary>
    // void FrontVectorConfirm();

    /// <summary>
    /// 受け取ったベクトルからキャラクターの回転量を計算
    /// </summary>
    /// <param name="_MoveVector">進行したい方向ベクトル</param>
    /// <returns>回転する角度(Y軸回転)</returns>
    /// float RotateDirectionVector(XMVECTOR _MoveVector);

    //----------チャージ----------

    ///// <summary>
    ///// 加速度の加算
    ///// </summary>
    //void Charging();

    ///// <summary>
    ///// 蓄積したTmpAccele_を実際に加速度に代入
    ///// </summary>
    //void ChargeRelease();

    ///// <summary>
    ///// TmpAccele_を0にする
    ///// </summary>
    //void ChargeReset();

    ///// <summary>
    ///// チャージ中の矢印位置をセット
    ///// </summary>
    //void SetArrow();

    ///// <summary>
    ///// 矢印モデルの描画
    ///// </summary>
    //void DrawArrow();

    //----------攻撃----------

 /*   /// <summary>
    /// 減速処理(加速時の増加量使用)
    /// </summary>
    void Deceleration() { MoveParam_.Acceleration_ -= MoveParam_.AcceleValue_; }

    /// <summary>
    /// 摩擦による減速処理
    /// </summary>
    void FrictionDeceleration() { MoveParam_.Acceleration_ -= MoveParam_.Friction_; }

    /// <summary>
    /// 加速度リセット
    /// </summary>
    void AccelerationStop() { MoveParam_.Acceleration_ = 0.0f; }

    /// <summary>
    /// 停止判定
    /// </summary>
    /// <returns>加速量が0.0以下かどうか</returns>
    bool IsDashStop() { if (MoveParam_.Acceleration_ <= 0.0f) return true; else return false; }*/


    //----------空中----------

 /*   /// <summary>
    /// 重力処理
    /// </summary>
    void CharacterGravity();

    /// <summary>
    /// ジャンプ開始
    /// </summary>
    void SetJump();*/

    //----------回転----------

    ///// <summary>
    ///// 通常X軸回転
    ///// </summary>
    //void MoveRotateX() { this->transform_.rotate_.x += RotateParam_.MoveRotateX; }

    ///// <summary>
    ///// 通常X軸回転(-x回転)
    ///// </summary>
    //void MoveRotateXReverse() { this->transform_.rotate_.x -= RotateParam_.MoveRotateX; }

    ///// <summary>
    ///// 高速X軸回転
    ///// </summary>
    //void FastRotateX() { this->transform_.rotate_.x += RotateParam_.FastRotateX; }

    ///// <summary>
    ///// 高速X軸回転(-x回転)
    ///// </summary>
    //void FastRotateReverse() { this->transform_.rotate_.x -= RotateParam_.FastRotateX; }

    ///// <summary>
    ///// X回転を止める
    ///// </summary>
    //void RotateXStop() { this->transform_.rotate_.x = 0.0f; }

    //----------被弾----------

    /// <summary>
    /// キャラクター同士の反射処理
    /// </summary>
    /// <param name="_myVector">自身の位置ベクトル</param>
    /// <param name="_targetVector">相手の位置ベクトル</param>
    /// <param name="_myVelocity">自身の加速度</param>
    /// <param name="_targetVelocity">相手の加速度</param>
    /// <param name="_attackName">攻撃したキャラクターの名前</param>
    //void Reflect(XMVECTOR _myVector, XMVECTOR _targetVector, float _myVelocity, float _targetVelocity, 
    //    std::string _attackName);

    ///// <summary>
    ///// ノックバック中のY軸回転角の計算 ノックバック直前に行う
    ///// </summary>
    ///// <param name="_KnockBackVector">ノックバックする方向(正規化されていること前提)</param>
    ///// <param name="_KnockBackValue">ノックバックする量</param>
    //void KnockBackAngleY(XMFLOAT3 _KnockBackVector, float _KnockBackValue);

    ///// <summary>
    ///// ノックバック移動処理
    ///// </summary>
    //void KnockBack();

    ///// <summary>
    ///// 指定された鉄線の名前に応じた各法線ベクトルを取得する
    ///// </summary>
    ///// <param name="_normal">キャラクターが接触した鉄線の名前</param>
    ///// <returns>鉄線の名前に対応した法線ベクトル</returns>
    //XMVECTOR HitNormal(std::string _normal);

    /// <summary>
    /// 柵に接触した際の計算処理　柵の法線で計算
    /// </summary>
    /// <param name="_normal">反射される方向(接触した柵の法線ベクトル)</param>
    //void FenceReflect(XMVECTOR _normal);

    /// <summary>
    /// ノックバック終了判定
    /// </summary>
    /// <returns>ノックバック速度が終了値以下か</returns>
    ///bool IsKnockBackEnd();

    /// <summary>
    /// ノックバック速度を0に戻す
    /// </summary>
    /// void KnockBackVelocityReset() { HitParam_.KnockBack_Velocity_ = { 0,0,0 };}

    /// <summary>
    /// ダメージ後の無敵時間の計算
    /// </summary>
    //void InvincibilityTimeCalculation();

    /// <summary>
    /// 監視する対象(配列)に柵にヒットしたことを通知
    /// </summary>
    //void NotifyFenceHit();

    //----------影付け----------

    ///// <summary>
    ///// 影モデルの初期化
    ///// </summary>
    //void InitShadow();

    ///// <summary>
    ///// 影付け（毎フレーム更新）
    ///// </summary>
    //void ShadowSet();

    ///// <summary>
    ///// 影モデル描画
    ///// </summary>
    //void ShadowDraw();


    //----------エフェクト処理----------

  /*  /// <summary>
    /// CSV用データの初期化
    /// </summary>
    void InitCSVEffect();

    /// <summary>
    /// チャージ状態エフェクトをつける
    /// </summary>
    /// <param name="_path">csvを読み込むパス</param>
    void SetChargingEffect(std::string _path);

    /// <summary>
    /// 最大チャージ状態エフェクト(光の拡散)をつける
    /// </summary>
    void SetFullChargeEffect();

    /// <summary>
    /// 突撃エフェクトつける(プレイヤーの背後に光の粒子)
    /// </summary>
    void SetAttackLocusEffect();

    /// <summary>
    /// 相手と接触時の衝撃エフェクトつける
    /// </summary>
    void SetHitEffect();

    /// <summary>
    /// 柵に接触時の衝撃エフェクトつける
    /// </summary>
    void SetFenceHitEffect();*/

    //----------セッター・ゲッター関数----------

    //キャラクターの移動制限をセット
    void SetEnd(float _upper, float _lower, float _left, float _right) {
        EndParam_.NorthEnd_ = _upper;
        EndParam_.SouthEnd_ = _lower;
        EndParam_.WestEnd_ = _left;
        EndParam_.EastEnd_ = _right;
    }

    //初期状態

    void SetID(int _id) { InitParam_.CharacterID = _id; }
    int GetID() const { return InitParam_.CharacterID; }

    void SetStartPosition(XMFLOAT3 _pos) { InitParam_.StartPosition_ = _pos; }
    XMFLOAT3 GetStartPosition() const { return InitParam_.StartPosition_; }

    //移動

    void SetVelocity(float _velocity) { MoveParam_.Velocity_ = _velocity; }
    float GetVelocity() const { return MoveParam_.Velocity_; }

    void SetAcceleration(float _acceleration) { MoveParam_.Acceleration_ = _acceleration; }
    float GetAcceleration() const { return MoveParam_.Acceleration_; }

    void SetTmpAccele(float _tmpAccele) { MoveParam_.TmpAccele_ = _tmpAccele; }
    float GetTmpAccele() const { return MoveParam_.TmpAccele_; }

    void SetAcceleValue(float _acceleValue) { MoveParam_.AcceleValue_ = _acceleValue; }
    float GetAcceleValue() const { return MoveParam_.AcceleValue_; }

    void SetFullAccelerate(float _fullAccelerate) { MoveParam_.FullAccelerate_; }
    float GetFullAccelerate() { return MoveParam_.FullAccelerate_; }

    void SetFriction(float _friction) { MoveParam_.Friction_; }
    float GetFriction() { return  MoveParam_.Friction_; }

    //回転
    void SetMoveRotateX(float _moveRotate) { RotateParam_.MoveRotateX = _moveRotate; }
    float GetMoveRotateX() const { return RotateParam_.MoveRotateX ; }

    void SetFastRotateX(float _fastRotate) { RotateParam_.FastRotateX = _fastRotate; }
    float GetFastRotateX() const { return RotateParam_.FastRotateX; }

    //空中
    void SetGravity(float _gravity) { JumpParam_.Gravity_ = _gravity; }
    float GetGravity() const { return JumpParam_.Gravity_ ; }

    void SetJumpHeight(float _jumpHeight) { JumpParam_.JumpHeight = _jumpHeight; }
    float GetJumpHeight() const { return JumpParam_.JumpHeight; }

    //被弾
    void SetOriginalRangeMin(float _originalRangeMin) { HitParam_.OriginalRangeMin_ = _originalRangeMin; }
    float GetOriginalRangeMin() const { return HitParam_.OriginalRangeMin_; }

    void SetOriginalRangeMax(float _originalRangeMax) { HitParam_.OriginalRangeMax_ = _originalRangeMax; }
    float GetOriginalRangeMax() const { return HitParam_.OriginalRangeMax_; }

    void SetConvertedRangeMin(float _convertedRangeMin) { HitParam_.ConvertedRangeMin_ = _convertedRangeMin; }
    float GetConvertedRangeMin() const { return  HitParam_.ConvertedRangeMin_; }

    void SetConvertedRangeMax(float _convertedRangeMax) { HitParam_.ConvertedRangeMax_ = _convertedRangeMax; }
    float GetConvertedRangeMax() const { return  HitParam_.ConvertedRangeMax_; }

    void SetKnockBackDirection(XMFLOAT3 _knockbackDirection) { HitParam_.KnockBack_Direction_ = _knockbackDirection; }
    XMFLOAT3 GetKnockBackDirection()  const { return  HitParam_.KnockBack_Direction_; }

    void SetKnockBackVelocity(XMFLOAT3 _knockbackVelocity) { HitParam_.KnockBack_Velocity_ = _knockbackVelocity; }
    XMFLOAT3 GetKnockBackVelocity() const { return HitParam_.KnockBack_Velocity_; }

    void SetDecelerationRate(float _decelerationRate) { HitParam_.DecelerationRate_ = _decelerationRate; }
    float GetDecelerationRate()  const { return  HitParam_.DecelerationRate_; }

    void SetKnockBackEnd(float _knockbackEnd) { HitParam_.KnockBackEnd_ = _knockbackEnd; }
    float GetKnockBackEnd() const { return HitParam_.KnockBackEnd_; }

    void SetAttackedName(std::string _AttackedName) { HitParam_.AttackedName_ = _AttackedName; }
    std::string GetAttackedName() const { return HitParam_.AttackedName_; }

    //柵の接触
    void SetKnockBackPower(float _knockbackPower) { FenceHitParam_.KnockBackPower_ = _knockbackPower; }
    float GetKnockBackPower() const { return FenceHitParam_.KnockBackPower_; }

    void SetInvincibilityTime(int _invincibilityTime) { FenceHitParam_.InvincibilityTime_ = _invincibilityTime; }
    int GetInvincibilityTime() const { return FenceHitParam_.InvincibilityTime_; }

    void SetInvincibilityValue(int _invincibilityValue) { FenceHitParam_.InvincibilityValue_ = _invincibilityValue; }
    int GetInvincibilityValue() const { return FenceHitParam_.InvincibilityValue_; }

    void SetBlinkTimer(int _blinkTimer) { FenceHitParam_.BlinkTimer_ = _blinkTimer; }
    int GetBlinkTimer() const { return  FenceHitParam_.BlinkTimer_; }

    void SetBlinkValue(int _blinkValue) { FenceHitParam_.BlinkValue_ = _blinkValue; }
    int GetBlinkValue() const { return FenceHitParam_.BlinkValue_; }
};