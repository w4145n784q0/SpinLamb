#include "EnemyStateRoot.h"
#include"../Enemy.h"

void EnemyStateRoot::Enter(Enemy* _enemy)
{
}

void EnemyStateRoot::Update(Enemy* _enemy)
{
	//“G‚Ìó‘Ô‘JˆÚ‚ÌÅãˆÊ UŒ‚‚â”í’eó‘Ô‚ªI‚í‚Á‚½‚ç‚±‚±‚É–ß‚é
	//‚±‚±‚©‚çŽŸ‚Ìó‘Ô‚Ö‘JˆÚ‚·‚é

	//Ž©g‚ÆPlayer‚Ì‹——£‚ð‘ª‚é
	float dist = _enemy->PlayerEnemyDistanceX();

	//ˆê’è‹——£ˆÈã—£‚ê‚Ä‚¢‚é‚È‚ç’ÇÕ
	if (dist > _enemy->GetChaseLength())
	{
		_enemy->ChangeState(Enemy::S_Approach);
	}
	else//Ú‹ß‚µ‚Ä‚¢‚é‚È‚çUŒ‚€”õ
	{
		_enemy->ChangeState(Enemy::S_Aim);
	}
}

void EnemyStateRoot::Exit(Enemy* _enemy)
{
	//ó‘Ô‘JˆÚ‚ÌÛ‚Íˆê“xx‰ñ“]‚ðƒXƒgƒbƒv
	_enemy->rotate_->RotateXStop();
}