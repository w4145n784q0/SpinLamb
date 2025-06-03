#include "LowerWire.h"

LowerWire::LowerWire(GameObject* parent)
	:GameObject(parent, "LowerWire"), Normal_({ 0,0,0 })
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

void LowerWire::OnCollision(GameObject* pTarget)
{
}

void LowerWire::InitCollision(XMFLOAT3 pos, XMFLOAT3 size)
{
	BoxCollider* collision_wall1 = new BoxCollider(pos,size);
	AddCollider(collision_wall1);
}
