#pragma once
#include "Engine/GameObject.h"
#include"array"
#include"GameTimer.h"

//���ԕ\���E���S�E���[�h�\���Ȃǂ�UI��`�悷��N���X
//���W�Ȃǂ����������ɓǂ݂��݁A�w�����ꂽ��\������

namespace
{
	
	enum DrawMode
	{
		S_StartLogo,
		S_Timer,
		S_FinishLogo,
		S_Practice,
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

	//�Q�[���J�n���S(�o�g���V�[���p)
	int hStart_;

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

	//���ԃN���X�̃C���X�^���X
	GameTimer* pGameTimer_;

	//�`�惂�[�h���i�[����ϐ�
	DrawMode DrawMode_;
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

	void SetCSV();

	//GameTimer�|�C���^��ݒ�
	void SetTimerPointer(GameTimer* _gametimer) { pGameTimer_ = _gametimer; }

	//�`�惂�[�h��ύX
	void SetDrawMode(DrawMode _drawmode) { DrawMode_ = _drawmode; }

	//----------�`��֐�----------

	void DrawPracticeLogo();
	void DrawTimer();
	void DrawStartLogo();
	void DrawFinishLogo();
};

