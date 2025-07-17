#pragma once
#include "Engine/GameObject.h"
#include"BaseScene.h"
#include"TransitionEffect.h"

#include<array>
#include<list>

//�Q�[�����[�h��I�����郁�j���[��ʃV�[��
class GameModeScene :
    public BaseScene
{
private:

	//���ݑI�����Ă��郂�[�h
	enum SelectMode
	{
		S_Battle = 0,//"�o�g��"��I��
		S_Practice,//"�t���[�v���C"��I��
		S_HowToPlay,//"�����т���"��I��
		S_Title,//"�^�C�g��"��I��
		MaxCount //���[�h�̍ő�l
	};
	SelectMode SelectMode_;

	//�Q�[�����[�h�V�[���̏��
	enum GameModeState
	{
		S_Selecting,//���[�h�̑I��
		S_Confirmation,//�����̊m�F���(�K�v�ɉ����Ď���)
		MaxState
	};
	GameModeState GameModeState_;

	//----------�w�i----------

	//�w�i�摜
	int hBackScreen_;

	//�w�i�L�����摜
	int hBackChara_;

	//�w�i�̃L�����N�^�[�̃g�����X�t�H�[��
	Transform BackChara_;

	//----------�{�^��----------
	//�o�g���X�^�[�g�{�^���摜
	int hBattle_;

	//���K���[�h�{�^���摜
	int hPractice_;

	//�����т��������{�^���摜
	int hHowtoPlay_;

	//�^�C�g���{�^���摜
	int hBackTitle_;

	//�I��g�̘g���摜
	int hFrameLine_;

	//�I��g�̃g�����X�t�H�[��
	Transform TransFrame_;

	//�e�{�^���̃g�����X�t�H�[���Ǘ��z��
	//csv�ǂݍ��ݎ��ɏ�����
	std::array<Transform, MaxCount> ModeTransArray_;

	//�e�{�^���̉摜�n���h���z��
	std::array<int, MaxCount> ButtonImageArray_;
	

	//----------���S�E�e�L�X�g----------

	//���[�h�Z���N�g�摜
	int hModeSelect_;

	//��ʉ����̃Q�[�������e�L�X�g�摜(�o�g���p)
	int hBattleText_;

	//��ʉ����̃Q�[�������e�L�X�g�摜(�t���[�v���C�p)
	int hFreePlayText_;

	//��ʉ����̃Q�[�������e�L�X�g�摜(�����т����p)
	int hHowtoPlayText_;

	//��ʉ����̃Q�[�������e�L�X�g�摜(�^�C�g���p)
	int hTitleText_;

	//��l�v���C����l�v���C�����m�F����摜
	int hPlayerNumSelect_;

	//"���[�h�Z���N�g"�摜�̃g�����X�t�H�[��
	Transform TransSelect_;

	//�Q�[�������e�L�X�g�̌Œ�ʒu
	Transform TransText_;

	//�e�e�L�X�g�̉摜�n���h���z��
	std::array<int, MaxCount> TextArray_;

	//----------�T�E���h�n���h��----------

	//�Q�[�����[�h�V�[����BGM
	int hSoundGameMode_;

	//���[�h�I������SE
	int hSoundSelect_;

	//���[�h���莞��SE
	int hSoundDecide_;


	//�e���[�h�̃��X�g
	std::list<SelectMode> ModeList_;

	//ModeList_�̃C���f�b�N�X���w���l
	std::list<SelectMode>::iterator itr;
	

	//----------�C���X�^���X----------
	TransitionEffect* pTransitionEffect_;
	
public:
	GameModeScene(GameObject* parent);
	~GameModeScene();

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

	//CSV�t�@�C������K�v�p�����[�^��ǂݍ��݂���
	void SetGameModeSCV();

	//----------GameModeState_�ɉ����ē��e���ς��Update�֐�----------
	void UpdateSelecting();

	void UpdateConfirmation();

	//----------BaseScene�̌p���֐�----------
	//�ʏ�̏���
	void UpdateActive() override;

	//�V�[���J�ڒ��̏���
	void UpdateTransition() override;
};

