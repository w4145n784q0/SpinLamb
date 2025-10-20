#pragma once
#include"CharacterParams.h"
#include"../../Engine/GameObject.h"

//Characterクラスの回転関係の処理を行うモジュール

class Character;

class CharacterRotate :
    public GameObject
{
private:
    //親クラス(Character)のポインタ
    Character* character_;

    //使用するパラメータ(CharacterParams)のポインタ
    CharacterParams* params_;

public:
    CharacterRotate(GameObject* parent);
    virtual ~CharacterRotate() = default;

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

    /// <summary>
    /// 受け取ったベクトルからキャラクターの回転量を計算
    /// </summary>
    /// <param name="_MoveVector">進行したい方向ベクトル</param>
    /// <returns>回転する角度(Y軸回転)</returns>
    float RotateDirectionVector(XMVECTOR _MoveVector);

    /// <summary>
    /// 通常X軸回転
    /// </summary>
    void MoveRotateX();

    /// <summary>
    /// 高速X軸回転
    /// </summary>
    void FastRotateX();

    /// <summary>
    /// X回転を止める
    /// </summary>
    void RotateXStop();
};

