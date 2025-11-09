#pragma once
#include "BaseEnemyState.h"
class EnemyStateApproach :
    public BaseEnemyState
{
    void Enter(Enemy* _enemy) override;
    void Update(Enemy* _enemy) override;
    void Exit(Enemy* _enemy) override;
    std::string GetStateName() override { return "Approach"; }
};

