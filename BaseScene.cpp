#include "BaseScene.h"
#include"Engine/Audio.h"

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
	//‚±‚Ìˆ—‚ÍŒp³æ‚ÌUpdate‚Å•K‚¸ŒÄ‚Ô
	switch (SceneState_)
	{
	case BaseScene::S_Active:
		UpdateActive();
		break;
	case BaseScene::S_Passive:
		UpdatePassive();
		break;
	case BaseScene::S_Transition:
		UpdateTransition();
		break;
	case BaseScene::MaxDecide:
		break;
	default:
		break;
	}


	/*switch (ModeDecide_)
	{
	case BaseScene::Selected:
		UpdateSelect();
		break;
	case BaseScene::Decided:
		UpdateDecide();
		break;
	default:
		break;
	}*/
}