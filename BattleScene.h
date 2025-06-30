#pragma once
#include "Engine/GameObject.h"
#include"BaseScene.h"

#include"Player.h"
#include"Enemy.h"
#include"GameTimer.h"
#include"MiniMap.h"
#include"HUD.h"

#include"TransitionEffect.h"

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
	//�����̒l�ɉ����ĊeUpdate�֐����Ăяo��
	enum Battle
	{
		S_BEFORE = 0,//�J�n�O����
		S_READY,//�J�n���O
		S_NOW,//�o�g����
		S_AFTER,//�I����
		MAXMODE
	};
	Battle BattleState_;

	//----------�摜�n���h��----------

	//�^�C�g���摜
	int hBackScreen_;

	//----------�T�E���h�n���h��----------

	//�o�g�����T�E���h
	int hSoundBattle_;

	//�J�n�E�I���z�C�b�X���T�E���h
	int hSoundWhistle_;

	//�v���C���[�̃X�R�A
	int PlayerScore_;

	//�G�̃X�R�A
	int EnemyScore_;

	//----------�C���X�^���X----------
	TransitionEffect* pTransitionEffect_;


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

	//----------BaseScene�̌p���֐�----------
	//�ʏ�̏���
	void UpdateActive() override;

	//�V�[���J�ڒ��̏���
	void UpdateTransition() override;

	//----------BattleState_�ɉ����ē��e���ς��Update�֐�----------
	//�o�g���J�n�O
	void UpdateBattleBefore();

	//�o�g���J�n���O
	void UpdateBattleReady();

	//�o�g����
	void UpdateBattle();

	//�o�g���I����
	void UpdateBattleAfter();

	//------------------------------

	//CSV����K�v�p�����[�^��ǂݍ��݂���
	void SetCSVBattle();

	//Player�̃X�R�A�㏸
	void PlusPlayerScore() { PlayerScore_++; }

	//CPU�̃X�R�A�㏸
	void PlusEnemyScore() { EnemyScore_++; }

};

