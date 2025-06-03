#include "RightWire.h"

RightWire::RightWire(GameObject* parent)
	:GameObject(parent, "RightWire"), Normal_({ 0,0,0 })
{
}

RightWire::~RightWire()
{
}

void RightWire::Initialize()
{
}

void RightWire::Update()
{
}

void RightWire::Draw()
{
}

void RightWire::Release()
{
}

void RightWire::OnCollision(GameObject* pTarget)
{
}

void RightWire::InitCollision(XMFLOAT3 pos, XMFLOAT3 size)
{
	BoxCollider* collision_wall1 = new BoxCollider(pos, size);
	AddCollider(collision_wall1);
}