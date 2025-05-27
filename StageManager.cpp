#include "StageManager.h"
#include"Engine/CsvReader.h"
#include"Engine/Global.h"
#include"Character.h"

StageManager::StageManager(GameObject* parent)
	:GameObject(parent,"StageManager"),
	NorthEnd_(0.0f), SouthEnd_(0.0f), EastEnd_(0.0f),WestEnd_(0.0f)
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
	csv.Load("CSVdata\\StageData.csv");
	
	GroundData_.position_ = { csv.GetValueFloat(1, 1), csv.GetValueFloat(1, 2), csv.GetValueFloat(1, 3) };
	GroundData_.rotate_ = { csv.GetValueFloat(1, 4), csv.GetValueFloat(1, 5), csv.GetValueFloat(1, 6) };
	GroundData_.scale_ = { csv.GetValueFloat(1, 7), csv.GetValueFloat(1, 8), csv.GetValueFloat(1, 9) };

	WireData_.position_ = { csv.GetValueFloat(2, 1), csv.GetValueFloat(2, 2), csv.GetValueFloat(2, 3) };
	WireData_.rotate_ = { csv.GetValueFloat(2, 4), csv.GetValueFloat(2, 5), csv.GetValueFloat(2, 6) };
	WireData_.scale_ = { csv.GetValueFloat(2, 7), csv.GetValueFloat(2, 8), csv.GetValueFloat(2, 9) };

	PillerData_.position_ = { csv.GetValueFloat(3, 1), csv.GetValueFloat(3, 2), csv.GetValueFloat(3, 3) };
	PillerData_.rotate_ = { csv.GetValueFloat(3, 4), csv.GetValueFloat(3, 5), csv.GetValueFloat(3, 6) };
	PillerData_.scale_ = { csv.GetValueFloat(3, 7), csv.GetValueFloat(3, 8), csv.GetValueFloat(3, 9) };

	NorthEnd_ = csv.GetValueFloat(3, 10);
	SouthEnd_ = csv.GetValueFloat(3, 11);
	EastEnd_ = csv.GetValueFloat(3, 12);
	WestEnd_ = csv.GetValueFloat(3, 13);
}

void StageManager::InitEndData()
{
	Fence* pFence = (Fence*)FindObject("Fence");
	pFence->SetPiller(NorthEnd_,SouthEnd_,WestEnd_,EastEnd_);

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
	pFence->SetWireTransform(WireData_);
	pFence->SetPillerTransform(PillerData_);
	
}
