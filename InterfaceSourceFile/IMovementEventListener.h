#pragma once
#include <DirectXMath.h>

class IMovementEventListener {

    //��]�֘A�̃C�x���g�ʒm���s���C���^�[�t�F�[�X
public:
    virtual bool OnIsOutsideStage(DirectX::XMFLOAT3 _position) = 0;
    virtual void OnAccelerationStop() = 0;
	virtual void OnMoveConfirm() = 0;
    virtual ~IMovementEventListener() = default;
};