#include "BaseWire.h"

BaseWire::BaseWire(GameObject* parent)
	:GameObject(parent, "BaseWire"), Normal_({ 0,0,0 })
{
}

BaseWire::BaseWire(GameObject* parent, const std::string& name)
	:GameObject(parent,name), Normal_({ 0,0,0 })
{
}

BaseWire::~BaseWire()
{
}

void BaseWire::Initialize()
{
}

void BaseWire::Update()
{
}

void BaseWire::Draw()
{
}

void BaseWire::Release()
{
}

void BaseWire::InitCollision(XMFLOAT3 _pos, XMFLOAT3 _size)
{
	// ボックスコライダーの位置,サイズを設定し、当たり判定の追加
	//継承先のポインタから呼び出す
	BoxCollider* collision_wire = new BoxCollider(_pos, _size);
	AddCollider(collision_wire);
}
