#include "Ground.h"
#include"../Engine/Model.h"
#include"../Engine/Global.h"


Ground::Ground(GameObject* parent)
	:GameObject(parent,"Ground"),hGrass_(-1)
{
	
}

Ground::~Ground()
{
}

void Ground::Initialize()
{
	//モデルの読み込み
	hGrass_ = Model::Load("Model\\GrassField.fbx");
	assert(hGrass_ >= 0);
}

void Ground::Update()
{
}

void Ground::Draw()
{
	//モデル描画
	Model::SetAndDraw(hGrass_, this->transform_);
}

void Ground::Release()
{
}

void Ground::DrawImGui()
{
#ifdef _DEBUG

	//地面モデル
	if (ImGui::TreeNode("Ground"))
	{
		if (ImGui::TreeNode("GroundPosition"))
		{
			ImGui::InputFloat("PositionX", &this->transform_.position_.x, ZERO_POINT_ONE);
			ImGui::InputFloat("PositionY", &this->transform_.position_.y, ZERO_POINT_ONE);
			ImGui::InputFloat("PositionZ", &this->transform_.position_.z, ZERO_POINT_ONE);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("GroundRotate"))
		{
			ImGui::InputFloat("RotateX", &this->transform_.rotate_.x, ZERO_POINT_ONE);
			ImGui::InputFloat("RotateY", &this->transform_.rotate_.y, ZERO_POINT_ONE);
			ImGui::InputFloat("RotateZ", &this->transform_.rotate_.z, ZERO_POINT_ONE);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("GroundScale"))
		{
			ImGui::InputFloat("ScaleX", &this->transform_.scale_.x, ZERO_POINT_ONE);
			ImGui::InputFloat("ScaleY", &this->transform_.scale_.y, ZERO_POINT_ONE);
			ImGui::InputFloat("ScaleZ", &this->transform_.scale_.z, ZERO_POINT_ONE);
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
#endif
}
