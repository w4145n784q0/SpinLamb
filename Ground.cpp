#include "Ground.h"
#include"Engine/Model.h"

namespace {
	Transform t;
}

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

	hhouse_ = Model::Load("Model\\Logs.fbx");
	assert(hGrass_ >= 0);

	t.position_ = { 0,0,-100 };
	t.rotate_ = { 0,0,0 };
	t.scale_ = { 1,1,1 };
}

void Ground::Update()
{
}

void Ground::Draw()
{
	//ƒ‚ƒfƒ‹•`‰æ
	Model::SetAndDraw(hGrass_, this->transform_);

	Model::SetAndDraw(hhouse_, t);

#ifdef _DEBUG
	if (ImGui::TreeNode("Ground"))
	{
		if (ImGui::TreeNode("House"))
		{
			ImGui::SliderFloat("houseposX", &t.position_.x, -100.0f, 100.0f);
			ImGui::SliderFloat("houseposY", &t.position_.y, -100.0f, 100.0f);
			ImGui::SliderFloat("houseposZ", &t.position_.z, -100.0f, 100.0f);

			ImGui::SliderFloat("housescaleX", &t.scale_.x, 0.0f, 10.0f);
			ImGui::SliderFloat("housescaleY", &t.scale_.y, 0.0f, 10.0f);
			ImGui::SliderFloat("housescaleZ", &t.scale_.z, 0.0f, 10.0f);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("GroundPosition"))
		{
			ImGui::InputFloat("PositionX", &this->transform_.position_.x);
			ImGui::InputFloat("PositionY", &this->transform_.position_.y);
			ImGui::InputFloat("PositionZ", &this->transform_.position_.z);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("GroundRotate"))
		{
			ImGui::InputFloat("PositionX", &this->transform_.rotate_.x);
			ImGui::InputFloat("PositionY", &this->transform_.rotate_.y);
			ImGui::InputFloat("PositionZ", &this->transform_.rotate_.z);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("GroundScale"))
		{
			ImGui::InputFloat("PositionX", &this->transform_.scale_.x);
			ImGui::InputFloat("PositionY", &this->transform_.scale_.y);
			ImGui::InputFloat("PositionZ", &this->transform_.scale_.z);
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
#endif

}

void Ground::Release()
{
}
