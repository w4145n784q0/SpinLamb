#pragma once

class IChargeEventListener {

    //チャージエフェクトのイベント通知
public:
    virtual ~IChargeEventListener() = default;
    virtual void OnChargeEffect() = 0;
};