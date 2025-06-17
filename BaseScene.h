#pragma once
#include "Engine/GameObject.h"

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

	//�I�𒆂̏���
	virtual void UpdateSelect() = 0;

	//�I����̏���(��ɃV�[���J�ڂ܂łɎ��Ԃ���������)
	virtual void UpdateDecide() = 0;
};

