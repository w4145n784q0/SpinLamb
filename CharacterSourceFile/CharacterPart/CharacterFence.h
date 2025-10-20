#pragma once
#include"CharacterParams.h"
#include"../../Engine/GameObject.h"
#include "../../InterfaceSourceFile/IVFXEventListener.h"
#include"../../InterfaceSourceFile/IChargeEventListner.h"
#include"../../InterfaceSourceFile/IMovementEventListener.h"

//Character�N���X�̍�֌W�̏������s�����W���[��

class Character;

class CharacterFence :
    public GameObject
{    
private:
    //�e�N���X(Character)�̃|�C���^
    Character* character_;

    //�g�p����p�����[�^(CharacterParams)�̃|�C���^
    CharacterParams* params_;

    //VFX�C�x���g��ʒm���郊�X�i�[
    IVFXEventListener* VFXListener_;

    //�`���[�W�֘A�C�x���g��ʒm���郊�X�i�[
    IChargeEventListener* ChargeListener_;
  
    //�ړ��֘A�C�x���g��ʒm���郊�X�i�[
    IMovementEventListener* MovementListener_;
public:

    CharacterFence(GameObject* parent);
    virtual ~CharacterFence() = default;

    void Initialize() override {};
    void Update() override {};
    void Draw() override {};
    void Release() override {};

    //CharacterParams�̃Z�b�^�[�֐�
    void SetParams(CharacterParams* _params) {
        params_ = _params;
    }

    //�e�N���X(Character)�̃Z�b�^�[�֐�
    void SetCharacter(Character* _character) {
        character_ = _character;
    }

    //���X�i�[�̃Z�b�^�[�֐�
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

