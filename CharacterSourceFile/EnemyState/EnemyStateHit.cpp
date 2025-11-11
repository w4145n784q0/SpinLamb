#include "EnemyStateHit.h"
#include"../Enemy.h"

void EnemyStateHit::Enter(Enemy* _enemy)
{
}

void EnemyStateHit::Update(Enemy* _enemy)
{
	//相手と接触した状態

	//ノックバックする
	_enemy->hit_->KnockBack();

	//ノックバックする速度が一定以下なら通常状態へ戻る
	if (_enemy->hit_->IsKnockBackEnd())
	{
		//ノックバック速度を0に戻しておく
		_enemy->hit_->KnockBackVelocityReset();

		//ルートへ戻る
		_enemy->ChangeState(Enemy::S_Root);

		//攻撃までの時間を再抽選(ノックバックした際も抽選し、頻繁に変わるようにする)
		_enemy->RandomAimReLottery();

		//ダッシュ中の速度リセット(ノックバック終了時点でリセット)
		_enemy->movement_->AccelerationStop();
	}
}

void EnemyStateHit::Exit(Enemy* _enemy)
{
	//状態遷移の際は一度x回転をストップ
	_enemy->rotate_->RotateXStop();
}

bool EnemyStateHit::isHitState() const
{
	//被弾状態であることを返す
	return true;
}
