#pragma once
#include "BasePlayerState.h"

//ダッシュ攻撃を行う状態クラス
class PlayerStateAttack :
    public BasePlayerState
{
public:
    void Enter(Player* _player) override;
    void Update(Player* _player) override;
    void Exit(Player* _player) override;
	std::string GetStateName() override { return "Attack"; }
};

