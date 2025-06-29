#include "LowerWire.h"

LowerWire::LowerWire(GameObject* parent)
	:Wire(parent, "LowerWire")
{
}

LowerWire::~LowerWire()
{
}

void LowerWire::Initialize()
{
}

void LowerWire::Update()
{
}

void LowerWire::Draw()
{
}

void LowerWire::Release()
{
}

//void LowerWire::OnCollision(GameObject* pTarget)
//{
//}
//
//void LowerWire::InitCollision(XMFLOAT3 pos, XMFLOAT3 size)
//{
//	CollisionPos_ = pos;
//	CollisionSize_ = size;
//
//	BoxCollider* collision_wall1 = new BoxCollider(pos,size);
//	AddCollider(collision_wall1);
//}
