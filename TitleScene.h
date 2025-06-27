#pragma once
#include "Engine/GameObject.h"
#include"BaseScene.h"
#include"TransitionEffect.h"

//�^�C�g���V�[��
class TitleScene :
    public BaseScene
{
private:
	//----------�摜�n���h��----------

	//�^�C�g���摜
	int hBackScreen_;

	//----------�T�E���h�n���h��----------

	//�^�C�g���p�T�E���h
	int hSoundTitle_;

	//�^�C�g�����̌��艹
	int hSoundStart_;

	//----------�C���X�^���X----------
	TransitionEffect* pTransitionEffect_;

public:
	TitleScene(GameObject* parent);
	~TitleScene();

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

	//----------BaseScene�̌p���֐�----------
	//�ʏ�̏���
	void UpdateActive() override;

	//�V�[���J�ڒ��̏���
	void UpdateTransition() override;
};