#pragma once
#include "Engine/GameObject.h"

//�e�V�[���̊��N���X
//�V�[�����Ƃ̋��ʏ������܂Ƃ߂�
class BaseScene :
    public GameObject
{
protected:

	//�Q�[���V�[���S�ʂ̏��
	//�����̒l�ɉ����ĊeUpdate�֐����Ăяo��
	enum SceneState
	{
		S_Active = 0,//�ʏ�
		S_Transition,//�V�[���J�ڒ�
		MaxScene
	};
	SceneState SceneState_;

	//�V�[���J�ڂ܂ł̎��Ԃ��v��
	int SceneTransitionTimer_;
public:
	BaseScene(GameObject* parent);
	BaseScene(GameObject* parent, const std::string& name);
	virtual ~BaseScene();

	//�X�V
	void Update() override;

	//�ȉ��̏����͏������z�֐��̂��߁A�p����ŕK���I�[�o�[���C�h

	//�ʏ�̏���
	virtual void UpdateActive() = 0;

	//�V�[���J�ڒ��̏���(�V�[���J�ڒ��̉��o�ȂǂɎg�p)
	virtual void UpdateTransition() = 0;
};

