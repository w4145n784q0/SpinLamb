#include "EnemyStateAttack.h"
#include"../Enemy.h"
#include"../../Engine/Audio.h"

void EnemyStateAttack::Enter(Enemy* _enemy)
{
}

void EnemyStateAttack::Update(Enemy* _enemy)
{
	//攻撃状態 正面の方向に移動

	//攻撃中のエフェクトを出す
	_enemy->vfx_->SetAttackLocusEffect();

	//正面ベクトルの方向に移動
	_enemy->movement_->CharacterAttackMove(_enemy->GetAutoAttackDirection());

	//進行方向に合わせてY軸を回転
	_enemy->RotateFromDirection(_enemy->GetAutoAttackDirection());

	//摩擦量分速度を減少
	_enemy->movement_->FrictionDeceleration();

	//高速X回転
	_enemy->rotate_->FastRotateX();

	//加速量が0になったら
	if (_enemy->movement_->IsAcceleStop())
	{
		//明示的に加速量を0にする
		_enemy->movement_->AccelerationStop();

		//ルートへ戻る
		_enemy->ChangeState(Enemy::S_Root);

		//攻撃までの時間を再抽選
		_enemy->RandomAimReLottery();
	}

	//攻撃SE再生
	Audio::Play(_enemy->GetParams()->SoundParam_.hSoundattack_);
}

void EnemyStateAttack::Exit(Enemy* _enemy)
{
	//状態遷移の際は一度x回転をストップ
	_enemy->rotate_->RotateXStop();
}
