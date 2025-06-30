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
protected:

    //----------ステージの端----------
	float NorthEnd_ = 0.0f;//ステージ北端(前方)の位置
	float SouthEnd_ = 0.0f;//ステージ南端(後方)の位置
	float EastEnd_ = 0.0f;//ステージ東端(右側)の位置
	float WestEnd_ = 0.0f;//ステージ西端(左側)の位置

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
        float TmpAccele_ = 0.0f;//加速度上昇時に使う仮の値
        float AcceleValue_ = 0.0f;//Acceleration_上昇時、1fあたりの増加量
        float FullAccelerate_ = 0.0f;//加速度の最大
		float Friction_ = 0.0f;//摩擦係数(減速率) 1fあたりの減速量
        XMVECTOR ForwardVector_ = { 0,0,0 };//キャラクターから見た正面の方向(ワールド座標系) 自身のy軸回転量とかけて計算 正規化した値を入れる
        XMVECTOR MoveDirection_ = { 0,0,0 };//移動方向 この値に速さの要素をかけて移動ベクトル化する
        XMVECTOR NewPositon_ = { 0,0,0 };//移動後の位置ベクトル
		Transform ArrowTransform_;//チャージ/攻撃準備中の矢印のトランスフォーム
        XMFLOAT3 ArrowRotate_ = { 0,0,0 };//矢印の初期回転
        XMFLOAT3 ArrowScale_ = { 0,0,0 };//矢印の大きさ
		float AddArrowDepth_ = 0.0f;//矢印の奥行き(前方向)の調整値
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
        float Gravity_ = 0.0f; //重力 キャラクターの下方向にかかる力 9.8/60(1秒)より軽くしている
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
        float DecelerationRate_ = 0.0f;//ノックバック時の1fごとの減速率
        float KnockBackEnd_ = 0.0f;//ノックバックを終了する値
    };
    HitParam HitParam_;

    //----------壁の接触ダメージ----------
    struct WallHitParam
    {
        XMVECTOR UpperNormal_ = {0,0,0};//ステージ北端(前方)の法線ベクトル
        XMVECTOR LowerNormal_ = { 0,0,0 };//ステージ南端(後方)の法線ベクトル
        XMVECTOR RightNormal_ = { 0,0,0 };//ステージ東端(右側)の法線ベクトル
        XMVECTOR LeftNormal_ = { 0,0,0 };//ステージ西端(左側)の法線ベクトル
        std::vector<XMVECTOR> NormalArray_ = {};//各法線ベクトルを格納した配列
        std::vector<std::string> WireArray_ = { "UpperWire", "LowerWire", "RightWire" ,"LeftWire"};//各鉄線の名前の配列

        float KnockBackPower_ = 0.0f; //壁ヒットでノックバックする強さ（変化なし）
        int InvincibilityTime_ = 0;//ダメージ後の無敵時間 1fごとに上昇
        bool IsInvincibility_ = false;//無敵時間か
        int InvincibilityValue_ = 0;//無敵時間の値　この値を超えると無敵時間終了
        int blinkTimer_ = 0;//ダメージ後の点滅カウント
        int blinkValue_ = 0;//この値にblinkTimerが到達すると描画する
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
    std::vector<float> FullChargeParam = {};
	std::vector<float> AttackLocusParam_ = {};//突撃エフェクトのパラメータ
	std::vector<float> HitEffectParam_ = {};//接触時の衝撃エフェクトのパラメータ
	std::vector<float> WallHitEffectParam_ = {};//壁に接触時の衝撃エフェクトのパラメータ

    //----------サウンド関連----------
    int ChargeSoundCount_ = 0;//チャージ音を鳴らす回数
    int AttackSoundCount_ = 0;//突撃音を鳴らす回数

public:
    Character(GameObject* parent);
    Character(GameObject* parent, const std::string& name);

    virtual ~Character();

    //更新(継承先の共通処理のみ行う)
    void Update() override;

    //描画(継承先の共通描画のみ行う)
    void Draw() override;

    //----------初期化----------

    /// <summary>
    /// CSVから各ステータスを読み込みする
    /// Characterクラス限定
    /// </summary>
    /// <param name="_path">csvファイルのパス</param>
    void SetcsvStatus(std::string _path);

    /// <summary>
    /// 各方向の柵から法線ベクトルを取得しNormalArrayを初期化
    /// </summary>
    void GetWireNormal();

    /// <summary>
    /// 初期位置の設定
    /// </summary>
    void SetStartPosition() { this->transform_.position_ = InitParam_.StartPosition_; }

    /// <summary>
    /// 矢印トランスフォームの初期化
    /// </summary>
    void InitArrow();
  
    //----------描画----------

    /// <summary>
    /// キャラクターモデル描画(ダメージ時の点滅表現等行う)
    /// </summary>
    /// <param name="_handle">モデルハンドル</param>
    /// <param name="_transform">描画位置のトランスフォーム</param>
    void DrawCharacterModel(int _handle, Transform _transform);

    /// <summary>
    /// キャラクター共通のImGuiを描画 個別要素は派生先で記述
    /// </summary>
    void DrawCharacterImGui();

    //----------移動----------

    /// <summary>
    /// キャラクターの移動処理
    /// </summary>
    /// <param name="_direction">動かす方向ベクトル</param>
    void CharacterMove(XMVECTOR _direction);

    /// <summary>
    /// 移動ベクトルをつくる(方向ベクトルが必要)
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
    /// Y軸の回転行列をベクトルに変換
    /// </summary>
    /// <param name="rotY">Y軸に回転したい角度（Degree）</param>
    /// <param name="front">正面ベクトル(ローカル空間)</param>
    /// <returns>変形したベクトル（ワールド空間）</returns>
    XMVECTOR RotateVecFront(float rotY, XMVECTOR front);

    /// <summary>
    /// 正面ベクトルを更新
    /// </summary>
    void FrontVectorConfirm(){

        //ローカル正面ベクトルを現在のy軸回転量で変形すると、正面からどれだけ回転したかが計算される
        //その値がワールド正面ベクトルとなる
        MoveParam_.ForwardVector_ = RotateVecFront(this->transform_.rotate_.y, InitParam_.FrontDirection_);
    }

    /// <summary>
    /// 受け取ったベクトルからキャラクターの回転量を計算
    /// </summary>
    /// <param name="_MoveVector">進行したい方向ベクトル</param>
    /// <returns>回転する角度(Y軸回転)</returns>
    float RotateDirectionVector(XMVECTOR _MoveVector);

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

    //----------攻撃----------

    /// <summary>
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
    bool IsDashStop() { if (MoveParam_.Acceleration_ <= 0.0f) return true; else return false; }


    //----------空中----------

    /// <summary>
    /// 重力処理
    /// </summary>
    void CharacterGravity();

    //----------回転----------

    /// <summary>
    /// 通常X軸回転
    /// </summary>
    void MoveRotate() { this->transform_.rotate_.x += RotateParam_.MoveRotateX; }

    /// <summary>
    /// 通常X軸回転(-x回転)
    /// </summary>
    void MoveRotateReverse() { this->transform_.rotate_.x -= RotateParam_.MoveRotateX; }

    /// <summary>
    /// 高速X軸回転
    /// </summary>
    void FastRotate() { this->transform_.rotate_.x += RotateParam_.FastRotateX; }

    /// <summary>
    /// 高速X軸回転(-x回転)
    /// </summary>
    void FastRotateReverse() { this->transform_.rotate_.x -= RotateParam_.FastRotateX; }

    /// <summary>
    /// X回転を止める
    /// </summary>
    void RotateStop() { this->transform_.rotate_.x = 0.0f; }


    //----------被弾----------

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
    /// 指定された鉄線の名前に応じた各法線ベクトルを取得する
    /// </summary>
    /// <param name="_normal">キャラクターが接触した鉄線の名前</param>
    /// <returns>鉄線の名前に対応した法線ベクトル</returns>
    XMVECTOR HitNormal(std::string _normal);

    /// <summary>
    /// 壁に接触した際の計算処理　壁の法線で計算
    /// </summary>
    /// <param name="normal">反射される方向(接触した柵の法線ベクトル)</param>
    void WallReflect(XMVECTOR normal);

    /// <summary>
    /// ノックバック終了判定
    /// </summary>
    /// <returns>ノックバック速度が終了値以下か</returns>
    bool IsKnockBackEnd();

    /// <summary>
    /// ダメージ後の無敵時間の計算
    /// </summary>
    void InvincibilityTimeCalclation();

    //----------影付け----------

    /// <summary>
    /// 影モデルの初期化
    /// </summary>
    void InitShadow();

    /// <summary>
    /// 影付け（毎フレーム更新）
    /// </summary>
    void ShadowSet();

    /// <summary>
    /// 影モデル描画
    /// </summary>
    void ShadowDraw();


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
    /// 壁に接触時の衝撃エフェクトつける
    /// </summary>
    void SetWallHitEffect();

    //----------サウンド関係----------

    //CSVからパラメータを読み込みする
    void InitCSVSound();

    //----------セッター・ゲッター関数----------
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