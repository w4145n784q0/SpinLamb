#pragma once
#include <DirectXMath.h>

class IMovementEventListener {

    //回転関連のイベント通知を行うインターフェース
public:
    virtual bool OnIsOutsideStage(DirectX::XMFLOAT3 _position) = 0;
    virtual void OnAccelerationStop() = 0;
	virtual void OnMoveConfirm() = 0;
    virtual ~IMovementEventListener() = default;
};