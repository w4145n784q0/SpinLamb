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
#ifdef _DEBUG
	if (ImGui::TreeNode("LowerWire"))
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

void LowerWire::Release()
{
}

void LowerWire::OnCollision(GameObject* pTarget)
{
}

void LowerWire::InitCollision(XMFLOAT3 pos, XMFLOAT3 size)
{
	CollisionPos_ = pos;
	CollisionSize_ = size;

	BoxCollider* collision_wall1 = new BoxCollider(pos,size);
	AddCollider(collision_wall1);
}
