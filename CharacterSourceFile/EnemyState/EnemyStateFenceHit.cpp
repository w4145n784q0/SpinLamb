#include "EnemyStateFenceHit.h"
#include"../Enemy.h"

void EnemyStateFenceHit::Enter(Enemy* _enemy)
{
}

void EnemyStateFenceHit::Update(Enemy* _enemy)
{
	//ダメージを受ける柵と接触した状態 

	//ノックバックする
	_enemy->OnKnockBack();

	//ノックバックする速度が一定以下なら通常状態へ戻る
	if (_enemy->OnIsKnockBackEnd())
	{
		//ノックバック速度を0に戻しておく
		_enemy->OnKnockBackVelocityReset();

		//待機状態へ遷移
		_enemy->ChangeState(Enemy::S_Wait);

		//攻撃までの時間を再抽選(ノックバックした際も抽選し、頻繁に変わるようにする)
		_enemy->RandomAimReLottery();

		//加速度リセット(ノックバック終了時点でリセット)
		_enemy->OnAccelerationStop();
	}
}

void EnemyStateFenceHit::Exit(Enemy* _enemy)
{
    //状態遷移の際は一度x回転をストップ
    _enemy->OnRotateXStop();
}

bool EnemyStateFenceHit::IsUpdateInvincibility() const
{
    //柵に接触している状態は無敵時間の更新を行わない
    return false;
}

bool EnemyStateFenceHit::IsFenceHitState() const
{
    //柵に接触している状態であることを返す
    return true;
}
