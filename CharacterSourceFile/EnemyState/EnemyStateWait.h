#pragma once
#include "BaseEnemyState.h"

//待機状態 攻撃後のクールタイムなどに使用
class EnemyStateWait :
    public BaseEnemyState
{
public:
    void Enter(Enemy* _enemy) override;
    void Update(Enemy* _enemy) override;
    void Exit(Enemy* _enemy) override;
    std::string GetStateName() override { return "Wait"; }
};

