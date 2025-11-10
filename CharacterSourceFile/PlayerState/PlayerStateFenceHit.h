#pragma once
#include "BasePlayerState.h"

//ò‚ÉÚG‚µ‚½ó‘ÔƒNƒ‰ƒX
class PlayerStateFenceHit :
    public BasePlayerState
{
public:
    void Enter(Player* _player) override;
    void Update(Player* _player) override;
    void Exit(Player* _player) override;
    std::string GetStateName() override { return "FenceHit"; }
    bool IsUpdateInvincibility() const override; 
    bool IsFenceHitState() const override;
};

