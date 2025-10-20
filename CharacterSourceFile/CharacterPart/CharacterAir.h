#pragma once
#include"CharacterParams.h"
#include "../../Engine/GameObject.h"

//Characterクラスの空中関係の処理を行うモジュール

class Character;

class CharacterAir :
    public GameObject
{
private:
    //親クラス(Character)のポインタ
    Character* character_;

    //使用するパラメータ(CharacterParams)のポインタ
    CharacterParams* params_;

public:
    CharacterAir(GameObject* parent);
    virtual ~CharacterAir() = default;

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
    /// 重力処理
    /// </summary>
    void CharacterGravity();

    /// <summary>
    /// ジャンプ開始
    /// </summary>
    void SetJump();
};

