#pragma once
#include "BaseEnemyState.h"
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

