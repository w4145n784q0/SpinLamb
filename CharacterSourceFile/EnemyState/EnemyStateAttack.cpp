#include "EnemyStateAttack.h"
#include"../Enemy.h"
#include"../../Engine/Audio.h"

void EnemyStateAttack::Enter(Enemy* _enemy)
{
}

void EnemyStateAttack::Update(Enemy* _enemy)
{
	//攻撃状態 正面の方向に移動

	if (_enemy == nullptr)
	{
		return;
	}

	//攻撃中のエフェクトを出す
	_enemy->OnAttackLocusVFX(_enemy->GetPosition());

	//正面ベクトルの方向に移動
	_enemy->OnCharacterAttackMove(_enemy->GetAutoAttackDirection());

	//進行方向に合わせてY軸を回転
	_enemy->RotateFromDirection(_enemy->GetAutoAttackDirection());

	//摩擦量(攻撃摩擦量)分速度を減少
	_enemy->OnFrictionAttackDeceleration();

	//高速X回転
	_enemy->OnFastRotateX();

	//加速量が0になったら
	if (_enemy->OnIsAcceleStop())
	{
		//明示的に加速量を0にする
		_enemy->OnAccelerationStop();

		//攻撃終了後は待機状態にする
		_enemy->ChangeState(Enemy::S_Wait);

		//攻撃までの時間を再抽選
		_enemy->RandomAimReLottery();
	}

	//攻撃SE再生
	Audio::Play(_enemy->GetParams()->SoundParam_.hSoundattack_);
}

void EnemyStateAttack::Exit(Enemy* _enemy)
{
	if (_enemy == nullptr)
	{
		return;
	}

	//状態遷移の際は一度x回転をストップ
	_enemy->OnRotateXStop();
}

bool EnemyStateAttack::IsAttackState() const
{
	//攻撃状態であることを返す
	return true;
}