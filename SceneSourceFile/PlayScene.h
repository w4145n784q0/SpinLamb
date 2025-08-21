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


	//----------�g�����X�t�H�[��----------
 
	//�w�i�̃g�����X�t�H�[��
	Transform TransBackScreen_;

	//----------�I����Ԃ��Ǘ����郊�X�g----------

	//�|�[�Y��ʂ̂Ƃ��ɑI�����Ă�����
	enum PauseSelect
	{
		S_Continue = 0,//������
		S_Exit,        //��߂�
		MaxPauseSelect
	};
	PauseSelect PauseSelect_;

	//�|�[�Y��ʂ̑I����Ԃ̃��X�g
	std::list<PauseSelect> PauseSelectList_;

	// PauseSelectList_�̃C���f�b�N�X���w���l
	std::list<PauseSelect>::iterator Pauseitr;

	//�|�[�Y��ʂ̑I���A�C�R�������ɓ����ϐ�
	XMFLOAT3 TmpIconPos_;

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

	//�V�[���J�ڃG�t�F�N�g���Z�b�g
	//���N���X�̎��_�ł͏��������߂��A�p����̂��̂��g�p����
	virtual void SetTransitionEffect() {};

	//�I���A�C�R���̈ʒu���Z�b�g
	//���̃N���X�ł͕��@���w�肵�Ȃ�
	virtual void SetPauseIconY() {};
};

