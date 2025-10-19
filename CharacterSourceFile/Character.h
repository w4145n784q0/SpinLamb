#pragma once
#include "../Engine/GameObject.h"

//���i���������w�b�_�t�@�C��
#include "CharacterPart/CharacterParams.h"
#include "CharacterPart/CharacterModelBlink.h"
#include "CharacterPart/CharacterVfx.h"
#include "CharacterPart/CharacterShadow.h"
#include "CharacterPart/CharacterAir.h"
#include "CharacterPart/CharacterForward.h"
#include "CharacterPart/CharacterMovement.h"
#include "CharacterPart/CharacterRotate.h"
#include "CharacterPart/CharacterCharge.h"
#include "CharacterPart/CharacterHit.h"
#include "CharacterPart/CharacterFence.h"
#include "CharacterPart/CharacterCsvLoader.h"
#include "CharacterPart/CharacterObserver.h"
#include"CharacterPart/CharacterDebugPanel.h"

//�C���^�[�t�F�[�X

#include"../InterfaceSourceFile/IVFXEventListener.h"
#include"../InterfaceSourceFile/IChargeEventListner.h"
#include"../InterfaceSourceFile/IRotateEventListner.h"
#include"../InterfaceSourceFile/IMovementEventListener.h"


//�v���C���[,�G�N���X�̋��ʎ����N���X
class Character :
    public GameObject, 
    public IVFXEventListener, public IChargeEventListener,
    public IRotateEventListener, public IMovementEventListener
{
public:
    //----------���W���[���Q----------

    CharacterModelBlink* blink_;
    CharacterVFX*        vfx_;
    CharacterShadow*     shadow_;
    CharacterAir*        air_;
    CharacterForward*    forward_;
    CharacterMovement*   movement_;
    CharacterRotate*     rotate_;
    CharacterCharge*     charge_;
    CharacterHit*        hit_;
    CharacterFence*      fence_;
    CharacterCsvLoader*  csvload_;
    CharacterObserver*   observer_;
    CharacterDebugPanel* debugpanel_;

protected:

    //----------���ʃp�����[�^----------

    CharacterParams* params_;

public:
    Character(GameObject* parent);
    Character(GameObject* parent, const std::string& name);

    virtual ~Character();

    //�X�V(�p����̋��ʏ����̂ݍs��)
    void Update() override;

    //�`��(�p����̋��ʕ`��̂ݍs��)
    void Draw() override;

    //�`���[�W���G�t�F�N�g���o���C�x���g
    void OnChargeVFX() override
    {
        vfx_->SetFullChargeEffect(); 
    }

    //�ő�`���[�W�G�t�F�N�g���o���C�x���g
    void OnFullChargeVFX() override
    {
        vfx_->SetFullChargeEffect();
    }

    //�O�ՃG�t�F�N�g���o���C�x���g
    void OnAttackLocusVFX() override
    {
        vfx_->SetAttackLocusEffect();
    }

    //�ڐG�G�t�F�N�g���o���C�x���g
    void OnHitVFX() override
    {
        vfx_->SetHitEffect();
    }

    //��ڐG�G�t�F�N�g���o���C�x���g
    void OnFenceHitVFX() override
    {
        vfx_->SetFenceHitEffect();
    }

    //�`���[�W��(TmpAccele_)��0�ɂ���C�x���g
    void OnChargeReset() override
    {
        charge_->ChargeReset();
    }

    //X����]���s���C�x���g
    void OnMoveRotateX() override
    {
        rotate_->MoveRotateX();
    }

    //����X����]���s���C�x���g
    void OnFastRotateX() override
    {
        rotate_->FastRotateX();
    }

    //X����]��~���s���C�x���g
    void OnRotateXStop() override
    {
        rotate_->RotateXStop();
    }

    //�ړ��m�蔻����s���C�x���g
    bool OnIsOutsideStage(DirectX::XMFLOAT3 _position) override
    {
        return movement_->IsOutsideStage(_position);
    }

    //�����x��0�ɂ���C�x���g
    void OnAccelerationStop() override
    {
        movement_->AccelerationStop();
    }

    //�ړ��m�菈�����s���C�x���g
    void OnMoveConfirm() override
    {
		movement_->MoveConfirm();
    }

    //���ʃp�����[�^�Q�̃Q�b�^�[�֐�
    CharacterParams& GetParams() {
        return *params_;
    }

};