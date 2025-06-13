#include "MiniMap.h"
#include"Engine/Image.h"

namespace
{
	enum MiniMapIndex
	{
		i_reductionXParam = 0,
		i_reductionZParam,
		i_CorrectionValueXParam,
		i_CorrectionValueYParam,
		i_MapAlphaParam,
	};

	float reductionX = 0.0;//�L�����N�^�[��X���W���k������l
	float reductionY = 0.00;//�L�����N�^�[��Z���W���k������l
	float CorrectionValueX = 0.0f;//�}�b�v��X���W��␳����l
	float CorrectionValueY = 0.0f;//�}�b�v��Y���W��␳����l
	int MapAlpha = 0;//�}�b�v�̓����x
}

MiniMap::MiniMap(GameObject* parent)
	:GameObject(parent,"MiniMap"),
	pPlayer_(nullptr), pEnemy_(nullptr)
{
}

MiniMap::~MiniMap()
{
}

void MiniMap::Initialize()
{
	SetCSV();

	pPlayer_ = (Player*)FindObject("Player");
	pEnemy_ = (Enemy*)FindObject("Enemy");

}

void MiniMap::Update()
{
	playerPos_.x = (pPlayer_->GetPosition().x * reductionX) + CorrectionValueX;
	playerPos_.y = (pPlayer_->GetPosition().z * reductionY) - CorrectionValueY;

	enemyPos_.x = (pEnemy_->GetPosition().x * reductionX) + CorrectionValueX;
	enemyPos_.y = (pEnemy_->GetPosition().z * reductionY) - CorrectionValueY;
}

void MiniMap::Draw()
{

}

void MiniMap::Release()
{
}

void MiniMap::SetCSV()
{
	CsvReader csv;
	csv.Load("CSVdata\\MiniMapData.csv");

	std::string init = "MIniMapInit";
	if (csv.IsGetParamName(init))
	{
		std::vector<float> v = csv.GetParam(init);
		reductionX = v[i_reductionXParam];
		reductionY = v[i_reductionZParam];
		CorrectionValueX = v[i_CorrectionValueXParam];
		CorrectionValueY = v[i_CorrectionValueYParam];
		MapAlpha = (int)v[i_MapAlphaParam];
	}
}
