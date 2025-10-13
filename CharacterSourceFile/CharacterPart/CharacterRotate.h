#pragma once
#include"CharacterParams.h"
#include"../../Engine/GameObject.h"

class CharacterRotate :
    public GameObject
{
public:
    //----------‰ñ“]----------

    /// <summary>
    /// ó‚¯æ‚Á‚½ƒxƒNƒgƒ‹‚©‚çƒLƒƒƒ‰ƒNƒ^[‚Ì‰ñ“]—Ê‚ğŒvZ
    /// </summary>
    /// <param name="_MoveVector">is‚µ‚½‚¢•ûŒüƒxƒNƒgƒ‹</param>
    /// <returns>‰ñ“]‚·‚éŠp“x(Y²‰ñ“])</returns>
    float RotateDirectionVector(XMVECTOR _MoveVector);

    /// <summary>
    /// ’ÊíX²‰ñ“]
    /// </summary>
    void MoveRotateX();

    /// <summary>
    /// ’ÊíX²‰ñ“](-x‰ñ“])
    /// </summary>
    void MoveRotateXReverse();

    /// <summary>
    /// ‚‘¬X²‰ñ“]
    /// </summary>
    void FastRotateX();

    /// <summary>
    /// ‚‘¬X²‰ñ“](-x‰ñ“])
    /// </summary>
    void FastRotateReverse();

    /// <summary>
    /// X‰ñ“]‚ğ~‚ß‚é
    /// </summary>
    void RotateXStop();

};

