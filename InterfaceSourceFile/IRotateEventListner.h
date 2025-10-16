#pragma once
class IRotateEventListener {

    //��]�֘A�̃C�x���g�ʒm���s���C���^�[�t�F�[�X
public:
    virtual void OnMoveRotateX() = 0;
    virtual void OnFastRotateX() = 0;
    virtual void OnRotateXStop() = 0;
    virtual ~IRotateEventListener() = default;
};
