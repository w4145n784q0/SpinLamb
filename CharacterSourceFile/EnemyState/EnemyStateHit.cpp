#include "EnemyStateHit.h"
#include"../Enemy.h"

void EnemyStateHit::Enter(Enemy* _enemy)
{
}

void EnemyStateHit::Update(Enemy* _enemy)
{
	//相手と接触した状態

	//ノックバックする
	_enemy->GetModuleHit()->KnockBack();

	//ノックバックする速度が一定以下なら通常状態へ戻る
	if (_enemy->GetModuleHit()->IsKnockBackEnd())
	{
		//ノックバック速度を0に戻しておく
		_enemy->GetModuleHit()->KnockBackVelocityReset();

		//ルートへ戻る
		_enemy->ChangeState(Enemy::S_Wait);

		//攻撃までの時間を再抽選(ノックバックした際も抽選し、頻繁に変わるようにする)
		_enemy->RandomAimReLottery();

		//ダッシュ中の速度リセット(ノックバック終了時点でリセット)
		_enemy->GetModuleMovement()->AccelerationStop();
	}
}

void EnemyStateHit::Exit(Enemy* _enemy)
{
	//状態遷移の際は一度x回転をストップ
	_enemy->GetModuleRotate()->RotateXStop();
}

bool EnemyStateHit::isHitState() const
{
	//被弾状態であることを返す
	return true;
}
