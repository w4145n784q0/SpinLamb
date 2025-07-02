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

	Model::SetAndDraw(hCabin_, CabinTransform);
	Model::SetAndDraw(hTree_, TreeTransform);
	Model::SetAndDraw(hLogs_, LogsTransform);
}

void OutStageThing::Release()
{
}
