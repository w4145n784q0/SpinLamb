#include "UpperWire.h"

UpperWire::UpperWire(GameObject* parent)
	:GameObject(parent, "UpperWire"), Normal_({ 0,0,0 })
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
#ifdef _DEBUG
	if (ImGui::TreeNode("UpperWire"))
	{
		ImGui::InputFloat("collisionPosX", &CollisionPos_.x);
		ImGui::InputFloat("collisionPosY", &CollisionPos_.y);
		ImGui::InputFloat("collisionPosZ", &CollisionPos_.z);

		ImGui::InputFloat("collisionSizeX", &CollisionSize_.x);
		ImGui::InputFloat("collisionSizeY", &CollisionSize_.y);
		ImGui::InputFloat("collisionSizeZ", &CollisionSize_.z);
	
		ImGui::TreePop();
	}
#endif
}

void UpperWire::Release()
{
}

void UpperWire::OnCollision(GameObject* pTarget)
{
}

void UpperWire::InitCollision(XMFLOAT3 pos, XMFLOAT3 size)
{
	CollisionPos_ = pos;
	CollisionSize_ = size;

	BoxCollider* collision_wall = new BoxCollider(pos, size);
	AddCollider(collision_wall);
}
