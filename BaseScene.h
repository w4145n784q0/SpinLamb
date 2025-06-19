#pragma once
#include "Engine/GameObject.h"

//Title,GameMode,ResultScene�̋��ʏ������܂Ƃ߂��N���X
class BaseScene :
    public GameObject
{
protected:

	//�Q�[���V�[���S�ʂ̏��
	enum SceneState
	{
		S_Active = 0,
		S_Passive,
		S_Transition,
		MaxDecide
	};
	SceneState SceneState_;

	//�V�[���J�ڂ܂ł̎��Ԃ��v��
	int SceneTransitionTimer_;
public:
	BaseScene(GameObject* parent);
	BaseScene(GameObject* parent, const std::string& name);
	virtual ~BaseScene();

	void Update() override;

	//�ȉ��̏����͏������z�֐��̂��߁A�p����ŕK���I�[�o�[���C�h

	//�ʏ�̏���
	virtual void UpdateActive() = 0;

	//��~���̏���
	virtual void UpdatePassive() = 0;

	//�V�[���J�ڒ��̏���
	virtual void UpdateTransition() = 0;
};

