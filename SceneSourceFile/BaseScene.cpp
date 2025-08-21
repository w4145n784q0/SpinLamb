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
	//現在のシーンの状態(通常・停止・遷移中)によって更新を分ける
	//継承先がSceneManagerに登録されているシーンの場合、Updateで必ず呼ぶ
	//何の処理を実装するかは継承先によって分ける

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