#pragma once
#include "../../Engine/GameObject.h"
class CharacterAir :
    public GameObject
{
public:
    /// <summary>
    /// 重力処理
    /// </summary>
    void CharacterGravity();

    /// <summary>
    /// ジャンプ開始
    /// </summary>
    void SetJump();
};

