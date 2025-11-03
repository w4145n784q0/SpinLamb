#pragma once
#include "BasePlayerState.h"
class PlayerStateIdle :
    public BasePlayerState
{
public:
    void Enter(Player* player) override;
    void Update(Player* player) override;
    void Exit(Player* player) override;
};

