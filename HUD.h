#pragma once
#include "Engine/GameObject.h"
#include"array"

class HUD :
    public GameObject
{
private:
	//----------�摜�n���h��----------

	//�^�C�g���ɖ߂�(���K�V�[���p)
	int hBackTitleLogo_;

	//���K��(���K�V�[���p)
	int hPracticeNow_;

	//�Q�[���J�n���S(�o�g���V�[���p)
	int hStart_;

	//��������(�o�g���V�[���p)
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

	//�i���o�[�n���h���̔z��
	std::array<int, 10> ArrayHandle_;

	//�t�B�j�b�V���̃��S(�o�g���V�[���p)
	int hFinish_;

	//std::array<int, 3> CountDownArray_;

	//----------�Q�[���V�[��(��Ԃɂ��`����e��ύX)----------
	enum GameMode
	{
		BattlePreStart,//�o�g���V�[���J�n�O
		BattleInProgress,//�o�g���V�[���v���C��
		BattleEnd,//�o�g���V�[���I��
		Practice,//���K�V�[��
		Max,
	};
	GameMode GameModeHUD_;

	//----------���ԕ`��----------

	//���ԋL�^
	int TimeNumber_;

	//���ԃJ�E���g(10�̈�)
	int Timeten_;

	//���ԃJ�E���g(10�̈�)
	int Timeone_;

	//�摜�`��p�g�����X�t�H�[��
	Transform logo_backtitle_;
	Transform logo_practice_;
	Transform logo_start_;
	Transform logo_Finish_;
	Transform TenTime_;
	Transform OneTime_;


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

	void SetCSV();
};

