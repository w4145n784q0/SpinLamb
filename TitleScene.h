#pragma once
#include "Engine/GameObject.h"

class TitleScene :
    public GameObject
{
private:
	//----------�摜�n���h��----------

	//�^�C�g���摜
	int hBackScreen_;

	//----------�T�E���h�n���h��----------

	//�^�C�g���p�T�E���h
	int hSoundTitle_;
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
};

