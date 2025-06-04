#include "StageManager.h"
#include"Engine/CsvReader.h"
#include"Engine/Global.h"

namespace
{
	enum GroundEndIndex
	{
		i_up = 0,
		i_down,
		i_right,
		i_left,
		i_pillernum,
	};

	enum StageCollisionIndex
	{
		i_collisionX = 0,
		i_collisionY,
		i_collisionZ,
	};

}

StageManager::StageManager(GameObject* parent)
	:GameObject(parent,"StageManager"),
	UpperEnd_(0.0f), LowerEnd_(0.0f), RightEnd_(0.0f), LeftEnd_(0.0f),PillerNum_(0),
	WirePosUpper_({ 0,0,0 }), WirePosLower_({ 0,0,0 }),WirePosRight_({ 0,0,0 }), WirePosLeft_({ 0,0,0 }), 
	WireSizeUpper_({ 0,0,0 }), WireSizeLower_({ 0,0,0 }), WireSizeRight_({ 0,0,0 }),WireSizeLeft_({ 0,0,0 }),
	UpperNormal_({ 0,0,0 }), LowerNormal_({ 0,0,0 }), RightNormal_({ 0,0,0 }), LeftNormal_({ 0,0,0 })
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
		UpperEnd_ = v[i_up];
		LowerEnd_ = v[i_down];
		RightEnd_ = v[i_right];
		LeftEnd_ = v[i_left];
		PillerNum_ = static_cast<int>(v[i_pillernum]);
	}

	CsvReader csv_wire;
	csv_wire.Load("CSVdata\\StageWireData.csv");//当たり判定の位置、サイズ

	std::string collision[] = { "UpperPos","LowerPos","RightPos","LeftPos",
	"UpperSize","LowerSize","RightSize","LeftSize",
	"UpperNormal","LowerNormal","RightNormal","LeftNormal"};

	XMFLOAT3* CollisionData[] = { &WirePosUpper_ ,&WirePosLower_,&WirePosRight_,&WirePosLeft_,
	&WireSizeUpper_,&WireSizeLower_,&WireSizeRight_,&WireSizeLeft_,
	&UpperNormal_,&LowerNormal_,&RightNormal_,&LeftNormal_ };

	for (int i = 0; i < sizeof(collision) / sizeof(collision[0]); i++)
	{
		if (csv_wire.IsGetParamName(collision[i]))
		{
			std::vector<float> v = csv_wire.GetParam(collision[i]);

			CollisionData[i]->x = v[i_collisionX];
			CollisionData[i]->y = v[i_collisionY];
			CollisionData[i]->z = v[i_collisionZ];
		}
	}
}

void StageManager::InitEndData()
{
	Fence* pFence = (Fence*)FindObject("Fence");
	pFence->SetPillerNum(PillerNum_);
	pFence->SetPiller(UpperEnd_,LowerEnd_,RightEnd_,LeftEnd_, PillerData_.position_.y);

	pFence->SetWireCollisionUpper(WirePosUpper_, WireSizeUpper_, UpperNormal_);
	pFence->SetWireCollisionLower(WirePosLower_, WireSizeLower_,LowerNormal_);
	pFence->SetWireCollisionRight(WirePosRight_, WireSizeRight_, RightNormal_);
	pFence->SetWireCollisionLeft(WirePosLeft_, WireSizeLeft_, LeftNormal_);

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
