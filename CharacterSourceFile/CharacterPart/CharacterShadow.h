#pragma once
#include"CharacterParams.h"
#include "../../Engine/GameObject.h"

class Character;

class CharacterShadow :
    public GameObject
{
private:
    Character* character_;
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

    void SetCharacter(Character* _character) {
        character_ = _character;
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

