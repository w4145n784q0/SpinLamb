#pragma once
class IRotateEventListener {

    //回転関連のイベント通知を行うインターフェース
public:
    virtual void OnMoveRotateX() = 0;
    virtual void OnFastRotateX() = 0;
    virtual void OnRotateXStop() = 0;
    virtual ~IRotateEventListener() = default;
};
