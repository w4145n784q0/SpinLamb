#pragma once
#include "BaseEnemyState.h"

//ヒットストップを行う状態クラス
class EnemyStateHitStop :
    public BaseEnemyState
{
public:
    void Enter(Enemy* _enemy) override;
    void Update(Enemy* _enemy) override;
    void Exit(Enemy* _enemy) override;
    std::string GetStateName() override { return "HitStop"; }
};

