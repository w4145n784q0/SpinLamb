#pragma once
#include"CharacterParams.h"
#include "../../Engine/GameObject.h"
class CharacterAir :
    public GameObject
{
public:

    //----------空中----------

    /// <summary>
    /// 重力処理
    /// </summary>
    void CharacterGravity();

    /// <summary>
    /// ジャンプ開始
    /// </summary>
    void SetJump();
};

