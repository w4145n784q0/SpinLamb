#pragma once
#include "Engine/GameObject.h"

//title,practice,gameover,clear�V�[���Ōp������
//�e�V�[���̋��ʏ������܂Ƃ߂��N���X

class SceneData :
    public GameObject
{
protected:

	//���ʉ�ʂ̃��S�̈ʒu
	Transform Result;
	XMFLOAT3 ResultPosition_ = { 0,0,0 }; 

	//���ʉ�ʂ̉��̃��S�̈ʒu
	Transform PushTitle;
	XMFLOAT3 UnderPosition_ = { 0,0,0 };

	//���K���[�h�̏I�����̃J�E���g
	int SceneEnd_ = 0;

public:
	SceneData(GameObject* parent);
	SceneData(GameObject* parent, const std::string& name);
	virtual ~SceneData();

	//csv�ǂݍ���(�K�v�ȃV�[�����������ɍs��)
	void SetCSVScene();

	int GetPracticeEnd() { return SceneEnd_; }
	XMFLOAT3 GetResultPosition() { return ResultPosition_; }
	XMFLOAT3 GetUnderPosition() { return UnderPosition_; }
};

