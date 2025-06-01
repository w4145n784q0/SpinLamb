#pragma once
#include "Engine/GameObject.h"
#include"SceneData.h"

class GameOverScene :
    public SceneData
{
private:
	//----------�摜�n���h��----------

	//�^�C�g���摜
	int hBackScreen_;

	//"cpu win"�̃��S
	int hlogoResult_;

	//"push title"�̃��S
	int hlogoTitle_;

	//----------�T�E���h�n���h��----------

	//�Q�[���I�[�o�[�T�E���h
	int hSoundGameOver_;
public:
	//�R���X�g���N�^
	//�����Fparent  �e�I�u�W�F�N�g�iSceneManager�j
	GameOverScene(GameObject* parent);

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;
};

