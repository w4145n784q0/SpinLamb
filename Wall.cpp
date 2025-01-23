#include "Wall.h"

Wall::Wall(GameObject* parent)
	:GameObject(parent,"Wall")
{
}

Wall::~Wall()
{
}

void Wall::Initialize()
{
	SphereCollider* col = new SphereCollider(XMFLOAT3(0, 0, 0), 1.0f);
	this->AddCollider(col);
}

void Wall::Update()
{
}

void Wall::Draw()
{
}

void Wall::Release()
{
}

void Wall::OnCollision(GameObject* pTarget)
{
}
