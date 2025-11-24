#pragma once
#include "BasePlayerState.h"

//ダッシュ攻撃前のチャージを行っている状態クラス
class PlayerStateCharge :
    public BasePlayerState
{
public:
    void Enter(Player* _player) override;
    void Update(Player* _player) override;
    void Exit(Player* _player) override;
    std::string GetStateName() override { return "Charge"; }
    void Draw(Player* _player) override;
};

