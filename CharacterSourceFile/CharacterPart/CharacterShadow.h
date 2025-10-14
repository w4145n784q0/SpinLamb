#pragma once
#include"CharacterParams.h"
#include "../../Engine/GameObject.h"
class CharacterShadow :
    public GameObject
{
public:
    CharacterShadow(GameObject* parent);
    virtual ~CharacterShadow() = default;

    void Initialize() override {};
    void Update() override {};
    void Draw() override {};
    void Release() override {};


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

