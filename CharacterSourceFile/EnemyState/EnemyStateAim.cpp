#include "EnemyStateAim.h"
#include"../Enemy.h"

namespace
{
	std::string EnemyChargeEffectPath = "ParticleAssets\\circle_R.png";
}

void EnemyStateAim::Enter(Enemy* _enemy)
{
}

void EnemyStateAim::Update(Enemy* _enemy)
{
	//チャージ(TmpAcceleを溜めている状態) しながらPlayerを狙う状態

	//プレイヤーのいる方向へY回転する
	_enemy->LookPlayer();

	//進行方向に合わせてY軸を回転
	_enemy->RotateFromDirection(_enemy->GetAutoAttackDirection());

	//加速度を溜める
	_enemy->GetModuleCharge()->Charging();

	//矢印モデルをセット
	_enemy->GetModuleCharge()->SetArrow();

	//矢印モデルの位置を自身の回転と合わせる
	_enemy->GetParams()->MoveParam_.ArrowTransform_.rotate_.y = _enemy->GetRotate().y;

	//チャージ中のエフェクトを出す
	_enemy->GetModuleVFX()->SetChargingEffect(EnemyChargeEffectPath, _enemy->GetPosition());

	//高速X回転
	_enemy->GetModuleRotate()->FastRotateX();

	//攻撃までのタイマーを進める
	_enemy->AimTimerAdd();

	//時間経過で攻撃状態へ（配列中のランダムな時間）
	if (_enemy->IsTimeOverAttackTime())
	{
		//攻撃までのタイマーをリセット
		_enemy->AimTimerReset();

		//チャージ解放
		_enemy->GetModuleCharge()->ChargeRelease();

		//攻撃状態へ移行
		_enemy->ChangeState(Enemy::S_Attack);
	}
}

void EnemyStateAim::Exit(Enemy* _enemy)
{
	//状態遷移の際は一度x回転をストップ
	_enemy->GetModuleRotate()->RotateXStop();
}

void EnemyStateAim::Draw(Enemy* _enemy)
{
	//矢印モデルの描画
	_enemy->GetModuleCharge()->DrawArrow();
}
