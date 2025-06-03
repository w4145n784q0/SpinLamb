#pragma once
#include "Engine/GameObject.h"
#include "Engine/CsvReader.h"
#include "Ground.h"

#include"UpperWire.h"
#include"LowerWire.h"
#include"LeftWire.h"
#include"RightWire.h"

//プレイヤー,敵クラスの共通事項クラス
class Character :
    public GameObject
{
private:
	//----------ステージの端----------
   float NorthEnd_ = 0.0f;
   float SouthEnd_ = 0.0f;
   float EastEnd_ = 0.0f;
   float WestEnd_ = 0.0f;
protected:
    
	//----------サウンドハンドル----------
	int hSoundcharge_ = -1; //チャージ音のハンドル
	int hSoundattack_ = -1; //突撃音のハンドル
    int hSoundCollision_ = -1;//接触音

    //----------初期状態----------
    struct InitializeParam
    {
        XMFLOAT3 StartPosition_ = {0,0,0};//初期位置
        XMVECTOR FrontDirection_ = {0,0,1};//正面の初期値(ローカル座標系) ここからどれだけ回転したか
    };
    InitializeParam InitParam_;

    //----------移動----------
    struct MoveParam   
    {   
        float Velocity_ = 0.0f;//初速度 この速度に加速度が加算される
        float Acceleration_ = 0.0f;//加速度
        float AcceleValue_ = 0.0f;//Acceleration_上昇時、1fあたりの増加量
        float FullAccelerate_ = 0.0f;//加速度の最大
        XMVECTOR ForwardVector_ = { 0,0,0 };//キャラクターから見た正面の方向(ワールド座標系) 自身のy軸回転量とかけて計算 正規化した値を入れる
        XMVECTOR MoveDirection_ = { 0,0,0 };//移動方向 この値に速さの要素をかけて移動ベクトル化する
        XMVECTOR NewPositon_ = { 0,0,0 };//移動後の位置ベクトル
    };
    MoveParam MoveParam_;

    //----------回転----------
    struct RotateParam
    {
        float MoveRotateX = 0.0f;//移動時の1fの回転量
        float FastRotateX = 0.0f;//(チャージ中など)高速回転中の1fの回転量
    };
    RotateParam RotateParam_;

    //----------空中----------
    struct JumpParam
    {
        float Gravity_ = 0.0f; //重力 キャラクターの下方向にかかる力
        bool IsOnGround_ = false;//地面にいるか
        float JumpSpeed_ = 0.0f; //プレイヤーの上方向に向く力 +ならジャンプしている状態 -なら下降～地面にいる状態
        float HeightLowerLimit_ = 0.0f;//高さの下限
        float HeightUpperLimit_ = 0.0f;//高さの上限
    };
    JumpParam JumpParam_;


    //----------被弾----------
    struct HitParam
    {
        float ColliderSize_ = 0.0f; //当たり判定(球体)のサイズ
		float OriginaRangeMin_ = 0.0f; //変換元のノックバック量の最小値
		float OriginaRangeMax_ = 0.0f;  //変換元のノックバック量の最大値
		float ConvertedRangeMin_ = 0.0f; //変換後のノックバック量の最小値
		float ConvertedRangeMax_ = 0.0f; //変換後のノックバック量の最大値
        XMFLOAT3 KnockBack_Direction_ = { 0,0,0 };//ノックバックする方向
        XMFLOAT3 KnockBack_Velocity_ = {0,0,0};//ノックバックする速度
        float KnockBackPower_ = 0.0f; //ノックバックする強さ（変化なし）
        float DecelerationRate_ = 0.0f;//ノックバック時の1fごとの減速率
        float KnockBackEnd_ = 0.0f;//ノックバックを終了する値
    };
    HitParam HitParam_;

    //----------壁の接触ダメージ----------
    struct WallHitParam
    {
        XMVECTOR UpperNormal_;//ステージ北端の法線ベクトル
        XMVECTOR LowerNormal_;//ステージ南端の法線ベクトル
        XMVECTOR RightNormal_;//ステージ東端の法線ベクトル
        XMVECTOR LeftNormal_;//ステージ西端の法線ベクトル

        int InvincibilityTime_ = 0;//ダメージ後の無敵時間 1fごとに上昇
        bool IsInvincibility_ = false;//無敵時間か
        int InvincibilityValue = 0;//無敵時間の値　この値を超えると無敵時間終了
        int blinkTimer = 0;//ダメージ後の点滅カウント
        int blinkValue = 0;//この値にblinkTimerが到達すると描画する
    };
    WallHitParam WallHitParam_;
    
    //----------影付け----------
    struct ShadowParam
    {
        int hShadow_ = -1;//影のモデルハンドル
        float ShadowHeight_ = 0.0f;//影をつける高さ
        float ShadowCorrection_ = 0.0f;//影の位置の調整値
        Transform ShadowTrans_;//影の描画トランスフォーム
        Ground* pGround_ = nullptr;//地面のインスタンス
    };
    ShadowParam ShadowParam_;

    //----------エフェクト関連----------

    std::vector<float> ChargeParam_ = {};//チャージ状態エフェクトのパラメータ
	std::vector<float> AttackLocusParam_ = {};//突撃エフェクトのパラメータ
	std::vector<float> HitEffectParam_ = {};//接触時の衝撃エフェクトのパラメータ
	std::vector<float> WallHitEffectParam_ = {};//壁に接触時の衝撃エフェクトのパラメータ
	std::vector<std::vector<float>> EffectArray_ = {};//エフェクトパラメータをまとめる配列

public:
    Character(GameObject* parent);
    Character(GameObject* parent, const std::string& name);

    virtual ~Character();

    /// <summary>
    /// CSVから各ステータスを読み込みする
    /// Characterクラス限定
    /// </summary>
    /// <param name="_path">csvファイルのパス</param>
    void SetcsvStatus(std::string _path);

    void GetWireNormal() {
        UpperWire* pUpperWire = (UpperWire*)FindObject("UpperWire");
        WallHitParam_.UpperNormal_ = pUpperWire->GetNormal();

        LowerWire* pLowerWire = (LowerWire*)FindObject("LowerWire");
		WallHitParam_.LowerNormal_ = pLowerWire->GetNormal();

        LeftWire* pLeftWire = (LeftWire*)FindObject("LeftWire");
		WallHitParam_.LeftNormal_ = pLeftWire->GetNormal();

        RightWire* pRightWire = (RightWire*)FindObject("RightWire");
		WallHitParam_.RightNormal_ = pRightWire->GetNormal();
    }

    //----------基本処理----------

    /// <summary>
    /// 初期位置の設定
    /// </summary>
    void SetStartPosition() { this->transform_.position_ = InitParam_.StartPosition_; }

    /// <summary>
    /// 重力処理
    /// </summary>
    void CharacterGravity();

    /// <summary>
    /// 影モデルの初期化
    /// </summary>
    void ShadowInit();

    /// <summary>
    /// 影付け（毎フレーム更新）
    /// </summary>
    void ShadowSet();

    /// <summary>
    /// 影モデル描画
    /// </summary>
    void ShadowDraw();

    /// <summary>
    /// キャラクターの移動処理(回転も行う)
    /// </summary>
    /// <param name="_direction">動かす方向ベクトル</param>
    void CharacterMoveRotate(XMVECTOR _direction, float rotateY);

    /// <summary>
    /// キャラクターの移動処理
    /// </summary>
    /// <param name="_direction">動かす方向ベクトル</param>
    void CharacterMove(XMVECTOR _direction);

    /// <summary>
    /// 移動ベクトルをつくる
    /// </summary>
    void CreateMoveVector();

    /// <summary>
    /// 場外判定
    /// </summary>
    /// <param name="_position">更新予定の位置</param>
    /// <returns>移動可能か</returns>
    bool IsOutsideStage(XMFLOAT3 _position);

    /// <summary>
    /// 移動確定処理
    /// </summary>
    void MoveConfirm();

    /// <summary>
    /// 正面ベクトルを更新
    /// </summary>
    void FrontVectorConfirm(){ 
        MoveParam_.ForwardVector_ = RotateVecFront(this->transform_.rotate_.y, InitParam_.FrontDirection_);
    }

    /// <summary>
    /// 反射処理
    /// </summary>
    /// <param name="myVector">自身の位置ベクトル</param>
    /// <param name="eVector">相手の位置ベクトル</param>
    /// <param name="myVelocity">自身の加速度</param>
    /// <param name="eVelocity">相手の加速度</param>
    void Reflect(XMVECTOR myVector, XMVECTOR eVector, float myVelocity, float eVelocity);

    /// <summary>
    /// ノックバック移動処理
    /// </summary>
    void KnockBack();

    /// <summary>
    /// 壁に接触した際の計算処理
    /// </summary>
    void WallHit();


    void WallReflect(XMVECTOR pos);

    /// <summary>
    /// ノックバック終了判定
    /// </summary>
    /// <returns>ノックバック速度が終了値以下か</returns>
    bool IsKnockBackEnd();

    /// <summary>
    /// ダメージ後の無敵時間の計算
    /// </summary>
    void InvincibilityTimeCalclation();

    /// <summary>
    /// 通常X軸回転
    /// </summary>
    void MoveRotate(){ this->transform_.rotate_.x += RotateParam_.MoveRotateX; }

    /// <summary>
    /// 通常X軸回転(-x回転)
    /// </summary>
    void MoveRotateReverse(){ this->transform_.rotate_.x -= RotateParam_.MoveRotateX; }

    /// <summary>
    /// 高速X軸回転
    /// </summary>
    void FastRotate(){ this->transform_.rotate_.x += RotateParam_.FastRotateX; }

    /// <summary>
    /// 高速X軸回転(-x回転)
    /// </summary>
    void FastRotateReverse() { this->transform_.rotate_.x -= RotateParam_.FastRotateX; }

    /// <summary>
    /// X回転を止める
    /// </summary>
    void RotateStop(){ this->transform_.rotate_.x = 0.0f; }

    /// <summary>
    /// 加速度の加算
    /// </summary>
    void Charging();

    /// <summary>
    /// 減速処理
    /// </summary>
    void Deceleration() { MoveParam_.Acceleration_ -= MoveParam_.AcceleValue_; }

    /// <summary>
    /// 加速度リセット
    /// </summary>
    void AccelerationStop(){ MoveParam_.Acceleration_ = 0.0f; }

    /// <summary>
    /// 停止判定
    /// </summary>
    /// <returns>加速量が0.0以下かどうか</returns>
    bool IsDashStop() { if (MoveParam_.Acceleration_ <= 0.0f) return true; else return false; }

    /// <summary>
    /// Y軸の回転行列をベクトルに変換
    /// </summary>
    /// <param name="rotY">Y軸に回転したい角度（Degree）</param>
    /// <param name="front">正面ベクトル(ローカル空間)</param>
    /// <returns>変形したベクトル（ワールド空間）</returns>
    XMVECTOR RotateVecFront(float rotY, XMVECTOR front);


    //----------エフェクト処理----------
    
    /// <summary>
    /// CSV用データの初期化
    /// </summary>
    void InitCSVEffect();

    /// <summary>
    /// チャージ状態エフェクトをつける
    /// </summary>
    /// <param name="_path">csvを読み込むパス</param>
    void SetChargingEffect(std::string _path);

    /// <summary>
    /// 突撃エフェクトつける(プレイヤーの背後に光の粒子)
    /// </summary>
    void SetAttackLocusEffect();

    /// <summary>
    /// 相手と接触時の衝撃エフェクトつける
    /// </summary>
    void SetHitEffect();

    /// <summary>
    /// 壁に接触時の衝撃エフェクトつける
    /// </summary>
    void SetWallHitEffect();

    //setter,getter関数

    void SetAcceleration(float _acceleration) { MoveParam_.Acceleration_ = _acceleration; }
    float GetAcceleration() { return MoveParam_.Acceleration_; }

    //キャラクターの移動制限をセット
    void SetEnd(float upper, float lower, float left, float right) {
        NorthEnd_ = upper;
        SouthEnd_ = lower;
        WestEnd_ = left;
        EastEnd_ = right;
    }
};