#pragma once
#include"CharacterParams.h"
#include "../../Engine/GameObject.h"
class CharacterAir :
    public GameObject
{
private:
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

