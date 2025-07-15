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
	FirstPos_({ 0,0,0 }), SecondPos_({ 0,0,0 }),
	OriginalFirstPos_({ 0,0,0 }), OriginalSecondPos_({ 0,0,0 })
{
}

MiniMap::~MiniMap()
{
}

void MiniMap::Initialize()
{
	//csv����p�����[�^�ǂݍ���
	SetMiniMapCSV();
}

void MiniMap::Update()
{
	//�v���C���[�ECPU�̈ʒu���Ƃ葱����
	//�L�����N�^�[�̃��[���h���W���k��,�␳�l�𑫂��ă}�b�v���ɕ\��

	FirstPos_.x = (OriginalFirstPos_.x * reductionX) + CorrectionValueX;
	FirstPos_.y = (OriginalFirstPos_.z * reductionY) - CorrectionValueY;

	SecondPos_.x = (OriginalSecondPos_.x * reductionX) + CorrectionValueX;
	SecondPos_.y = (OriginalSecondPos_.z * reductionY) - CorrectionValueY;
}

void MiniMap::Draw()
{

}

void MiniMap::Release()
{
}

void MiniMap::SetMiniMapCSV()
{
	//csv�t�@�C���ǂݍ���
	CsvReader csv;
	csv.Load("CSVdata\\HUDData\\MiniMapData.csv");

	//csv�t�@�C���̊e0��ڂ̕�������擾
	std::string init = "MIniMapInit";

	//0��ڂ̕������n���A���̍s�̃p�����[�^���擾
	std::vector<float> MapData = GetCSVReadData(csv, init);

	//�������̏��Ԃ�csv�̊e�s�̏��Ԃɍ��킹��
	//v�̓Y������namespace�Ő錾�����񋓌^���g�p
	reductionX = MapData[i_reductionXParam];
	reductionY = MapData[i_reductionZParam];
	CorrectionValueX = MapData[i_CorrectionValueXParam];
	CorrectionValueY = MapData[i_CorrectionValueYParam];
	
}
