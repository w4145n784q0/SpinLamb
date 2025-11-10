#pragma once
#include "BaseEnemyState.h"

//‘Šè‚ğ‘_‚Á‚Ä‚¢‚éó‘ÔƒNƒ‰ƒX
class EnemyStateAim :
    public BaseEnemyState
{
public:
    void Enter(Enemy* _enemy) override;
    void Update(Enemy* _enemy) override;
    void Exit(Enemy* _enemy) override;
    void Draw(Enemy* _enemy) override;
    std::string GetStateName() override { return "Aim"; }
};

