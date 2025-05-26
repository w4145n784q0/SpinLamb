#pragma once
#include "Engine/GameObject.h"
#include"array"

class HUD :
    public GameObject
{
private:
	//----------�摜�n���h��----------

	//�^�C�g���ɖ߂�(���K���[�h�p)
	int hBackTitleLogo_;

	//���K��(���K���[�h�p)
	int hPracticeNow_;

	//�Q�[���J�n���S
	int hStart_;

	//��������
	int hTime_;

	//�i���o�[
	int hNumber0_;
	int hNumber1_;
	int hNumber2_;
	int hNumber3_;
	int hNumber4_;
	int hNumber5_;
	int hNumber6_;
	int hNumber7_;
	int hNumber8_;
	int hNumber9_;
	std::array<int, 10> ArrayHandle_;

	//�t�B�j�b�V���̃��S
	int hFinish_;

	//std::array<int, 3> CountDownArray_;

	enum GameMode
	{
		BattlePreStart,
		BattleInProgress,
		BattleEnd,
		Practice,
		Max,
	};
	GameMode GameModeHUD_;

	//���ԕ`��
	int TimeNumber_;
	int Timeten_;
	int Timeone_;

	//score�`��
	int PlayerScore_;
	int EnemyScore_;

	int PlayerOne_;
	int PlayerTen_;
	int EnemyOne_;
	int EnemyTen_;

public:
	//�R���X�g���N�^
	//�����Fparent  �e�I�u�W�F�N�g�iSceneManager�j
	HUD(GameObject* parent);

	~HUD();

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;


	void UpdateBattlePreStart();
	void UpdateBattleInProgress();
	void UpdateBattleEnd();
	void UpdatePractice();

	void DrawBattlePreStart();
	void DrawBattleInProgress();
	void DrawBattleEnd();
	void DrawPractice();

	void SetStateBattle() { GameModeHUD_ = BattlePreStart; }
	void SetStateBattleInProgress() { GameModeHUD_ = BattleInProgress; }
	void SetStateBattleEnd(){ GameModeHUD_ = BattleEnd; }
	void SetStatePractice() { GameModeHUD_ = Practice; }

	//void SetNumber(int _num) { countDownNumber_ = _num; }
	
	void SetTime(int _time) { TimeNumber_ = _time; }
	void SetPlayerScore(int _score) { PlayerScore_ = _score; }
	void SetEnemyScore(int _score) {EnemyScore_ = _score; }
};

