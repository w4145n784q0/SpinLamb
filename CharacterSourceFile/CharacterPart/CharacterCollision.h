#pragma once
#include "../../Engine/GameObject.h"
#include"CharacterParams.h"

class Character;

class CharacterCollision :
    public GameObject
{
private:
    //使用するパラメータ(CharacterParams)のポインタ
    CharacterParams* params_;

    //親クラス(Character)のポインタ
    Character* character_;
public:
    CharacterCollision(GameObject* parent);
    virtual ~CharacterCollision() = default;

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

    void CommonCollision(GameObject* target);

    bool IsHitCharacter(std::string _name);

    bool IsHitFence(std::string _name);

    bool IsInDamageState();

    void CharacterReflect(GameObject* target);

    void SomeFenceReflect(std::string wire);
};

