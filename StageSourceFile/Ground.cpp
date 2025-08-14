#include "Ground.h"
#include"../Engine/Model.h"


Ground::Ground(GameObject* parent)
	:GameObject(parent,"Ground"),hGrass_(-1)
{
	
}

Ground::~Ground()
{
}

void Ground::Initialize()
{
	//ƒ‚ƒfƒ‹‚Ì“Ç‚Ýž‚Ý
	hGrass_ = Model::Load("Model\\GrassField.fbx");
	assert(hGrass_ >= 0);
}

void Ground::Update()
{
}

void Ground::Draw()
{
	//ƒ‚ƒfƒ‹•`‰æ
	Model::SetAndDraw(hGrass_, this->transform_);
}

void Ground::Release()
{
}

void Ground::DrawImGui()
{
#ifdef _DEBUG
	if (ImGui::TreeNode("Ground"))
	{
		if (ImGui::TreeNode("GroundPosition"))
		{
			ImGui::InputFloat("PositionX", &this->transform_.position_.x, ZeroPointOne);
			ImGui::InputFloat("PositionY", &this->transform_.position_.y, ZeroPointOne);
			ImGui::InputFloat("PositionZ", &this->transform_.position_.z, ZeroPointOne);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("GroundRotate"))
		{
			ImGui::InputFloat("RotateX", &this->transform_.rotate_.x, ZeroPointOne);
			ImGui::InputFloat("RotateY", &this->transform_.rotate_.y, ZeroPointOne);
			ImGui::InputFloat("RotateZ", &this->transform_.rotate_.z, ZeroPointOne);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("GroundScale"))
		{
			ImGui::InputFloat("ScaleX", &this->transform_.scale_.x, ZeroPointOne);
			ImGui::InputFloat("ScaleY", &this->transform_.scale_.y, ZeroPointOne);
			ImGui::InputFloat("ScaleZ", &this->transform_.scale_.z, ZeroPointOne);
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
#endif
}
