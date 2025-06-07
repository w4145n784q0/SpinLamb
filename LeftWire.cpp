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
#ifdef _DEBUG
	if (ImGui::TreeNode("LeftWire"))
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

void LeftWire::Release()
{
}

void LeftWire::OnCollision(GameObject* pTarget)
{
}

void LeftWire::InitCollision(XMFLOAT3 pos, XMFLOAT3 size)
{
	CollisionPos_ = pos;
	CollisionSize_ = size;

	BoxCollider* collision_wall = new BoxCollider(pos, size);
	AddCollider(collision_wall);
}