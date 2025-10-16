#pragma once
#include"../../InterfaceSourceFile/IGameObserver.h"
#include"../../StageSourceFile/Ground.h"

//プレイヤー,敵クラスの共通パラメータのみを扱うファイル

    //----------ステージの端----------
    struct EndParam 
    {
        //キャラクターが持つ移動制限の値
        float NorthEnd_ = 0.0f;     //ステージ北端(前方)の位置
        float SouthEnd_ = 0.0f;     //ステージ南端(後方)の位置
        float EastEnd_ = 0.0f;      //ステージ東端(右側)の位置
        float WestEnd_ = 0.0f;      //ステージ西端(左側)の位置
    };
    //EndParam EndParam_;

    //----------サウンドハンドル----------
    struct SoundParam
    {

        int hSoundcharge_ = -1;     //チャージ音のハンドル
        int hSoundattack_ = -1;     //突撃音のハンドル
        int hSoundCollision_ = -1;  //接触音のハンドル
        int hSoundJump_ = -1;       //ジャンプ音のハンドル
    };
    //SoundParam SoundParam_;

    //----------初期状態----------
    struct InitializeParam
    {
        int CharacterID = -1;                   //ゲームに参加するキャラクターのid
        XMFLOAT3 StartPosition_ = { 0,0,0 };    //初期位置
        XMVECTOR FrontDirection_ = { 0,0,1 };   //正面の初期値(ローカル座標系) ここからどれだけ回転したか
        std::vector<IGameObserver*> observers;  //監視される対象の配列
    };
    //InitializeParam InitParam_;

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
   // MoveParam MoveParam_;

    //----------回転----------
    struct RotateParam
    {
        float MoveRotateX = 0.0f;   //移動時の1fの回転量
        float FastRotateX = 0.0f;   //(チャージ中など)高速回転中の1fの回転量
    };
    //RotateParam RotateParam_;

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
    //JumpParam JumpParam_;


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
        std::string AttackedName_ = "";             //接触した相手の名前
    };
    //HitParam HitParam_;

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
    //FenceHitParam FenceHitParam_;

    //----------影付け----------
    struct ShadowParam
    {
        int hShadow_ = -1;                  //影のモデルハンドル
        float ShadowHeight_ = 0.0f;         //影をつける高さ
        float ShadowCorrection_ = 0.0f;     //影の位置の調整値
        Transform ShadowTrans_;             //影の描画トランスフォーム
        Ground* pGround_ = nullptr;         //地面のインスタンス
    };
    //ShadowParam ShadowParam_;

    //----------エフェクト関連----------

    struct EffectParam
    {
        std::vector<float> ChargeParam_ = {};        //チャージ状態エフェクトのパラメータ
        std::vector<float> FullChargeParam = {};     //最大チャージ状態エフェクトのパラメータ
        std::vector<float> AttackLocusParam_ = {};   //突撃エフェクトのパラメータ
        std::vector<float> HitEffectParam_ = {};     //接触時の衝撃エフェクトのパラメータ
        std::vector<float> FenceHitEffectParam_ = {};//柵に接触時の衝撃エフェクトのパラメータ
    };
   // EffectParam EffectParam_;
