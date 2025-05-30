#include "StageManager.h"
#include"Engine/CsvReader.h"
#include"Engine/Global.h"

namespace
{
	enum GroundEnd
	{
		up,
		down,
		right,
		left,
		pillernum
	};

}

StageManager::StageManager(GameObject* parent)
	:GameObject(parent,"StageManager"),
	UpperEnd_(0.0f), LowerEnd_(0.0f), RightEnd_(0.0f),LeftEnd_(0.0f)
{
}

StageManager::~StageManager()
{
}

void StageManager::Initialize()
{
	SetSCV();
	Instantiate<Ground>(this);
	InitGroundData();
	Instantiate<Fence>(this);
	InitFenceData();
	InitEndData();
}

void StageManager::Update()
{
}

void StageManager::Draw()
{
}

void StageManager::Release()
{
}

void StageManager::SetSCV()
{
	CsvReader csv;
	csv.Load("CSVdata\\StageData.csv");//ステージオブジェクトのトランスフォーム

	std::string ground = "Ground";
	if (csv.IsGetParamName(ground))
	{
		std::vector<float> v = csv.GetParam(ground);
		SetTransformPRS(GroundData_, v);
	}
	
	std::string wire = "wire";
	if (csv.IsGetParamName(wire))
	{
		std::vector<float> v = csv.GetParam(wire);
		SetTransformPRS(WireData_, v);
	}

	std::string piller = "piller";
	if (csv.IsGetParamName(piller))
	{
		std::vector<float> v = csv.GetParam(piller);
		SetTransformPRS(PillerData_, v);
	}

	CsvReader csv_end;
	csv_end.Load("CSVdata\\StageEndData.csv");//ステージの各端の位置、柱の本数

	std::string end = "end";
	if (csv_end.IsGetParamName(end))
	{
		std::vector<float> v = csv_end.GetParam(end);
		UpperEnd_ = v[up];
		LowerEnd_ = v[down];
		RightEnd_ = v[right];
		LeftEnd_ = v[left];
		PillerNum_ = v[pillernum];
	}
}

void StageManager::InitEndData()
{
	Fence* pFence = (Fence*)FindObject("Fence");
	pFence->SetPillerNum(PillerNum_);
	pFence->SetPiller(UpperEnd_,LowerEnd_,RightEnd_,LeftEnd_, PillerData_.position_.y);
}

void StageManager::InitGroundData()
{
	Ground* pGround = (Ground*)FindObject("Ground");
	pGround->SetScale(GroundData_.scale_);
	pGround->SetRotate(GroundData_.rotate_);
	pGround->SetPosition(GroundData_.position_);
}

void StageManager::InitFenceData()
{
	Fence* pFence = (Fence*)FindObject("Fence");
	pFence->InitWireTransform(WireData_);
	pFence->InitPillerTransform(PillerData_);
	
}
