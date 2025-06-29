#include "UpperWire.h"

UpperWire::UpperWire(GameObject* parent)
	:Wire(parent, "UpperWire")
{
}

UpperWire::~UpperWire()
{
}

void UpperWire::Initialize()
{
}

void UpperWire::Update()
{
}

void UpperWire::Draw()
{
}

void UpperWire::Release()
{
}

//void UpperWire::OnCollision(GameObject* pTarget)
//{
//}

//void UpperWire::InitCollision(XMFLOAT3 pos, XMFLOAT3 size)
//{
//	CollisionPos_ = pos;
//	CollisionSize_ = size;
//
//	BoxCollider* collision_wall = new BoxCollider(pos, size);
//	AddCollider(collision_wall);
//}
