#pragma once
//#include"Engine/Text.h"
#include "Engine/GameObject.h"
#include"BaseScene.h"

#include"Player.h"
#include"Enemy.h"
#include"GameTimer.h"
#include"MiniMap.h"
#include"HUD.h"

//"�o�g��"����n�܂郂�[�h�V�[��
class BattleScene :
	public BaseScene
{
private:

	//----------�C���X�^���X----------
	Player* pPlayer_;
	Enemy* pEnemy_;

	HUD* pHUD_;

	//HUD�N���X�ɓn���|�C���^
	GameTimer* pGameTimer_;//BattleScene�����瑀�삷��ꍇ�̃^�C�}�[�N���X�|�C���^
	MiniMap* pMiniMap_;//HUD�N���X�ɓn���|�C���^


	//----------�o�g���V�[���X�e�[�g----------
	enum Battle
	{
		BEFORE,//�J�n�O
		NOW,//�o�g����
		AFTER,//�I����
		MAX
	};
	Battle BattleState_;

	//�^�C�g���摜
	int hBackScreen_;

	//----------�T�E���h�n���h��----------

	//�o�g�����T�E���h
	int hSoundBattle_;

	//�I���z�C�b�X���T�E���h
	int hSoundWhistle_;

	//�v���C���[�̃X�R�A
	int PlayerScore_;

	//�G�̃X�R�A
	int EnemyScore_;

public:
	BattleScene(GameObject* parent);
	~BattleScene();

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

	void UpdateActive() override;
	void UpdateTransition() override;

	void UpdateBattleBefore();
	void UpdateBattle();
	void UpdateBattleAfter();

	void PlusPlayerScore() { PlayerScore_++; }
	void PlusEnemyScore() { EnemyScore_++; }

	void SetCSVBattle();
};

