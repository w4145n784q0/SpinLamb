#pragma once
#include "../Engine/GameObject.h"
#include "../StageSourceFile/Ground.h"
#include"../InterfaceSourceFile/IGameObserver.h"

//プレイヤー,敵クラスの共通事項クラス
class Character :
    public GameObject
{
protected:

    //----------ステージの端----------
    //キャラクターが持つ移動制限の値
    float NorthEnd_ = 0.0f;     //ステージ北端(前方)の位置
    float SouthEnd_ = 0.0f;     //ステージ南端(後方)の位置
    float EastEnd_ = 0.0f;      //ステージ東端(右側)の位置
    float WestEnd_ = 0.0f;      //ステージ西端(左側)の位置

    //----------サウンドハンドル----------
    int hSoundcharge_ = -1;     //チャージ音のハンドル
    int hSoundattack_ = -1;     //突撃音のハンドル
    int hSoundCollision_ = -1;  //接触音のハンドル
    int hSoundJump_ = -1;       //ジャンプ音のハンドル

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
        float Velocity_ = 0.0f;                 //初速度 この速度に加速度が加算される
        float Acceleration_ = 0.0f;             //加速度 ダッシュ攻撃中に使用する値
        float TmpAccele_ = 0.0f;                //加速度上昇時に使う仮の値
        float AcceleValue_ = 0.0f;              //Acceleration_上昇時、1fあたりの増加量
        float FullAccelerate_ = 0.0f;           //加速度の最大
        float Friction_ = 0.0f;                 //摩擦係数(減速率) 1fあたりの減速量
        int hMoveArrow_ = -1;                   //チャージ中に表示する矢印モデル
        XMVECTOR ForwardVector_ = { 0,0,0 };    //キャラクターから見た正面の方向(ワールド座標系) 自身のy軸回転量とかけて計算 正規化した値を入れる
        XMVECTOR MoveDirection_ = { 0,0,0 };    //移動方向 この値に速さの要素をかけて移動ベクトル化する
        XMVECTOR NewPosition_ = { 0,0,0 };      //移動後の位置ベクトル
        Transform ArrowTransform_;              //チャージ/攻撃準備中の矢印のトランスフォーム
        XMFLOAT3 ArrowRotate_ = { 0,0,0 };      //矢印の初期回転
        XMFLOAT3 ArrowScale_ = { 0,0,0 };       //矢印の大きさ
        float AddArrowDepth_ = 0.0f;            //矢印の奥行き(前方向)の調整値
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
        float Gravity_ = 0.0f;          //重力 キャラクターの下方向にかかる力 実際の重力より(9.8/60 m/s)より軽くしている
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
        float OriginalRangeMin_ = 0.0f;             //変換元のノックバック量の最小値
        float OriginalRangeMax_ = 0.0f;             //変換元のノックバック量の最大値
        float ConvertedRangeMin_ = 0.0f;            //変換後のノックバック量の最小値
        float ConvertedRangeMax_ = 0.0f;            //変換後のノックバック量の最大値
        XMFLOAT3 KnockBack_Direction_ = { 0,0,0 };  //ノックバックする方向
        XMFLOAT3 KnockBack_Velocity_ = { 0,0,0 };   //ノックバックする速度
        float DecelerationRate_ = 0.0f;             //ノックバック時の1fごとの減速率
        float KnockBackEnd_ = 0.0f;                 //ノックバックを終了する値
        std::string AttackedName_ = "";              //接触した相手の名前
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

    std::vector<float> ChargeParam_ = {};        //チャージ状態エフェクトのパラメータ
    std::vector<float> FullChargeParam = {};     //最大チャージ状態エフェクトのパラメータ
    std::vector<float> AttackLocusParam_ = {};   //突撃エフェクトのパラメータ
    std::vector<float> HitEffectParam_ = {};     //接触時の衝撃エフェクトのパラメータ
    std::vector<float> FenceHitEffectParam_ = {};//柵に接触時の衝撃エフェクトのパラメータ

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
    void SetCSVStatus(std::string _path);

    /// <summary>
    /// 各方向の柵から法線ベクトルを取得しNormalArrayを初期化
    /// </summary>
    void GetWireNormal();

    /// <summary>
    /// 自身の位置を初期位置に設定
    /// </summary>
    void SetStartPosition() { this->transform_.position_ = InitParam_.StartPosition_; }

    /// <summary>
    /// 矢印トランスフォームの初期化
    /// </summary>
    void InitArrow();

    /// <summary>
    /// 自身を監視する対象を配列に追加
    /// BattleSceneでのみ行われる
    /// </summary>
    /// <param name="_observer">追加する監視対象</param>
    void AddObserver(IGameObserver* _observer);

    /// <summary>
    /// 自身を監視する対象を配列から削除
    /// </summary>
    /// <param name="_observer">削除する監視対象</param>
    void RemoveObserver(IGameObserver* _observer);

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
    /// <param name="_direction">動かす方向</param>
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
    /// <param name="_rotY">Y軸に回転したい角度（Degree）</param>
    /// <param name="_front">正面ベクトル(ローカル空間)</param>
    /// <returns>変形したベクトル（ワールド空間）</returns>
    XMVECTOR RotateVecFront(float _rotY, XMVECTOR _front);

    /// <summary>
    /// 正面ベクトルを更新
    /// </summary>
    void FrontVectorConfirm();

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

    /// <summary>
    /// 矢印モデルの描画
    /// </summary>
    void DrawArrow();

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

    /// <summary>
    /// ジャンプ開始
    /// </summary>
    void SetJump();

    //----------回転----------

    /// <summary>
    /// 通常X軸回転
    /// </summary>
    void MoveRotateX() { this->transform_.rotate_.x += RotateParam_.MoveRotateX; }

    /// <summary>
    /// 通常X軸回転(-x回転)
    /// </summary>
    void MoveRotateXReverse() { this->transform_.rotate_.x -= RotateParam_.MoveRotateX; }

    /// <summary>
    /// 高速X軸回転
    /// </summary>
    void FastRotateX() { this->transform_.rotate_.x += RotateParam_.FastRotateX; }

    /// <summary>
    /// 高速X軸回転(-x回転)
    /// </summary>
    void FastRotateReverse() { this->transform_.rotate_.x -= RotateParam_.FastRotateX; }

    /// <summary>
    /// X回転を止める
    /// </summary>
    void RotateXStop() { this->transform_.rotate_.x = 0.0f; }

    //----------被弾----------

    /// <summary>
    /// キャラクター同士の反射処理
    /// </summary>
    /// <param name="_myVector">自身の位置ベクトル</param>
    /// <param name="_targetVector">相手の位置ベクトル</param>
    /// <param name="_myVelocity">自身の加速度</param>
    /// <param name="_targetVelocity">相手の加速度</param>
    /// <param name="_attackName">攻撃したキャラクターの名前</param>
    void Reflect(XMVECTOR _myVector, XMVECTOR _targetVector, float _myVelocity, float _targetVelocity, 
        std::string _attackName);

    /// <summary>
    /// ノックバック中のY軸回転角の計算 ノックバック直前に行う
    /// </summary>
    /// <param name="_KnockBackVector">ノックバックする方向(正規化されていること前提)</param>
    void KnockBackAngleY(XMFLOAT3 _KnockBackVector);

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
    /// 柵に接触した際の計算処理　柵の法線で計算
    /// </summary>
    /// <param name="_normal">反射される方向(接触した柵の法線ベクトル)</param>
    void FenceReflect(XMVECTOR _normal);

    /// <summary>
    /// ノックバック終了判定
    /// </summary>
    /// <returns>ノックバック速度が終了値以下か</returns>
    bool IsKnockBackEnd();

    /// <summary>
    /// ノックバック速度を0に戻す
    /// </summary>
    void KnockBackVelocityReset() { HitParam_.KnockBack_Velocity_ = { 0,0,0 };}

    /// <summary>
    /// ダメージ後の無敵時間の計算
    /// </summary>
    void InvincibilityTimeCalculation();

    /// <summary>
    /// 監視する対象(配列)に柵にヒットしたことを通知
    /// </summary>
    void NotifyFenceHit();

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
    /// 柵に接触時の衝撃エフェクトつける
    /// </summary>
    void SetFenceHitEffect();

    //----------セッター・ゲッター関数----------

    //キャラクターの移動制限をセット
    void SetEnd(float _upper, float _lower, float _left, float _right) {
        NorthEnd_ = _upper;
        SouthEnd_ = _lower;
        WestEnd_ = _left;
        EastEnd_ = _right;
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