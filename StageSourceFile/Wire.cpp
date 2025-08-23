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

void Wire::InitCollision(XMFLOAT3 _pos, XMFLOAT3 _size)
{
	// ボックスコライダーの位置,サイズを設定し、当たり判定の追加
	//継承先のポインタから呼び出す
	BoxCollider* collision_wire = new BoxCollider(_pos, _size);
	AddCollider(collision_wire);
}
