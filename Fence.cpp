#include "Fence.h"
#include"Engine/Model.h"
#include"Engine/BoxCollider.h"

Fence::Fence(GameObject* parent)
	:GameObject(parent,"Fence"),hFence_(-1)
{
}

Fence::~Fence()
{
}

void Fence::Initialize()
{
	hFence_ = Model::Load("ElectricFence.fbx");
	assert(hFence_ >= 0);
	transform_.position_ = { 0,-0.5,0 };

	BoxCollider* collision_wall1 = new BoxCollider(XMFLOAT3(0, 5, 60.5), XMFLOAT3(120, 10, 1));
	AddCollider(collision_wall1);
	BoxCollider* collision_wall2 = new BoxCollider(XMFLOAT3(0, 5, -60.5), XMFLOAT3(120, 10, 1));
	AddCollider(collision_wall2);
	BoxCollider* collision_wall3 = new BoxCollider(XMFLOAT3(60.5, 5, 0), XMFLOAT3(1, 10, 120));
	AddCollider(collision_wall3);
	BoxCollider* collision_wall4 = new BoxCollider(XMFLOAT3(-60.5, 5, 0), XMFLOAT3(1, 10, 120));
	AddCollider(collision_wall4);
}

void Fence::Update()
{
}

void Fence::Draw()
{
	Model::SetTransform(hFence_, transform_);
	Model::Draw(hFence_);
}

void Fence::Release()
{
}

void Fence::OnCollision(GameObject* pTarget)
{
}
