#pragma once
#include<string>
class Player;

//プレイヤーキャラクターの状態遷移を管理する基底クラス
class BasePlayerState
{
public:
    virtual ~BasePlayerState() = default;

    //状態に入った瞬間に呼ばれる
    virtual void Enter(Player* _player) {}

    //毎フレームの更新
    virtual void Update(Player* _player) = 0;

    //状態から抜ける瞬間に呼ばれる
    virtual void Exit(Player* _player) {}

    //ステートごとの描画処理
    virtual void Draw(Player* player) {}

    //状態名を取得(デバッグ用)
    virtual std::string GetStateName() { return ""; }

    //-----以下は専用の状態でオーバーライド-----

    //ヒットストップ状態かどうか返す
    virtual bool IsHitStopState() const { return false; }

    //被弾状態かどうか返す
    virtual bool isHitState() const { return false; }

    //無敵時間の更新を行うかどうか返す
    virtual bool IsUpdateInvincibility() const { return true; }

	//柵に接触状態かどうか返す
    virtual bool IsFenceHitState() const { return false; }
};

