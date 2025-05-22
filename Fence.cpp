#include "Fence.h"
#include"Engine/Model.h"
#include"Engine/BoxCollider.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

namespace
{
	Transform piller[4];

	XMFLOAT3 piller_UpperLeft = { -60,0,60 };
	XMFLOAT3 piller_UpperRight = { 60,0,60 };
	XMFLOAT3 piller_LowerLeft = { -60,0,-60 };
	XMFLOAT3 piller_LowerRight = { 60,0,-60 };

}

Fence::Fence(GameObject* parent)
	:GameObject(parent,"Fence"),hPiller_(-1), hFence_(-1)
{
}

Fence::~Fence()
{
}

void Fence::Initialize()
{

	hPiller_ = Model::Load("Model\\piller.fbx");
	assert(hPiller_ >= 0);

	hFence_ = Model::Load("Model\\wire.fbx");
	assert(hFence_ >= 0);

	transform_.position_ = { 0,5.0f,0 };

	BoxCollider* collision_wall1 = new BoxCollider(XMFLOAT3(0, 0, 60.5), XMFLOAT3(120, 10, 1));
	AddCollider(collision_wall1);
	BoxCollider* collision_wall2 = new BoxCollider(XMFLOAT3(0, 0, -60.5), XMFLOAT3(120, 10, 1));
	AddCollider(collision_wall2);
	BoxCollider* collision_wall3 = new BoxCollider(XMFLOAT3(60.5, 0, 0), XMFLOAT3(1, 10, 120));
	AddCollider(collision_wall3);
	BoxCollider* collision_wall4 = new BoxCollider(XMFLOAT3(-60.5, 0, 0), XMFLOAT3(1, 10, 120));
	AddCollider(collision_wall4);

	piller[0].position_ = piller_UpperLeft;
	piller[1].position_ = piller_UpperRight;
	piller[2].position_ = piller_LowerLeft;
	piller[3].position_ = piller_LowerRight;
}

void Fence::Update()
{
}

void Fence::Draw()
{
#ifdef _DEBUG
	ImGui::SliderFloat("mapPos y", &transform_.position_.y, 0.0, 10.0);
	//ImGui::Text("mapPos.x%.1f", mTrans.position_.x);
	//ImGui::Text("mapPos.y%.1f", mTrans.position_.y);
#endif

	Model::SetTransform(hFence_, transform_);
	Model::Draw(hFence_);

	for (int i = 0; i < 4; i++)
	{
		Model::SetTransform(hPiller_, piller[i]);
		Model::Draw(hPiller_);
	}
}

void Fence::Release()
{
}

void Fence::OnCollision(GameObject* pTarget)
{
}
