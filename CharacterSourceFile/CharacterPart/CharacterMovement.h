#pragma once
#include"CharacterParams.h"
#include "../../Engine/GameObject.h"
#include"../../InterfaceSourceFile/IRotateEventListner.h"

//Characterクラスの移動関係の処理を行うモジュール

class Character;

class CharacterMovement :
    public GameObject
{
private:
    //親クラス(Character)のポインタ
    Character* character_;

    //使用するパラメータ(CharacterParams)のポインタ
    CharacterParams* params_;

    //回転関連イベントを通知するリスナー
    IRotateEventListener* RotateListener_;

public:
    CharacterMovement(GameObject* parent);
    virtual ~CharacterMovement() = default;

    void Initialize() override {};
    void Update() override {};
    void Draw() override {};
    void Release() override {};

    //CharacterParamsのセッター関数
    void SetParams(CharacterParams* _params) {
        params_ = _params;
    }

    //親クラス(Character)のセッター関数
    void SetCharacter(Character* _character) {
        character_ = _character;
    }

    // リスナーを後から設定する
    void SetEventListener(IRotateEventListener* _RotateListener);

    /// <summary>
    /// 自身の位置を初期位置に設定
    /// </summary>
    void InitStartPosition();
    
    /// <summary>
    /// 入力ベクトルを受け取り更新する
    /// </summary>
    /// <param name="_input">入力された方向ベクトル</param>
    void MoveUpdate(XMVECTOR _input);

    /// <summary>
    /// 加速度の追加
    /// </summary>
    void AddAcceleration();

    /// <summary>
    /// キャラクターのダッシュ攻撃処理
    /// </summary>
    /// <param name="_direction">動かす方向</param>
    void CharacterAttackMove(XMVECTOR _direction);

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
    /// 移動後の位置ベクトルを作り更新判定を行う
    /// </summary>
    void NewPositionConfirm();

    //----------攻撃----------

    /// <summary>
    /// 減速処理(加速時の増加量使用)
    /// </summary>
    void Deceleration();

    /// <summary>
    /// 通常時摩擦による減速処理
    /// </summary>
    void FrictionNormalDeceleration();

    /// <summary>
    /// 攻撃時摩擦による減速処理
    /// </summary>
    void FrictionAttackDeceleration();

    /// <summary>
    /// 加速度リセット
    /// </summary>
    void AccelerationStop();

    /// <summary>
    /// 停止判定
    /// </summary>
    /// <returns>加速量が0.0以下かどうか</returns>
    bool IsAcceleStop();

    /// <summary>
    /// 使用する画像の初期化
    /// </summary>
    void InitImage();

    /// <summary>
    /// 攻撃時のアニメーション更新
    /// </summary>
    void UpdateDashImage();

    /// <summary>
    /// 攻撃時のアニメーション描画
    /// </summary>
    void DrawDashImage();
};

