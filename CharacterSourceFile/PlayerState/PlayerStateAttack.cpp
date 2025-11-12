#include "PlayerStateAttack.h"
#include "../Player.h"
#include "../../Engine/Audio.h"

void PlayerStateAttack::Enter(Player* _player)
{
}

void PlayerStateAttack::Update(Player* _player)
{
	//UŒ‚ó‘Ô ³–Ê‚Ì•ûŒü‚ÉˆÚ“®‚µ‘€ì•s‰Â

	//UŒ‚’†‚ÌƒGƒtƒFƒNƒg‚ðo‚·
	_player->vfx_->SetAttackLocusEffect();

	//³–ÊƒxƒNƒgƒ‹‚Ì•ûŒü‚ÉˆÚ“®
	_player->movement_->CharacterAttackMove(_player->GetParams()->MoveParam_.ForwardVector_);

	//–€ŽC—Ê(UŒ‚–€ŽC—Ê)•ª‘¬“x‚ðŒ¸­
	_player->movement_->FrictionAttackDeceleration();

	//‚‘¬X‰ñ“]
	_player->rotate_->FastRotateX();

	//‰Á‘¬—Ê‚ª0‚É‚È‚Á‚½‚ç
	if (_player->movement_->IsAcceleStop())
	{
		//–¾Ž¦“I‚É‰Á‘¬—Ê‚ð0‚É‚·‚é
		_player->movement_->AccelerationStop();

		//’Êíó‘Ô‚Ö–ß‚é
		_player->ChangeState(Player::S_Idle);
	}

	//UŒ‚SEÄ¶
	Audio::Play(_player->GetParams()->SoundParam_.hSoundattack_);
}

void PlayerStateAttack::Exit(Player* _player)
{
	//ó‘Ô‘JˆÚ‚ÌÛ‚Íˆê“xx‰ñ“]‚ðƒXƒgƒbƒv
	_player->rotate_->RotateXStop();
}
