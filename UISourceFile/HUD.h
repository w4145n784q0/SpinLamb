#pragma once
#include "../Engine/GameObject.h"
#include"../UISourceFile/GameTimer.h"
#include"../UISourceFile/MiniMap.h"

//���ԕ\���E���S�E���[�h�\���Ȃǂ�UI��`�悷��N���X
//���W�Ȃǂ����������ɓǂ݂��݁A�w�����ꂽ��\������

namespace
{

	//�`�惂�[�h�i�󋵂ɉ����ĕ\��/��\����؂�ւ��������́j���w��
	//���N���X����SetDrawMode��ʂ��Ďw�����邽�߁Anamespace�ɐ錾(���̃N���X����͕ύX���Ȃ�)
	enum DrawMode
	{
		Mode_BeforeStart = 0,	//�X�^�[�g�O
		Mode_JustBefore,		//"Ready?","Go!"���o���A�Q�[���J�n���O
		Mode_Playing,			//�o�g���V�[���̃Q�[����
		Mode_PlayPause,			//�o�g���V�[���̃|�[�Y��
		Mode_Finish,			//"Finish"���o�� �Q�[���I��
		Mode_Practice,			//�t���[�v���C�̃Q�[����
		Mode_PracticePause,		//�t���[�v���C�̃|�[�Y��
		Mode_None,
	};

}

class HUD :
	public GameObject
{
private:
	//----------�摜�n���h��----------

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

	//�t�B�j�b�V���̃��S(�o�g���V�[���p)
	int hFinish_;

	//��ʕ����̘g
	int hSplitLine_;

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

	//�}�b�v�S��
	int hMap_;

	//�v���C���[1�̃A�C�R��
	int hFirstIcon_;

	//�G(CPU)�E�v���C���[2�̃A�C�R��
	int hSecondIcon_;

	//�|�[�Y���
	int hPauseMenu_;

	//�|�[�Y��ʂ̑I���A�C�R��
	int hPauseIcon_;

	//----------�X�R�A----------

	//�v���C���[1�̃X�R�A
	int FirstScore_;

	//CPU�E�v���C���[2�̃X�R�A
	int SecondScore_;

	//----------�C���X�^���X----------
	//hud�����瑀�삷��ꍇ�̃^�C�}�[�N���X�|�C���^
	GameTimer* pGameTimer_;

	//hud�����瑀�삷��ꍇ�̃~�j�}�b�v�N���X�|�C���^
	MiniMap* pMiniMap_;

	//----------��ԑJ��----------

	//�`�惂�[�h���i�[����ϐ�
	//���ڑ���͂����ASetDrawMode����ύX�����
	DrawMode DrawMode_;

	enum DrawStartMode
	{
		S_StartReady = 0,	//"Ready?"��\�����Ă�����
		S_StartGo,			//"Go!"��\�����Ă�����
		S_MaxStartMode,
	};
	DrawStartMode DrawStart_;//DrawStart�ŌĂԏ�ԑJ��

	//----------�C�[�W���O----------

	//DrawReady()����DrawGo()�ɑJ��ۂ̃^�C�}�[
	//���̃N���X���ł͒�`�������N���X�����������
	int ReadyTimer_;

	//�C�[�W���O�g�p���̃J�E���^�[
	float EasingCount_;



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

	//ImGui�̕`��
	void DrawImGui();

	//�S��ʕ`��
	void DrawFullScreen();

	//CSV�t�@�C������K�v�p�����[�^��ǂݍ��݂���
	void SetHUDCSV();

	//�`�悷��X�R�A�ݒ�
	void SetFirstScore(int score) { FirstScore_ = score; }
	void SetSecondScore(int score) { SecondScore_ = score; }

	//�e�|�C���^��ݒ�
	void SetTimerPointer(GameTimer* _gametimer) { pGameTimer_ = _gametimer; }
	void SetMiniMapPointer(MiniMap* _minimap) { pMiniMap_ = _minimap; }

	//�`�惂�[�h��ύX �����Ŏw�肵��_drawmode��Draw()�ɂČĂ΂��
	void SetDrawMode(DrawMode _drawmode) { DrawMode_ = _drawmode; }

	//DrawReady()����DrawGo()�ɑJ�鎞�Ԑݒ�
	//�o���o���Ɏ����Ƃ�h������,BattleScene����w�����Đݒ�
	void SetReadyTimer(int _timer) { ReadyTimer_ = _timer; }

	//�|�[�Y��ʑI���A�C�R���̈ʒu�ݒ�
	//HUD����͕ύX�����ɁABattleScene,Practice�V�[������Z�b�g
	void SetPauseIcon(float	_posY);

	//----------�ȉ��͌ʂɎw�肷��`��֐�----------

	//-----�ȉ��͒��ڌĂ΂��ASetDrawMode�����
	//�`�悷��(�V�[���ɂ���Đ؂�ւ����̂Ȃ�)-----

	//���K���[�h��
	void DrawPracticeLogo();

	//�^�C�}�[
	void DrawTimer();

	//�Q�[���̊ȈՐ���
	void DrawExplanation();

	//�J�n��(Ready,Go)�̃��S
	void DrawStartLogo();

	//�I��(Finish)���S
	void DrawFinishLogo();

	//�~�j�}�b�v
	void DrawMiniMap();

	//�X�R�A�\��
	void DrawScore();

	//�|�[�Y���
	void DrawPause();

	//-----�ȉ���HUD�̃|�C���^����`�悷��-----
	
	//��ʕ����̘g��
	void DrawSplitLine();

	//-----�ȉ���HUD�N���X���ŕ`�悷��-----

	//�^�C�}�[�̃C�[�W���O����
	void DrawTimerEasing();

	//----------ImGui�`��֐�(Imgui���O������Ă�)----------
	void DrawImGuiExplanation();
	void DrawImGuiStartLogo();
	void DrawImGuiFinishLogo();
	void DrawImGuiPracticeLogo();

	void DrawImGuiScore();
	void DrawImGuiTimer();
	void DrawImGuiMiniMap();
	void DrawImGuiPause();

private:
	//�ȉ��̕`��֐��͑��N���X����Ă΂��A��L�̕`��֐�����Ă�
	//Draw �� DrawStart �� DrawReady DrawGo

	void DrawReady();
	void DrawGo();
};

