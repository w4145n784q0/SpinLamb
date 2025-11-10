#pragma once
#include "BasePlayerState.h"

//ˆÚ“®‚È‚Ç‚ğs‚¤’Êí‚Ìó‘ÔƒNƒ‰ƒX
class PlayerStateIdle :
    public BasePlayerState
{
public:
    void Enter(Player* _player) override;
    void Update(Player* _player) override;
    void Exit(Player* _player) override;
    std::string GetStateName() override { return "Idle"; }
};

