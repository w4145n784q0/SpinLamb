#pragma once
#include"CharacterParams.h"
#include "../../Engine/GameObject.h"

class Character;

class CharacterMovement :
    public GameObject
{
private:
    Character* character_;
    CharacterParams* params_;

public:
    CharacterMovement(GameObject* parent);
    virtual ~CharacterMovement() = default;

    void Initialize() override {};
    void Update() override {};
    void Draw() override {};
    void Release() override {};

    void SetParams(CharacterParams* _params) {
        params_ = _params;
    }

    void SetCharacter(Character* _character) {
        character_ = _character;
    }

    //----------移動----------

    /// <summary>
    /// 自身の位置を初期位置に設定
    /// </summary>
    void InitStartPosition();

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

    //----------攻撃----------

    /// <summary>
    /// 減速処理(加速時の増加量使用)
    /// </summary>
    void Deceleration();

    /// <summary>
    /// 摩擦による減速処理
    /// </summary>
    void FrictionDeceleration();

    /// <summary>
    /// 加速度リセット
    /// </summary>
    void AccelerationStop();

    /// <summary>
    /// 停止判定
    /// </summary>
    /// <returns>加速量が0.0以下かどうか</returns>
    bool IsDashStop();

};

