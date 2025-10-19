#pragma once
#include"CharacterParams.h"
#include"../../Engine/GameObject.h"

class CharacterRotate :
    public GameObject
{
private:
    CharacterParams* params_;

public:
    CharacterRotate(GameObject* parent);
    virtual ~CharacterRotate() = default;

    void Initialize() override {};
    void Update() override {};
    void Draw() override {};
    void Release() override {};

    void SetParams(CharacterParams* _params) {
        params_ = _params;
    }

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
    /// ‚‘¬X²‰ñ“]
    /// </summary>
    void FastRotateX();

    /// <summary>
    /// X‰ñ“]‚ğ~‚ß‚é
    /// </summary>
    void RotateXStop();
};

