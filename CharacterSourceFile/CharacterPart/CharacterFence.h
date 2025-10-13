#pragma once
#include"CharacterParams.h"
#include"../../Engine/GameObject.h"
class CharacterFence :
    public GameObject
{    
public:
    //----------��֌W----------

    /// <summary>
    /// �e�����̍򂩂�@���x�N�g�����擾��NormalArray��������
    /// </summary>
    void GetWireNormal();

    /// <summary>
    /// ��ɐڐG�����ۂ̌v�Z�����@��̖@���Ōv�Z
    /// </summary>
    /// <param name="_normal">���˂�������(�ڐG������̖@���x�N�g��)</param>
    void FenceReflect(XMVECTOR _normal);

    /// <summary>
    /// �Ď�����Ώ�(�z��)�ɍ�Ƀq�b�g�������Ƃ�ʒm
    /// </summary>
    void NotifyFenceHit();

    /// <summary>
    /// �_���[�W��̖��G���Ԃ̌v�Z
    /// </summary>
    void InvincibilityTimeCalculation();
};

