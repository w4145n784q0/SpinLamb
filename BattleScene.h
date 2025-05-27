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
	Text* pTime_;
	Text* pPlayerScore_;
	Text* pEnemyScore_;

	enum Battle
	{
		BEFORE,
		NOW,
		AFTER,
		MAX
	};
	Battle BattleState;

	//���n���h��
	int hBattleSound_;
	int hWhistle_;

	//��������
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

	void DrawBattleBefore();
	void DrawBattle();
	void DrawBattleAfter();



	void PlusPlayerScore() { PlayerScore_++; }
	void PlusEnemyScore() { EnemyScore_++; }
	int GetGameTime() { return GameTime_; }

};

