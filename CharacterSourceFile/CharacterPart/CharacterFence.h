#pragma once
#include"CharacterParams.h"
#include"../../Engine/GameObject.h"
#include "../../InterfaceSourceFile/IVFXEventListener.h"
#include"../../InterfaceSourceFile/IChargeEventListner.h"
#include"../../InterfaceSourceFile/IMovementEventListener.h"

class CharacterFence :
    public GameObject
{    
private:

    IVFXEventListener* VFXListener_;
    IChargeEventListener* ChargeListener_;
    IMovementEventListener* MovementListener_;
public:

    //----------��֌W----------
    CharacterFence(GameObject* parent);
    virtual ~CharacterFence() = default;

    void Initialize() override {};
    void Update() override {};
    void Draw() override {};
    void Release() override {};

    // ���X�i�[���ォ��ݒ肷��
    void SetEventListener(IVFXEventListener* _VfxListener, 
        IChargeEventListener* _ChargeListener, IMovementEventListener* _MovementListener);

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

