#pragma once
#include"Engine/Text.h"
#include "Engine/GameObject.h"
#include"Player.h"
#include"Enemy.h"

class BattleScene :
    public GameObject
{
private:

	//�C���X�^���X
	Text* pPlayerScore_;
	Text* pEnemyScore_;

	//----------�o�g���V�[���X�e�[�g----------
	enum Battle
	{
		BEFORE,//�J�n�O
		NOW,//�o�g����
		AFTER,//�I����
		MAX
	};
	Battle BattleState;

	//----------�T�E���h�n���h��----------

	//�o�g�����T�E���h
	int hBattleSound_;

	//�I���z�C�b�X���T�E���h
	int hWhistle_;

	//���݂̎���(1�b���ƂɌ���)
	int GameTime_;

	int PlayerScore_;
	int EnemyScore_;

public:
	//�R���X�g���N�^
	//�����Fparent  �e�I�u�W�F�N�g�iSceneManager�j
	BattleScene(GameObject* parent);

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

