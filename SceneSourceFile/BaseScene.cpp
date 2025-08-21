#include "BaseScene.h"

BaseScene::BaseScene(GameObject* parent)
	:GameObject(parent,"BaseScene"),SceneState_(S_Active),SceneTransitionTimer_(0)
{
}

BaseScene::BaseScene(GameObject* parent, const std::string& name)
	:GameObject(parent, name), SceneState_(S_Active), SceneTransitionTimer_(0)
{
}

BaseScene::~BaseScene()
{
}

void BaseScene::Update()
{
	//���݂̃V�[���̏��(�ʏ�E��~�E�J�ڒ�)�ɂ���čX�V�𕪂���
	//�p���悪SceneManager�ɓo�^����Ă���V�[���̏ꍇ�AUpdate�ŕK���Ă�
	//���̏������������邩�͌p����ɂ���ĕ�����

	switch (SceneState_)
	{
	case BaseScene::S_Active:
		UpdateActive();
		break;
	case BaseScene::S_InActive:
		UpdateInActive();
		break;
	case BaseScene::S_Transition:
		UpdateTransition();
		break;
	default:
		break;
	}

}