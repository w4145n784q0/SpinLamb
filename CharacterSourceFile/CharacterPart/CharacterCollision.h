#pragma once
#include "../../Engine/GameObject.h"
#include"CharacterParams.h"
class CharacterCollision :
    public GameObject
{
private:
    //使用するパラメータ(CharacterParams)のポインタ
    CharacterParams* params_;

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


    void HitCollision(GameObject* target);

    bool IsHitCharacter(std::string _name);

    bool IsHitFence(std::string _name);
};

