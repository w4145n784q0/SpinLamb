#pragma once
#include "BasePlayerState.h"

//Š®‘S‚É’â~‚µ‚Ä‚¢‚éó‘ÔƒNƒ‰ƒX
class PlayerStateStop :
    public BasePlayerState
{
public:
    void Enter(Player* _player) override;
    void Update(Player* _player) override;
    void Exit(Player* _player) override;
    std::string GetStateName() override { return "Stop"; }
};

