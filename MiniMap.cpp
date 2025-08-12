#include "MiniMap.h"

namespace
{
	//csv�ǂݍ��ݎ��̃C���f�b�N�X(�}�b�v�̊e�␳�l)
	enum MiniMapIndex
	{
		i_ReductionXParam = 0,
		i_ReductionZParam,
		i_CorrectionValueXParam,
		i_CorrectionValueYParam,
	};

	//�L�����N�^�[��X���W���k������l
	float ReductionX = 0.0f;

	//�L�����N�^�[��Z���W���k������l
	float ReductionY = 0.0f;

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

	FirstPos_.x = (OriginalFirstPos_.x * ReductionX) + CorrectionValueX;
	FirstPos_.y = (OriginalFirstPos_.z * ReductionY) - CorrectionValueY;

	SecondPos_.x = (OriginalSecondPos_.x * ReductionX) + CorrectionValueX;
	SecondPos_.y = (OriginalSecondPos_.z * ReductionY) - CorrectionValueY;
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
	std::string Init = "MiniMapInit";

	//0��ڂ̕������n���A���̍s�̃p�����[�^���擾
	std::vector<float> MapData = GetCSVReadData(csv, Init);

	//�������̏��Ԃ�csv�̊e�s�̏��Ԃɍ��킹��
	//v�̓Y������namespace�Ő錾�����񋓌^���g�p
	ReductionX = MapData[i_ReductionXParam];
	ReductionY = MapData[i_ReductionZParam];
	CorrectionValueX = MapData[i_CorrectionValueXParam];
	CorrectionValueY = MapData[i_CorrectionValueYParam];
	
}
