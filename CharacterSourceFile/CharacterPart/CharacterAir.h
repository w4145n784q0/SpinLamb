#pragma once
#include"CharacterParams.h"
#include "../../Engine/GameObject.h"

class Character;

class CharacterAir :
    public GameObject
{
private:
    Character* character_;
    CharacterParams* params_;

public:
    CharacterAir(GameObject* parent);
    virtual ~CharacterAir() = default;

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

    //----------��----------

    /// <summary>
    /// �d�͏���
    /// </summary>
    void CharacterGravity();

    /// <summary>
    /// �W�����v�J�n
    /// </summary>
    void SetJump();
};

