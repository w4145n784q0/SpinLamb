#pragma once
#include "BasePlayerState.h"
class PlayerStateCharge :
    public BasePlayerState
{
public:
    void Enter(Player* _player) override;
    void Update(Player* _player) override;
    void Exit(Player* _player) override;
    std::string GetStateName() override { return "Charge"; }
    void Draw(Player* player) override;
};

