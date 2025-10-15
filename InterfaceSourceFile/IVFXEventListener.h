#pragma once
class IVFXEventListener {

    //�G�t�F�N�g�֘A�̃C�x���g�ʒm���s���C���^�[�t�F�[�X
public:
    virtual void OnChargeVFX() = 0;
    virtual void OnFullChargeVFX() = 0;
    virtual void OnAttackLocusVFX() = 0;
    virtual void OnHitVFX() = 0;
    virtual void OnFenceHitVFX() = 0;
    virtual ~IVFXEventListener() = default;
};