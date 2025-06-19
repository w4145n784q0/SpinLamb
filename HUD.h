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
		S_Start,
		S_Playing,
		S_Finish,
		S_Practice,
		S_MiniMap,
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
	int hPlayerIcon_;

	//�G�̃A�C�R��
	int hEnemyIcon_;

	//----------�Q�[���V�[���X�e�[�g(��Ԃɂ��`����e��ύX)----------
	enum GameMode
	{
		BattlePreStart,//�o�g���V�[���J�n�O
		BattleInProgress,//�o�g���V�[���v���C��
		BattleEnd,//�o�g���V�[���I��
		Practice,//���K�V�[��
		Max,
	};
	GameMode GameModeHUD_;

	//----------�C���X�^���X----------
	GameTimer* pGameTimer_;//hud�����瑀�삷��ꍇ�̃^�C�}�[�N���X�|�C���^
	MiniMap* pMiniMap_;

	///Text* pPlayerScore_;
	//Text* pEnemyScore_;

	//�`�惂�[�h���i�[����ϐ�
	DrawMode DrawMode_;

	int PlayerScore_;
	int EnemyScore_;

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

	//CSV�ݒ�
	void SetHUDCSV();

	void SetPlayerScore(int score) { PlayerScore_ = score; }
	void SetEnemyScore(int score) { EnemyScore_ = score; }

	//�e�|�C���^��ݒ�
	void SetTimerPointer(GameTimer* _gametimer) { pGameTimer_ = _gametimer; }
	void SetMiniMapPointer(MiniMap* _minimap) { pMiniMap_ = _minimap; }

	//�`�惂�[�h��ύX �����Ŏw�肵��drawmode��Draw()�ɂČĂ΂��
	void SetDrawMode(DrawMode _drawmode) { DrawMode_ = _drawmode; }

	//----------�`��֐�----------
	//�����͒��ڌĂ΂��ASetDrawMode�����,HUD�N���X��Draw����`��(��ɕ\��������̂͗�O)

	//���K���[�h��
	void DrawPracticeLogo();

	//�^�C�}�[
	void DrawTimer();

	//�J�n���̃��S
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

