#pragma once
#include<string>
class Enemy;

class BaseEnemyState
{
public:
    virtual ~BaseEnemyState() = default;

    //状態に入った瞬間に呼ばれる
    virtual void Enter(Enemy* _enemy) {}

    //毎フレームの更新
    virtual void Update(Enemy* _enemy) = 0;

    //状態から抜ける瞬間に呼ばれる
    virtual void Exit(Enemy* _enemy) {}

    //状態名を取得(デバッグ用)
    virtual std::string GetStateName() { return ""; }
};

