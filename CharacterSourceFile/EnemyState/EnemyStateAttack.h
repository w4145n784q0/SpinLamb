#pragma once
#include "BaseEnemyState.h"
class EnemyStateAttack :
    public BaseEnemyState
{
public:
    void Enter(Enemy* _enemy) override;
    void Update(Enemy* _enemy) override;
    void Exit(Enemy* _enemy) override;
    std::string GetStateName() override { return "Attack"; }
};

