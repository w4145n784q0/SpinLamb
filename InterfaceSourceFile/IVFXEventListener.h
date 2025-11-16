#pragma once
#include <DirectXMath.h>
using namespace DirectX;

class IVFXEventListener {

    //エフェクト関連のイベント通知を行うインターフェース
public:
    virtual void OnFullChargeVFX(DirectX::XMFLOAT3 _pos) = 0;
    virtual void OnAttackLocusVFX(DirectX::XMFLOAT3 _pos) = 0;
    virtual void OnHitVFX(DirectX::XMFLOAT3 _pos) = 0;
    virtual void OnFenceHitVFX(DirectX::XMFLOAT3 _pos) = 0;
    virtual ~IVFXEventListener() = default;
};