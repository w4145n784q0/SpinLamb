#pragma once
#include"CharacterParams.h"
#include "../../Engine/GameObject.h"
class CharacterAir :
    public GameObject
{
public:

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

