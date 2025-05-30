#include "Fence.h"
#include"Engine/Model.h"
#include"Engine/BoxCollider.h"
#include<array>
#include<vector>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

namespace
{
	//柱の数
	int pillerNum = 0;

	//柱の位置を格納するTransform配列
	std::vector<Transform> pillers = {};

	//柱の位置(XMFLOAT3)の配列
	std::vector<XMFLOAT3> PillerPosArray = {};
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
}

void Fence::Update()
{
}

void Fence::Draw()
{
#ifdef _DEBUG
	//ImGui::SliderFloat("Pos y", &transform_.position_.y, 0.0, 10.0);
	//ImGui::Text("mapPos.x%.1f", mTrans.position_.x);
	//ImGui::Text("mapPos.y%.1f", mTrans.position_.y);
#endif

	Model::SetTransform(hFence_, wire);
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

void Fence::SetPiller(float upper, float lower, float left, float right, float height)
{

	piller_UpperLeft_ = { left,height,upper };
	piller_UpperRight_ = { right,height,upper };
	piller_LowerLeft_ = { left, height,lower };
	piller_LowerRight_ = { right, height,lower };

	//この時点でPillerPosArrayの値を入れる
	PillerPosArray = { piller_UpperLeft_ ,piller_UpperRight_ , piller_LowerLeft_,piller_LowerRight_ };

	pillers.resize(pillerNum);

	for (int i = 0; i < pillerNum; i++)
	{
		pillers[i].position_ = PillerPosArray[i];
	}
}

void Fence::SetPillerNum(int num)
{
	pillerNum = num;
}

void Fence::InitPillerTransform(Transform _t)
{
	for (int i = 0; i < pillers.size(); i++)
	{
		pillers[i] = _t;
	}
}