#pragma once
#include"Engine/Text.h"
#include "Engine/GameObject.h"
#include"Player.h"
#include"Enemy.h"
#include"GameTimer.h"
#include"HUD.h"

//"�o�g��"����n�܂郂�[�h�V�[��
class BattleScene :
    public GameObject
{
private:

	//�C���X�^���X
	Text* pPlayerScore_;
	Text* pEnemyScore_;

	HUD* pHUD_;
	GameTimer* pGameTimer_;//hud�����瑀�삷��ꍇ�̃^�C�}�[�N���X�|�C���^


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

	//���݂̎���(1�b���ƂɌ���)
	int GameTime_;

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

	void UpdateBattleBefore();
	void UpdateBattle();
	void UpdateBattleAfter();

	void PlusPlayerScore() { PlayerScore_++; }
	void PlusEnemyScore() { EnemyScore_++; }
	int GetGameTime() { return GameTime_; }
	void SetGameTime(int time) { GameTime_ = time; }

	void SetCSVBattle();
};

