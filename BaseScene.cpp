#include "BaseScene.h"

BaseScene::BaseScene(GameObject* parent)
	:GameObject(parent,"BaseScene"),ModeDecide_(Selected),SceneTransitionTimer_(0)
{
}

BaseScene::BaseScene(GameObject* parent, const std::string& name)
	:GameObject(parent, name), ModeDecide_(Selected), SceneTransitionTimer_(0)
{
}

BaseScene::~BaseScene()
{
}

void BaseScene::Update()
{
	switch (ModeDecide_)
	{
	case BaseScene::Selected:
		UpdateSelect();
		break;
	case BaseScene::Decided:
		UpdateDecide();
		break;
	default:
		break;
	}
}