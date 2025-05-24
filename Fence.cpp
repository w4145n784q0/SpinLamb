#include "Fence.h"
#include"Engine/Model.h"
#include"Engine/BoxCollider.h"
#include"Engine/CsvReader.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

namespace
{
	const int pillerNum = 4;
	std::vector<Transform> pillers(pillerNum);
	//XMFLOAT3 pos[] = { piller_UpperLeft_ ,piller_UpperRight_ , piller_LowerLeft_,piller_LowerRight_ };
	//Transform piller[4];
	
}

Fence::Fence(GameObject* parent)
	:GameObject(parent,"Fence"),hPiller_(-1), hFence_(-1),
	piller_UpperLeft_({0,0,0}),piller_UpperRight_({0,0,0}),piller_LowerLeft_({0,0,0}),piller_LowerRight_({0,0,0})
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

	BoxCollider* collision_wall1 = new BoxCollider(XMFLOAT3(0, 5, 60.5), XMFLOAT3(120, 10, 1));
	AddCollider(collision_wall1);
	BoxCollider* collision_wall2 = new BoxCollider(XMFLOAT3(0, 5, -60.5), XMFLOAT3(120, 10, 1));
	AddCollider(collision_wall2);
	BoxCollider* collision_wall3 = new BoxCollider(XMFLOAT3(60.5, 5, 0), XMFLOAT3(1, 10, 120));
	AddCollider(collision_wall3);
	BoxCollider* collision_wall4 = new BoxCollider(XMFLOAT3(-60.5, 5, 0), XMFLOAT3(1, 10, 120));
	AddCollider(collision_wall4);

	/*for (int i = 0; i < pillers.size(); i++)
	{
		pillers[i].position_ = pos[i];
	}*/

	
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

	for (int i = 0; i < pillers.size(); i++)
	{
		Model::SetTransform(hPiller_, pillers[i]);
		Model::Draw(hPiller_);
	}
}

void Fence::Release()
{
}

void Fence::OnCollision(GameObject* pTarget)
{
}

void Fence::SetPiller(float upper, float lower, float left, float right)
{
	float height = piller.position_.y;

	piller_UpperLeft_ = { left,height,upper };
	piller_UpperRight_ = { right,height,upper };
	piller_LowerLeft_ = { left, height,lower };
	piller_LowerRight_ = { right, height,lower };

	pillers[0].position_ = piller_UpperLeft_;
	pillers[1].position_ = piller_UpperRight_;
	pillers[2].position_ = piller_LowerLeft_;
	pillers[3].position_ = piller_LowerRight_;
}

//void Fence::SetSCV()
//{
//	CsvReader csv;
//	csv.Load("CSVdata\\StageData.csv");
//
//	this->transform_.position_ = { csv.GetValueFloat(1, 1), csv.GetValueFloat(1, 2), csv.GetValueFloat(1, 3) };
//	piller_UpperLeft_ = { csv.GetValueFloat(1, 4), csv.GetValueFloat(1, 5), csv.GetValueFloat(1, 6) };
//	piller_UpperRight_ = { csv.GetValueFloat(1, 7), csv.GetValueFloat(1, 8), csv.GetValueFloat(1, 9) };
//	piller_LowerLeft_ = { csv.GetValueFloat(1, 10), csv.GetValueFloat(1, 11), csv.GetValueFloat(1, 12) };
//	piller_LowerRight_ = { csv.GetValueFloat(1, 13), csv.GetValueFloat(1, 14), csv.GetValueFloat(1, 15) };
//}
