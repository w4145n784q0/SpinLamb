#pragma once

class IChargeEventListener {

    //�`���[�W�G�t�F�N�g�̃C�x���g�ʒm
public:
    virtual ~IChargeEventListener() = default;
    virtual void OnChargeEffect() = 0;
};