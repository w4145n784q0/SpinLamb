#pragma once
#include"CharacterParams.h"
#include "../../Engine/GameObject.h"
class CharacterShadow :
    public GameObject
{
private:
    CharacterParams* params_;

public:
    CharacterShadow(GameObject* parent);
    virtual ~CharacterShadow() = default;

    void Initialize() override {};
    void Update() override {};
    void Draw() override {};
    void Release() override {};

    void SetParams(CharacterParams* _params) {
        params_ = _params;
    }

    //----------�e�t��----------

   /// <summary>
   /// �e���f���̏�����
   /// </summary>
    void InitShadow();

    /// <summary>
    /// �e�t���i���t���[���X�V�j
    /// </summary>
    void ShadowSet();

    /// <summary>
    /// �e���f���`��
    /// </summary>
    void ShadowDraw();
};

