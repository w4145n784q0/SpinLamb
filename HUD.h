#pragma once
#include "Engine/GameObject.h"
#include"array"
#include"GameTimer.h"
#include"MiniMap.h"
#include"Easing.h"

//���ԕ\���E���S�E���[�h�\���Ȃǂ�UI��`�悷��N���X
//���W�Ȃǂ����������ɓǂ݂��݁A�w�����ꂽ��\������

namespace
{

	//�`�惂�[�h�i�󋵂ɉ����ĕ\��/��\����؂�ւ��������́j���w��
	enum DrawMode
	{
		S_BeforeStart = 0,//�X�^�[�g�O
		S_Ready,//"Ready?","Go!"���o��
		S_Playing,//�Q�[����
		S_Finish,//"Finish"���o��
		S_Practice,//�t���[�v���C��
		S_None,
	};

}

class HUD :
	public GameObject
{
private:
	//----------�摜�n���h��----------

	//�^�C�g���ɖ߂�(���K�V�[���p)
	int hBackTitleLogo_;

	//���K��(���K�V�[���p)
	int hPracticeNow_;

	//�Q�[���J�n�O�̊ȈՐ���
	int hGameExplanation_;

	//�Q�[���J�n"start!"���S(�o�g���V�[���p)
	int hStart_;

	//�Q�[���J�n"ready?"���S(�o�g���V�[���p)
	int hReady_;

	//�Q�[���J�n"go!"���S(�o�g���V�[���p)
	int hGo_;

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

	//�t�B�j�b�V���̃��S(�o�g���V�[���p)
	int hFinish_;

	//�}�b�v�S��
	int hMap_;

	//�v���C���[�̃A�C�R��
	int hFirstIcon_;

	//�G�̃A�C�R��
	int hSecondIcon_;

	//----------�Q�[���V�[����ԑJ��(��Ԃɂ��`����e��ύX)----------
	enum GameMode
	{
		BattlePreStart,//�o�g���V�[���J�n�O
		BattleInProgress,//�o�g���V�[���v���C��
		BattleEnd,//�o�g���V�[���I��
		Practice,//���K�V�[��
		Max,
	};
	GameMode GameModeHUD_;

	//�`�惂�[�h���i�[����ϐ�
	DrawMode DrawMode_;

	//----------�C���X�^���X----------
	//hud�����瑀�삷��ꍇ�̃^�C�}�[�N���X�|�C���^
	GameTimer* pGameTimer_;

	//hud�����瑀�삷��ꍇ�̃~�j�}�b�v�N���X�|�C���^
	MiniMap* pMiniMap_;

	//�v���C���[�̃X�R�A
	int FirstScore_;

	//CPU�̃X�R�A
	int SecondScore_;

	//----------DrawStart�ŌĂԏ�ԑJ��----------
	enum DrawStartMode
	{
		start_ready = 0,
		start_go,
		start_max
	};
	DrawStartMode DrawStart_;

	//	DrawReady()����DrawGo()�ɑJ��ۂ̃^�C�}�[
	int ReadyTimer_;

	//�C�[�W���O�g�p���̃J�E���^�[
	float EasingCount_ = 0;

public:
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

	//CSV�t�@�C������K�v�p�����[�^��ǂݍ��݂���
	void SetHUDCSV();

	//�`�悷��X�R�A�ݒ�
	void SetFirstScore(int score) { FirstScore_ = score; }
	void SetSecondScore(int score) { SecondScore_ = score; }

	//�e�|�C���^��ݒ�
	void SetTimerPointer(GameTimer* _gametimer) { pGameTimer_ = _gametimer; }
	void SetMiniMapPointer(MiniMap* _minimap) { pMiniMap_ = _minimap; }

	//�`�惂�[�h��ύX �����Ŏw�肵��drawmode��Draw()�ɂČĂ΂��
	void SetDrawMode(DrawMode _drawmode) { DrawMode_ = _drawmode; }

	//DrawReady()����DrawGo()�ɑJ�鎞�Ԑݒ�
	//�o���o���Ɏ����Ƃ�h������,BattleScene����w�����Đݒ�
	void SetReadyTimer(int _timer) { ReadyTimer_ = _timer; }

	//----------�`��֐�----------
	//�����͒��ڌĂ΂��ASetDrawMode�����,HUD�N���X��Draw����`��(��ɕ\��������̂͗�O)

	//���K���[�h��
	void DrawPracticeLogo();

	//�^�C�}�[
	void DrawTimer();

	//�Q�[���̊ȈՐ���
	void DrawExplanation();

	//�J�n��(Ready)�̃��S
	void DrawStartLogo();

	//�I�����S
	void DrawFinishLogo();

	//�~�j�}�b�v
	void DrawMiniMap();

	//�X�R�A�\��(BattleScene�Ŏg�p)
	void DrawScore();

private:
	//�ȉ��̕`��֐��͑��N���X����Ă΂��A��L�̕`��֐�����Ă�
	//Draw -> DrawStart -> DrawReady DrawGo

	void DrawReady();
	void DrawGo();
};

