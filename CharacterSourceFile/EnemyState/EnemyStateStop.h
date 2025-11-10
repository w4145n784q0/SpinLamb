#pragma once
#include "BaseEnemyState.h"

//Š®‘S‚É’â~‚µ‚Ä‚¢‚éó‘ÔƒNƒ‰ƒX
class EnemyStateStop :
    public BaseEnemyState
{
public:
    void Enter(Enemy* _enemy) override;
    void Update(Enemy* _enemy) override;
    void Exit(Enemy* _enemy) override;
    std::string GetStateName() override { return "Stop"; }
};

