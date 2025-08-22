#pragma once
#include "BaseScene.h"
#include"../UISourceFile/HUD.h"
#include<array>

//Battle,Practice�V�[���̊��N���X
//�����̃V�[���ŋ��ʂ��鏈�����L�q
class PlayScene :
    public BaseScene
{
protected:
	//----------�摜�n���h��----------

	//�w�i
	int hBackScreen_;

	//----------�T�E���h�n���h��----------

	//�|�[�Y��ʂ��J�����E������
	int hSoundPause_;

	//�I������SE
	int hSoundSelect_;

	//�|�[�Y��ʂ���Q�[�����I��������
	int hSoundExit_;

	//----------�g�����X�t�H�[��----------
 
	//�w�i�̃g�����X�t�H�[��
	Transform TransBackScreen_;

	//----------�I����Ԃ��Ǘ����郊�X�g----------

	//�|�[�Y��ʂ̂Ƃ��ɑI�����Ă�����
	enum PauseSelect
	{
		S_Continue = 0,		//������
		S_Exit,				//��߂�
		MaxPauseSelect
	};
	PauseSelect PauseSelect_;

	//�|�[�Y��ʂ̑I����Ԃ̃��X�g
	std::list<PauseSelect> PauseSelectList_;

	// PauseSelectList_�̃C���f�b�N�X���w���l
	std::list<PauseSelect>::iterator Pauseitr;

	//�|�[�Y��ʂ̑I���A�C�R���̈ʒu�����ɕۊǂ���
	float TmpIconPosY_;

	//�|�[�Y��ʂ̑I���A�C�R���̈ʒu�z��
	//PauseSelect�̗v�f�����g���A���ꂼ��̐��l���I�𒆂̃A�C�R����Y�ʒu�ƂȂ�
	std::array<float, MaxPauseSelect> IconPosYArray;

public:
	PlayScene(GameObject* parent);
	PlayScene(GameObject* parent, const std::string& name);
	virtual ~PlayScene();

	//������
	void Initialize() override;

	//CSV�t�@�C������K�v�p�����[�^��ǂݍ��݂���
	void SetPlaySceneCSV();

	//BaseScene�̌p���֐������A����ɂ��̊֐���Battle,Practice���p������̂�
	//�����ł�BaseScene�̍X�V�֐��͌p�����Ȃ��i���̎��_�ł͂܂��������z�֐��̂܂܁j

	//----------�p����ŕK�v�Ȏ��ɌĂ΂��֐�----------

	//�w�i�`��
	void DrawBackScreen();

 	//�|�[�Y��ʂ̏���
	void UpdatePauseMenu();

	//�|�[�Y��ʂɑJ�ڂ�҂���(S_Active�ɋL�q)
	void WaitGotoPause();

	//�v���C��ʂɑJ�ڂ�҂���(S_InActive�ɋL�q)
	void WaitGotoPlay();

	//�ȉ��͉��z�֐��ŁA�p����ŕʓr������������̂��L�q

	//�|�[�Y��ʂɈړ�����
	virtual void GotoPause() {};

	//�ʏ��ԂɈړ�����
	virtual void GotoPlay() {};

	//�^�C�g���Ɉړ�����
	virtual void GotoTitle() {};

	//�I���A�C�R���̈ʒu���Z�b�g
	//���̃N���X�ł͕��@���w�肵�Ȃ�
	virtual void SetPauseIconY() {};
};

