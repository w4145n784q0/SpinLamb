#include "LeftWire.h"

LeftWire::LeftWire(GameObject* parent)
	:Wire(parent, "LeftWire")
{
}

LeftWire::~LeftWire()
{
}

void LeftWire::Initialize()
{
}

void LeftWire::Update()
{
}

void LeftWire::Draw()
{
}

void LeftWire::Release()
{
}

//void LeftWire::OnCollision(GameObject* pTarget)
//{
//}
//
//void LeftWire::InitCollision(XMFLOAT3 pos, XMFLOAT3 size)
//{
//	CollisionPos_ = pos;
//	CollisionSize_ = size;
//
//	BoxCollider* collision_wall = new BoxCollider(pos, size);
//	AddCollider(collision_wall);
//}