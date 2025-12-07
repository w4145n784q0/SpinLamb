#pragma once
#include "BasePlayerState.h"

//キャラクターと接触し弾かれた状態クラス
class PlayerStateHit :
    public BasePlayerState
{
public:
    void Enter(Player* _player) override;
    void Update(Player* _player) override;
    void Exit(Player* _player) override;
    std::string GetStateName() override { return "Hit"; }
    bool IsHitState() const override;
};

