#include "Wire.h"

Wire::Wire(GameObject* parent)
	:GameObject(parent, "Wire"), Normal_({ 0,0,0 })
{
}

Wire::Wire(GameObject* parent, const std::string& name)
	:GameObject(parent,name), Normal_({ 0,0,0 })
{
}

Wire::~Wire()
{
}

void Wire::Initialize()
{
}

void Wire::Update()
{
}

void Wire::Draw()
{
}

void Wire::Release()
{
}

void Wire::OnCollision(GameObject* pTarget)
{
}

void Wire::InitCollision(XMFLOAT3 pos, XMFLOAT3 size)
{
	// ボックスコライダーの位置,サイズを設定し、当たり判定の追加
	BoxCollider* collision_wall = new BoxCollider(pos, size);
	AddCollider(collision_wall);
}
