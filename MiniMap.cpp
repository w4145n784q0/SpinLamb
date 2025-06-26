#include "MiniMap.h"
#include"Engine/Image.h"

namespace
{
	//�␳�l�̃C���f�b�N�X
	enum MiniMapIndex
	{
		i_reductionXParam = 0,
		i_reductionZParam,
		i_CorrectionValueXParam,
		i_CorrectionValueYParam,
	};
	//�L�����N�^�[��X���W���k������l
	float reductionX = 0.0f;

	//�L�����N�^�[��Z���W���k������l
	float reductionY = 0.0f;

	//�}�b�v��X���W��␳����l
	float CorrectionValueX = 0.0f;

	//�}�b�v��Y���W��␳����l
	float CorrectionValueY = 0.0f;
}

MiniMap::MiniMap(GameObject* parent)
	:GameObject(parent,"MiniMap"),
	pPlayer_(nullptr), pEnemy_(nullptr),playerPos_({0,0,0}),enemyPos_({0,0,0})
{
}

MiniMap::~MiniMap()
{
}

void MiniMap::Initialize()
{
	//csv����p�����[�^�ǂݍ���
	SetMiniMapCSV();

	//�C���X�^���X��������
	pPlayer_ = (Player*)FindObject("Player");
	pEnemy_ = (Enemy*)FindObject("Enemy");

}

void MiniMap::Update()
{
	//�v���C���[�ECPU�̈ʒu���Ƃ葱����
	//�L�����N�^�[�̃��[���h���W���k��,�␳�l�𑫂��ă}�b�v���ɕ\��

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

void MiniMap::SetMiniMapCSV()
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
	}
}
