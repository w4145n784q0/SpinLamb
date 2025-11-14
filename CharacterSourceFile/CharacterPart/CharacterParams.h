#pragma once
#include"../../InterfaceSourceFile/IGameObserver.h"
#include"../../StageSourceFile/Ground.h"

//プレイヤー,敵クラスの共通パラメータのみを扱うファイル

class CharacterParams :
    public GameObject 
{
public:
    CharacterParams(GameObject* parent);
    virtual ~CharacterParams() = default;

    void Initialize() override {};
    void Update() override {};
    void Draw() override {};
    void Release() override {};

    //----------ステージの端----------
    struct EndParam
    {
        //キャラクターが持つ移動制限の値
        float NorthEnd_ = 0.0f;     //ステージ北端(前方)の位置
        float SouthEnd_ = 0.0f;     //ステージ南端(後方)の位置
        float EastEnd_ = 0.0f;      //ステージ東端(右側)の位置
        float WestEnd_ = 0.0f;      //ステージ西端(左側)の位置
    };
    EndParam EndParam_;

    //----------サウンドハンドル----------
    struct SoundParam
    {

        int hSoundcharge_ = -1;     //チャージ音のハンドル
        int hSoundattack_ = -1;     //突撃音のハンドル
        int hSoundCollision_ = -1;  //接触音のハンドル
        int hSoundJump_ = -1;       //ジャンプ音のハンドル
    };
    SoundParam SoundParam_;

    //----------初期状態----------
    struct InitializeParam
    {
        int CharacterID = -1;                   //ゲームに参加するキャラクターのid
        XMFLOAT3 StartPosition_ = { 0,0,0 };    //初期位置
        XMVECTOR FrontDirection_ = { 0,0,1 };   //正面の初期値(ローカル座標系) ここからどれだけ回転したか
        std::vector<IGameObserver*> observers;  //監視される対象の配列
    };
    InitializeParam InitParam_;

    //----------移動----------
    struct MoveParam
    {
        //通常の移動に使用
		float NormalAcceleValue_ = 0.0f;        //通常移動時、加速度の1fあたりの増加量
        float NormalFullAccelerate_ = 0.0f;     //通常移動時の加速度の増加量の最大(これとNormalVelocity_の合計が通常時の最高速にあたる)
        float NormalFriction_ = 0.0f;           //通常移動の摩擦係数(減速率) 1fあたりの減速量

        //チャージ・ダッシュ攻撃中に使用
        float TmpAccele_ = 0.0f;                //チャージ中の加速度上昇時に使う仮の値 チャージ中はこれが上昇する
        float AttackAcceleValue_ = 0.0f;        //チャージ中の加速度上昇時、1fあたりの増加量
        float AttackFullAccelerate_ = 0.0f;     //チャージ中の加速度の増加量の最大
        float AttackFriction_ = 0.0f;           //ダッシュ攻撃の摩擦係数(減速率) 1fあたりの減速量

        //チャージ中の矢印関連
        int hMoveArrow_ = -1;                   //チャージ中に表示する矢印モデル
        Transform ArrowTransform_;              //チャージ/攻撃準備中の矢印のトランスフォーム
        XMFLOAT3 ArrowRotate_ = { 0,0,0 };      //矢印の初期回転 csvから初期値を読み込む際に使用
        XMFLOAT3 ArrowScale_ = { 0,0,0 };       //矢印の大きさ csvから初期値を読み込む際に使用
        float AddArrowDepth_ = 0.0f;            //矢印の奥行き(前方向)の調整値

        //共通
        float CommonAcceleration_ = 0.0f;       //通常・ダッシュ共通の加速度

        XMVECTOR ForwardVector_ = { 0,0,0 };    //キャラクターから見た正面の方向(ワールド座標系) 自身のy軸回転量とかけて計算 正規化した値を入れる
        XMVECTOR MoveDirection_ = { 0,0,0 };    //移動方向 この値に速さの要素をかけて移動ベクトル化する
        XMVECTOR NewPosition_ = { 0,0,0 };      //移動後の位置ベクトル

    };
    MoveParam MoveParam_;

     //----------回転----------
    struct RotateParam
    {
        float MoveRotateX = 0.0f;   //移動時の1fの回転量
        float FastRotateX = 0.0f;   //(チャージ中など)高速回転中の1fの回転量
    };
    RotateParam RotateParam_;

    //----------空中----------
    struct JumpParam
    {
        float Gravity_ = 0.0f;          //重力 キャラクターの下方向にかかる力 実際の重力(9.8/60 m/s)より軽くしている
        bool IsOnGround_ = false;       //地面にいるか
        float JumpSpeed_ = 0.0f;        //キャラクターの上方向に向く力 +ならジャンプしている状態 -なら下降～地面にいる状態
        float JumpHeight = 0.0f;        //ジャンプ時の一時的に代入する値(=ジャンプの高さ)
        float HeightLowerLimit_ = 0.0f; //高さの下限
        float HeightUpperLimit_ = 0.0f; //高さの上限
        float MinusLimit_ = 0.0f;       //JumpSpeedの最低値(念のためオーバーフローを防止する)
    };
    JumpParam JumpParam_;


    //----------被弾----------
    struct HitParam
    {
        float ColliderSize_ = 0.0f;                 //当たり判定(球体)のサイズ
        int HitStopTimer_ = 0;                      //ヒットストップ時間を計測するタイマー
        int HitStopValue_ = 0;                      //ヒットストップする時間 この値を超えるとヒットストップ時間終了
        float OriginalRangeMin_ = 0.0f;             //変換元のノックバック量の最小値
        float OriginalRangeMax_ = 0.0f;             //変換元のノックバック量の最大値
        float ConvertedRangeMin_ = 0.0f;            //変換後のノックバック量の最小値
        float ConvertedRangeMax_ = 0.0f;            //変換後のノックバック量の最大値
        XMFLOAT3 KnockBack_Direction_ = { 0,0,0 };  //ノックバックする方向
        XMFLOAT3 KnockBack_Velocity_ = { 0,0,0 };   //ノックバックする速度
        float DecelerationRate_ = 0.0f;             //ノックバック時の1fごとの減速率
        float KnockBackEnd_ = 0.0f;                 //ノックバックを終了する値
        std::string AttackedName_ = "";             //接触した相手の名前
    };
    HitParam HitParam_;

    //----------柵の接触----------
    struct FenceHitParam
    {
        XMVECTOR UpperNormal_ = { 0,0,0 };                      //ステージ北端(前方)の法線ベクトル
        XMVECTOR LowerNormal_ = { 0,0,0 };                      //ステージ南端(後方)の法線ベクトル
        XMVECTOR RightNormal_ = { 0,0,0 };                      //ステージ東端(右側)の法線ベクトル
        XMVECTOR LeftNormal_ = { 0,0,0 };                       //ステージ西端(左側)の法線ベクトル
        std::vector<XMVECTOR> NormalArray_ = {};                //各法線ベクトルを格納した配列
        std::vector<std::string> WireArray_ =
        { "UpperWire", "LowerWire", "RightWire" ,"LeftWire" };  //各鉄線の名前の配列

        float KnockBackPower_ = 0.0f;                           //柵ヒットでノックバックする強さ（変化なし）
        int InvincibilityTime_ = 0;                             //ダメージ後の無敵時間 1fごとに上昇
        bool IsInvincibility_ = false;                          //無敵時間か
        int InvincibilityValue_ = 0;                            //無敵時間の値　この値を超えると無敵時間終了
        int BlinkTimer_ = 0;                                    //ダメージ後の点滅カウント
        int BlinkValue_ = 0;                                    //この値にblinkTimerが到達すると通常描画する(それまでは点滅)
    };
    FenceHitParam FenceHitParam_;

    //----------待機----------
    struct WatiParam
    {
        int WaitTimer_ = 0;  //待機時間タイマー
		int WaitValue_ = 0;  //待機時間 この値を超えると待機終了
    };
    WatiParam WaitParams_;

    //----------影付け----------
    struct ShadowParam
    {
        int hShadow_ = -1;                  //影のモデルハンドル
        float ShadowHeight_ = 0.0f;         //影をつける高さ
        float ShadowCorrection_ = 0.0f;     //影の位置の調整値
        Transform ShadowTrans_;             //影の描画トランスフォーム
        Ground* pGround_ = nullptr;         //地面のインスタンス
    };
    ShadowParam ShadowParam_;

    //----------エフェクト関連----------

    struct EffectParam
    {
        std::vector<float> ChargeParam_ = {};        //チャージ状態エフェクトのパラメータ
        std::vector<float> FullChargeParam = {};     //最大チャージ状態エフェクトのパラメータ
        std::vector<float> AttackLocusParam_ = {};   //突撃エフェクトのパラメータ
        std::vector<float> HitEffectParam_ = {};     //接触時の衝撃エフェクトのパラメータ
        std::vector<float> FenceHitEffectParam_ = {};//柵に接触時の衝撃エフェクトのパラメータ
    };
    EffectParam EffectParam_;

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

    void SetAcceleration(float _acceleration) { MoveParam_.CommonAcceleration_ = _acceleration; }
    float GetAcceleration() const { return MoveParam_.CommonAcceleration_; }

	void SetNormalAcceleValue(float _acceleValue) { MoveParam_.NormalAcceleValue_ = _acceleValue; }
	float GetNormalAcceleValue() const { return MoveParam_.NormalAcceleValue_; }

	void SetFullNormalAccelerate(float _fullNormalAccelerate) { MoveParam_.NormalFullAccelerate_ = _fullNormalAccelerate; }
	float GetFullNormalAccelerate() { return MoveParam_.NormalFullAccelerate_; }

    void SetNormalFriction(float _friction) { MoveParam_.NormalFriction_; }
    float GetNormalFriction() { return  MoveParam_.NormalFriction_; }

    void SetTmpAccele(float _tmpAccele) { MoveParam_.TmpAccele_ = _tmpAccele; }
    float GetTmpAccele() const { return MoveParam_.TmpAccele_; }

	void SetAttackAcceleValue(float _acceleValue) { MoveParam_.AttackAcceleValue_ = _acceleValue; }
	float GetAttackAcceleValue() const { return MoveParam_.AttackAcceleValue_; }

	void SetFullAttackAccelerate(float _fullAttackAccelerate) { MoveParam_.AttackFullAccelerate_ = _fullAttackAccelerate; }
	float GetFullAttackAccelerate() { return MoveParam_.AttackFullAccelerate_; }

    void SetAttackFriction(float _friction) { MoveParam_.AttackFriction_; }
    float GetAttackFriction() { return  MoveParam_.AttackFriction_; }

    void SetForwardVector(XMVECTOR _forward) { MoveParam_.ForwardVector_ = _forward; }
    XMVECTOR GetForwardVector() const { return MoveParam_.ForwardVector_; }

    //回転
    void SetMoveRotateX(float _moveRotate) { RotateParam_.MoveRotateX = _moveRotate; }
    float GetMoveRotateX() const { return RotateParam_.MoveRotateX; }

    void SetFastRotateX(float _fastRotate) { RotateParam_.FastRotateX = _fastRotate; }
    float GetFastRotateX() const { return RotateParam_.FastRotateX; }

    //空中
    void SetGravity(float _gravity) { JumpParam_.Gravity_ = _gravity; }
    float GetGravity() const { return JumpParam_.Gravity_; }

    void SetJumpHeight(float _jumpHeight) { JumpParam_.JumpHeight = _jumpHeight; }
    float GetJumpHeight() const { return JumpParam_.JumpHeight; }

    //被弾
	void SetHitStopTimer_(int _hitStopTimer) { HitParam_.HitStopTimer_ = _hitStopTimer; }
	int GetHitStopTimer_() const { return HitParam_.HitStopTimer_; }

	void SetHitStopValue(int _hitStopValue) { HitParam_.HitStopValue_ = _hitStopValue; }
	int GetHitStopValue() const { return HitParam_.HitStopValue_; }

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

	bool GetIsInvincibility() const { return FenceHitParam_.IsInvincibility_; }

    void SetInvincibilityValue(int _invincibilityValue) { FenceHitParam_.InvincibilityValue_ = _invincibilityValue; }
    int GetInvincibilityValue() const { return FenceHitParam_.InvincibilityValue_; }

    void SetBlinkTimer(int _blinkTimer) { FenceHitParam_.BlinkTimer_ = _blinkTimer; }
    int GetBlinkTimer() const { return  FenceHitParam_.BlinkTimer_; }

    void SetBlinkValue(int _blinkValue) { FenceHitParam_.BlinkValue_ = _blinkValue; }
    int GetBlinkValue() const { return FenceHitParam_.BlinkValue_; }
};