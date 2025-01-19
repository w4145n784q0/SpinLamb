#include "Tree.h"
#include"Engine/Model.h"

Tree::Tree(GameObject* parent)
	:GameObject(parent, "Tree"),hTree_(-1)
{
}

Tree::~Tree()
{
}

void Tree::Initialize()
{
	hTree_ = Model::Load("Tree.fbx");
	SphereCollider* col = new SphereCollider(XMFLOAT3(0, 0, 0), 1.0f);
	this->AddCollider(col);

	//Ground* pGround_ = (Ground*)FindObject("Ground");
//	transform_.position_ = pGround_->GetTreeTrans().position_;
}

void Tree::Update()
{
}

void Tree::Draw()
{
	Model::SetTransform(hTree_, transform_);
	Model::Draw(hTree_);
}

void Tree::Release()
{
}

void Tree::OnCollision(GameObject* pTarget)
{
}
