#pragma once
#include "Engine/GameObject.h"
#include"SceneData.h"

class PracticeScene :
    public SceneData
{
private:

	//----------�摜�n���h��----------

	//�^�C�g���摜
	int hBackScreen_;

	//�������Ń^�C�g���ɑJ�ڂ���J�E���g
	int Press_;

	//----------�T�E���h�n���h��----------
	int hSoundPractice_; 

public:
	//�R���X�g���N�^
	//�����Fparent  �e�I�u�W�F�N�g�iSceneManager�j
	PracticeScene(GameObject* parent);

	~PracticeScene();

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;
};

