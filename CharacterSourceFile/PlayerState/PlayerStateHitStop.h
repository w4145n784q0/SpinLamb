#pragma once
#include "BasePlayerState.h"

//ヒットストップを行う状態クラス
class PlayerStateHitStop :
    public BasePlayerState
{
public:
    void Enter(Player* _player) override;
    void Update(Player* _player) override;
    void Exit(Player* _player) override;
    std::string GetStateName() override { return "HitStop"; }
    bool IsHitStopState() const override;
};

