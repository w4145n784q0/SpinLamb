#pragma once
#include "../../Engine/GameObject.h"
class CharacterShadow :
    public GameObject
{
public:
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

