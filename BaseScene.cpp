#include "BaseScene.h"

BaseScene::BaseScene(GameObject* parent)
	:GameObject(parent,"BaseScene")
{
}

BaseScene::BaseScene(GameObject* parent, const std::string& name)
	:GameObject(parent, name)
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
		Select();
		break;
	case BaseScene::Decided:
		Decide();
		break;
	default:
		break;
	}
}
