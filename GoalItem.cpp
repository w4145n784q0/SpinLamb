#include "GoalItem.h"
#include"Engine/SceneManager.h"
#include"Engine/Model.h"

GoalItem::GoalItem(GameObject* parent)
	:GameObject(parent,"GoalItem"),hCarrot_(-1)
{
}

GoalItem::~GoalItem()
{
}

void GoalItem::Initialize()
{
	hCarrot_ = Model::Load("GoldCarrot.fbx");
	assert(hCarrot_ >= 0);

	SphereCollider* col = new SphereCollider(XMFLOAT3(0, 0, 0), 1.0f);
	this->AddCollider(col);

	transform_.rotate_.x = 80.0f;

}

void GoalItem::Update()
{
	transform_.rotate_.y += 1.0f;
}

void GoalItem::Draw()
{
	Model::SetTransform(hCarrot_, transform_);
	Model::Draw(hCarrot_);
}

void GoalItem::Release()
{
}

void GoalItem::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Player")
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_CLEAR);
	}
}
