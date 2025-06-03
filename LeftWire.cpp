#include "LeftWire.h"

LeftWire::LeftWire(GameObject* parent)
	:GameObject(parent, "LeftWire"),Normal_({ 0,0,0 })
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

void LeftWire::OnCollision(GameObject* pTarget)
{
}

void LeftWire::InitCollision(XMFLOAT3 pos, XMFLOAT3 size)
{
	BoxCollider* collision_wall1 = new BoxCollider(pos, size);
	AddCollider(collision_wall1);
}