#pragma once
class IChargeEventListener {

    //チャージ関連のイベント通知を行うインターフェース
public:
    virtual void OnChargeReset() = 0;
    virtual ~IChargeEventListener() = default;
};