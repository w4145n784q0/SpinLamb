#pragma once
#include"CharacterParams.h"
#include "../../Engine/GameObject.h"
class CharacterAir :
    public GameObject
{
public:
    CharacterAir(GameObject* parent);
    virtual ~CharacterAir() = default;

    void Initialize() override {};
    void Update() override {};
    void Draw() override {};
    void Release() override {};

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

