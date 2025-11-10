#pragma once
#include "BaseEnemyState.h"

//ò‚ÉÚG‚µ‚½ó‘ÔƒNƒ‰ƒX
class EnemyStateFenceHit :
    public BaseEnemyState
{
public:
    void Enter(Enemy* _enemy) override;
    void Update(Enemy* _enemy) override;
    void Exit(Enemy* _enemy) override;
    std::string GetStateName() override { return "FenceHit"; }
    bool IsUpdateInvincibility() const override;
    bool IsFenceHitState() const override;
};

