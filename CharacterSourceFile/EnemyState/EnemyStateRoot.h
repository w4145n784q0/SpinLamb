#pragma once
#include "BaseEnemyState.h"

//ここから次のステートに移る判断をする状態クラス
class EnemyStateRoot :
    public BaseEnemyState
{
public:
    void Enter(Enemy* _enemy) override;
    void Update(Enemy* _enemy) override;
    void Exit(Enemy* _enemy) override;
    std::string GetStateName() override { return "Root"; }

    bool TryApproach(Enemy* _enemy);
    bool TryAttack(Enemy* _enemy);
};

