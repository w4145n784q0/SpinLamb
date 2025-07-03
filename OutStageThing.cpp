#include "OutStageThing.h"
#include"Engine/Model.h"

OutStageThing::OutStageThing(GameObject* parent)
	:GameObject(parent, "OutStageThing"), hCabin_(-1), hTree_(-1), hLogs_(-1)
{
}

OutStageThing::~OutStageThing()
{
}

void OutStageThing::Initialize()
{
	//各モデルの読み込み
	hCabin_ = Model::Load("Model\\House2.fbx");
	assert(hCabin_ >= 0);

	hTree_ = Model::Load("Model\\LittleTree.fbx");
	assert(hTree_ >= 0);

	hLogs_ = Model::Load("Model\\Logs.fbx");
	assert(hLogs_ >= 0);
}

void OutStageThing::Update()
{
}

void OutStageThing::Draw()
{
	//各モデル描画

	for (int i = 0; i < CabinData_.size(); i++)
	{
		Model::SetAndDraw(hCabin_, CabinData_[i]);
	}

	for (int i = 0; i < TreeData_.size(); i++)
	{
		Model::SetAndDraw(hTree_, TreeData_[i]);
	}

	for (int i = 0; i < LogsData_.size(); i++)
	{
		Model::SetAndDraw(hLogs_, LogsData_[i]);
	}


}

void OutStageThing::Release()
{
}
