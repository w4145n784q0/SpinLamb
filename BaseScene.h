#pragma once
#include "Engine/GameObject.h"

//Title,GameMode,ResultScene�̋��ʏ������܂Ƃ߂��N���X
class BaseScene :
    public GameObject
{
protected:

	//�I�𒆂��I���ォ
	enum Decide
	{
		Selected,
		Decided,
		MaxSelect
	};
	Decide ModeDecide_;

	//�V�[���J�ڂ܂ł̎��Ԃ��v��
	int SceneTransitionTimer_;
public:
	BaseScene(GameObject* parent);
	BaseScene(GameObject* parent, const std::string& name);
	virtual ~BaseScene();

	void Update() override;

	//�ȉ��̏����͏������z�֐��̂��߁A�p����ŕK���I�[�o�[���C�h

	//�I�𒆂̏����i�v���C���ȊO�Ɏg�p�j
	virtual void UpdateSelect() = 0;

	//�I����̏����i�v���C���ȊO�Ɏg�p�j��ɃV�[���J�ڂ܂łɎ��Ԃ���������
	virtual void UpdateDecide() = 0;
};

