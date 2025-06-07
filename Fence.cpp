#include "Fence.h"
#include"Engine/Model.h"
#include"Engine/BoxCollider.h"
#include"UpperWire.h"
#include"LowerWire.h"
#include"LeftWire.h"
#include"RightWire.h"


#include<array>
#include<vector>

namespace
{
	//柱の数
	int pillerNum = 0;

	//鉄線のトランスフォーム
	Transform wire;

	//柱のトランスフォーム(回転・拡大率のみ 位置は別に扱う)
	Transform piller;

	//柱の位置を格納するTransform配列
	std::vector<Transform> pillersTransform = {};

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

	Instantiate<UpperWire>(this);
	Instantiate<LowerWire>(this);
	Instantiate<RightWire>(this);
	Instantiate<LeftWire>(this);

}

void Fence::Update()
{
}

void Fence::Draw()
{
#ifdef _DEBUG
	
#endif

	Model::SetTransform(hFence_, wire);
	Model::Draw(hFence_);

	for (int i = 0; i < pillersTransform.size(); i++)
	{
		Model::SetTransform(hPiller_, pillersTransform[i]);
		Model::Draw(hPiller_);
	}
}

void Fence::Release()
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

	pillersTransform.resize(pillerNum);

	for (int i = 0; i < pillerNum; i++)
	{
		pillersTransform[i].position_ = PillerPosArray[i];
		pillersTransform[i].rotate_ = piller.rotate_;
		pillersTransform[i].scale_ = piller.scale_;
	}
}

void Fence::SetPillerNum(int num)
{
	pillerNum = num;
}

void Fence::InitWireTransform(Transform _t)
{
	wire = _t; 
}

void Fence::InitPillerTransform(Transform _t)
{
	piller = _t;
}

void Fence::SetWireCollisionUpper(XMFLOAT3 pos, XMFLOAT3 size, XMFLOAT3 normal)
{
	UpperWire* pUpperWire = (UpperWire*)FindObject("UpperWire");
	pUpperWire->InitCollision(pos,size);
	pUpperWire->SetNormal(XMVECTOR({ normal.x,normal.y,normal.z }));
}

void Fence::SetWireCollisionLower(XMFLOAT3 pos, XMFLOAT3 size, XMFLOAT3 normal)
{
	LowerWire* pLowerWire = (LowerWire*)FindObject("LowerWire");
	pLowerWire->InitCollision(pos, size);
	pLowerWire->SetNormal(XMVECTOR({ normal.x,normal.y,normal.z }));
}

void Fence::SetWireCollisionRight(XMFLOAT3 pos, XMFLOAT3 size, XMFLOAT3 normal)
{
	RightWire* pRightWire = (RightWire*)FindObject("RightWire");
	pRightWire->InitCollision(pos, size);
	pRightWire->SetNormal(XMVECTOR({ normal.x,normal.y,normal.z }));
}

void Fence::SetWireCollisionLeft(XMFLOAT3 pos, XMFLOAT3 size, XMFLOAT3 normal)
{
	LeftWire* pLeftWire = (LeftWire*)FindObject("LeftWire");
	pLeftWire->InitCollision(pos, size);
	pLeftWire->SetNormal(XMVECTOR({ normal.x,normal.y,normal.z }));
}