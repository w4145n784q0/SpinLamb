#pragma once
class IChargeEventListener {

    //�`���[�W�֘A�̃C�x���g�ʒm���s���C���^�[�t�F�[�X
public:
    virtual void OnChargeReset() = 0;
    virtual ~IChargeEventListener() = default;
};