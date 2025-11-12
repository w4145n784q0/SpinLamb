#include "EnemyStateAttack.h"
#include"../Enemy.h"
#include"../../Engine/Audio.h"

void EnemyStateAttack::Enter(Enemy* _enemy)
{
}

void EnemyStateAttack::Update(Enemy* _enemy)
{
	//UŒ‚ó‘Ô ³–Ê‚Ì•ûŒü‚ÉˆÚ“®

	//UŒ‚’†‚ÌƒGƒtƒFƒNƒg‚ðo‚·
	_enemy->vfx_->SetAttackLocusEffect();

	//³–ÊƒxƒNƒgƒ‹‚Ì•ûŒü‚ÉˆÚ“®
	_enemy->movement_->CharacterAttackMove(_enemy->GetAutoAttackDirection());

	//is•ûŒü‚É‡‚í‚¹‚ÄYŽ²‚ð‰ñ“]
	_enemy->RotateFromDirection(_enemy->GetAutoAttackDirection());

	//–€ŽC—Ê(UŒ‚–€ŽC—Ê)•ª‘¬“x‚ðŒ¸­
	_enemy->movement_->FrictionAttackDeceleration();

	//‚‘¬X‰ñ“]
	_enemy->rotate_->FastRotateX();

	//‰Á‘¬—Ê‚ª0‚É‚È‚Á‚½‚ç
	if (_enemy->movement_->IsAcceleStop())
	{
		//–¾Ž¦“I‚É‰Á‘¬—Ê‚ð0‚É‚·‚é
		_enemy->movement_->AccelerationStop();

		//ƒ‹[ƒg‚Ö–ß‚é
		_enemy->ChangeState(Enemy::S_Root);

		//UŒ‚‚Ü‚Å‚ÌŽžŠÔ‚ðÄ’Š‘I
		_enemy->RandomAimReLottery();
	}

	//UŒ‚SEÄ¶
	Audio::Play(_enemy->GetParams()->SoundParam_.hSoundattack_);
}

void EnemyStateAttack::Exit(Enemy* _enemy)
{
	//ó‘Ô‘JˆÚ‚ÌÛ‚Íˆê“xx‰ñ“]‚ðƒXƒgƒbƒv
	_enemy->rotate_->RotateXStop();
}
