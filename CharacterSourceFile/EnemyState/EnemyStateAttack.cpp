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
	_enemy->OnAttackLocusVFX(_enemy->GetPosition());

	//³–ÊƒxƒNƒgƒ‹‚Ì•ûŒü‚ÉˆÚ“®
	_enemy->OnCharacterAttackMove(_enemy->GetAutoAttackDirection());

	//is•ûŒü‚É‡‚í‚¹‚ÄYŽ²‚ð‰ñ“]
	_enemy->RotateFromDirection(_enemy->GetAutoAttackDirection());

	//–€ŽC—Ê(UŒ‚–€ŽC—Ê)•ª‘¬“x‚ðŒ¸­
	_enemy->OnFrictionAttackDeceleration();

	//‚‘¬X‰ñ“]
	_enemy->OnFastRotateX();

	//‰Á‘¬—Ê‚ª0‚É‚È‚Á‚½‚ç
	if (_enemy->OnIsAcceleStop())
	{
		//–¾Ž¦“I‚É‰Á‘¬—Ê‚ð0‚É‚·‚é
		_enemy->OnAccelerationStop();

		//UŒ‚I—¹Œã‚Í‘Ò‹@ó‘Ô‚É‚·‚é
		_enemy->ChangeState(Enemy::S_Wait);

		//UŒ‚‚Ü‚Å‚ÌŽžŠÔ‚ðÄ’Š‘I
		_enemy->RandomAimReLottery();
	}

	//UŒ‚SEÄ¶
	Audio::Play(_enemy->GetParams()->SoundParam_.hSoundattack_);
}

void EnemyStateAttack::Exit(Enemy* _enemy)
{
	//ó‘Ô‘JˆÚ‚ÌÛ‚Íˆê“xx‰ñ“]‚ðƒXƒgƒbƒv
	_enemy->OnRotateXStop();
}
