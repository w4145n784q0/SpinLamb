#pragma once
#include "BaseEnemyState.h"

//キャラクターと接触し弾かれた状態クラス
class EnemyStateHit :
    public BaseEnemyState
{
public:
    void Enter(Enemy* _enemy) override;
    void Update(Enemy* _enemy) override;
    void Exit(Enemy* _enemy) override;
    std::string GetStateName() override { return "Hit"; }
};

