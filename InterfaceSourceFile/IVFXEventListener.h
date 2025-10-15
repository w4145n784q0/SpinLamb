#pragma once
class IVFXEventListener {

    //エフェクト関連のイベント通知を行うインターフェース
public:
    virtual void OnChargeVFX() = 0;
    virtual void OnFullChargeVFX() = 0;
    virtual void OnAttackLocusVFX() = 0;
    virtual void OnHitVFX() = 0;
    virtual void OnFenceHitVFX() = 0;
    virtual ~IVFXEventListener() = default;
};