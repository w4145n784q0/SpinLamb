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
#ifdef _DEBUG
	if (ImGui::TreeNode("RightWire"))
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

void RightWire::Release()
{
}

void RightWire::OnCollision(GameObject* pTarget)
{
}

void RightWire::InitCollision(XMFLOAT3 pos, XMFLOAT3 size)
{
	CollisionPos_ = pos;
	CollisionSize_ = size;

	BoxCollider* collision_wall1 = new BoxCollider(pos, size);
	AddCollider(collision_wall1);
}