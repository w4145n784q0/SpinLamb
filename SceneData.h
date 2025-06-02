#pragma once
#include "Engine/GameObject.h"

//,gameover,clear�V�[���Ōp������
//�e�V�[���̋��ʏ������܂Ƃ߂��N���X

class SceneData :
    public GameObject
{
protected:

	//���ʉ�ʂ̃��S�̈ʒu
	Transform Result;

	//���ʉ�ʂ̉��̃��S�̈ʒu
	Transform PushTitle;

public:
	SceneData(GameObject* parent);
	SceneData(GameObject* parent, const std::string& name);
	virtual ~SceneData();

	//csv�ǂݍ���(�K�v�ȃV�[�����������ɍs��)
	void SetCSVScene();

};

