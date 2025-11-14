#include "EnemyStateRoot.h"
#include"../Enemy.h"

void EnemyStateRoot::Enter(Enemy* _enemy)
{
}

void EnemyStateRoot::Update(Enemy* _enemy)
{
	//“G‚Ìó‘Ô‘JˆÚ‚ÌÅãˆÊ UŒ‚‚â”í’eó‘Ô‚ªI‚í‚Á‚½‚ç‚±‚±‚É–ß‚é
	//‚±‚±‚©‚çŽŸ‚Ìó‘Ô‚Ö‘JˆÚ‚·‚é

	//ƒvƒŒƒCƒ„[‚ªƒqƒbƒgƒXƒgƒbƒvE”í’eEò‚ÉÚGó‘ÔE–³“GŽžŠÔ‚È‚çŒ©‚é‚¾‚¯
	if (!_enemy->IsAttackDecision())
	{
		_enemy->ChangeState(Enemy::S_Look);
	}
	else
	{
		//‚»‚¤‚Å‚È‚¢‚È‚çUŒ‚‚©’ÇÕ‚Ì€”õ

		//Ž©g‚ÆPlayer‚Ì‹——£‚ð‘ª‚é
		float dist = _enemy->PlayerEnemyDistanceX();

		//‘ŠŽè‚ÆŽ©g‚ª‹ß‚¢‚È‚çUŒ‚€”õ
		if (_enemy->IsNearChaseLength(dist))
		{
			_enemy->ChangeState(Enemy::S_Aim);

		}
		else//—£‚ê‚Ä‚¢‚é‚È‚ç’ÇÕ
		{
			_enemy->ChangeState(Enemy::S_Approach);
		}
	}
}

void EnemyStateRoot::Exit(Enemy* _enemy)
{
	//ó‘Ô‘JˆÚ‚ÌÛ‚Íˆê“xx‰ñ“]‚ðƒXƒgƒbƒv
	_enemy->rotate_->RotateXStop();
}

bool EnemyStateRoot::TryApproach(Enemy* _enemy)
{
	//Ž©g‚ÆPlayer‚Ì‹——£‚ð‘ª‚é
	float dist = _enemy->PlayerEnemyDistanceX();

	if (_enemy->IsNearChaseLength(dist))
	{
		_enemy->ChangeState(Enemy::S_Aim);
		return true;
	}
	return false;
}

bool EnemyStateRoot::TryAttack(Enemy* _enemy)
{
	//Ž©g‚ÆPlayer‚Ì‹——£‚ð‘ª‚é
	float dist = _enemy->PlayerEnemyDistanceX();

	if (!_enemy->IsNearChaseLength(dist))
	{
		_enemy->ChangeState(Enemy::S_Approach);
		return true;
	}
	return false;
}
