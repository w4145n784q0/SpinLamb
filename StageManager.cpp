#include "StageManager.h"
#include"Engine/CsvReader.h"
#include "GameView.h"
#include"Ground.h"
#include"Fence.h"
#include"OutStageThing.h"

namespace
{
	//csv�ǂݍ��ݎ��̃C���f�b�N�X(�e�X�e�[�W�̒[)
	enum GroundEndIndex
	{
		i_up = 0,
		i_down,
		i_right,
		i_left,
		i_pillarnum,
	};

	//csv�ǂݍ��ݎ��̃C���f�b�N�X(�����蔻��̓ǂݍ���)
	enum StageCollisionIndex
	{
		i_collisionX = 0,
		i_collisionY,
		i_collisionZ,
	};

	//�C���X�^���X
	//����cpp�t�@�C�����ň�x�����g��Ȃ��̂Ŗ��O��ԂŐ錾
	Ground* pGround_ = nullptr;
	Fence* pFence_ = nullptr;
	OutStageThing* pOutStageThing_ = nullptr;
}

StageManager::StageManager(GameObject* parent)
	:GameObject(parent,"StageManager"),
	UpperEnd_(0.0f), LowerEnd_(0.0f), RightEnd_(0.0f), LeftEnd_(0.0f),PillarNum_(0),
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
	//Stage�֌W�̒l�͊�{StageManager�N���X�œǂݍ��݁A�n��

	//csv����p�����[�^�ǂݍ���
	SetStageInitCSV();

	//Ground�N���X����
	Instantiate<Ground>(this);

	//Ground�̃f�[�^������
	InitGroundData();

	//Fence�N���X����
	Instantiate<Fence>(this);

	//Fence�̃f�[�^������
	InitFenceData();
	InitEndData();

	//�X�e�[�W�O�I�u�W�F�N�g����
	Instantiate<OutStageThing>(this); 
	InitOutStageThingData();
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

void StageManager::SetStageInitCSV()
{

	//----------�n��,�S��,���̊e�g�����X�t�H�[��������----------

	//csv�t�@�C����ǂݍ���
	CsvReader csv;
	csv.Load("CSVdata\\StageData\\StageTransformData.csv");

	//csv�t�@�C���̊e0��ڂ̕�����̔z����擾
	std::vector<std::string> ParamNames = {
		"Ground","wire","pillar"
	};

	//�e�g�����X�t�H�[����z��ɓ����
	std::vector<std::reference_wrapper<Transform>> TransformArray = {
		GroundData_,WireData_,PillarData_
	};

	//�܂Ƃ߂ď�����
	InitCSVTransformArray(csv, ParamNames, TransformArray);


	//----------�X�e�[�W�̊e�[�̈ʒu�A���̖{��----------

	//csv�t�@�C����ǂݍ���
	CsvReader csv_end;
	csv_end.Load("CSVdata\\StageData\\StageEndData.csv");

	//csv�t�@�C����0��ڂ̕�������擾
	std::string end = "end";

	//0��ڂ̕������n���A���̍s�̃p�����[�^���擾
	std::vector<float> enddata = GetCSVReadData(csv_end, end);

	//�������̏��Ԃ�csv�̊e�s�̏��Ԃɍ��킹��
	//v�̓Y������namespace�Ő錾�����񋓌^���g�p
	UpperEnd_ = enddata[i_up];
	LowerEnd_ = enddata[i_down];
	RightEnd_ = enddata[i_right];
	LeftEnd_ = enddata[i_left];
	PillarNum_ = static_cast<int>(enddata[i_pillarnum]);


	//----------�����蔻��̈ʒu�A�T�C�Y,�@���̏�����----------

	//csv�t�@�C����ǂݍ���
	CsvReader csv_wire;
	csv_wire.Load("CSVdata\\StageData\\StageWireData.csv");

	//csv�t�@�C���̊e0��ڂ̕�����̔z����擾
	std::string collisionName[] = { "UpperPos","LowerPos","RightPos","LeftPos",
	"UpperSize","LowerSize","RightSize","LeftSize",
	"UpperNormal","LowerNormal","RightNormal","LeftNormal"};

	//�e�����蔻��p��XMFLOAT3�ϐ��̎Q�Ƃ��|�C���^�z��Ɋi�[
	XMFLOAT3* CollisionFloat[] = { &WirePosUpper_ ,&WirePosLower_,&WirePosRight_,&WirePosLeft_,
	&WireSizeUpper_,&WireSizeLower_,&WireSizeRight_,&WireSizeLeft_,
	&UpperNormal_,&LowerNormal_,&RightNormal_,&LeftNormal_ };

	for (int i = 0; i < sizeof(collisionName) / sizeof(collisionName[0]); i++)
	{
		//0��ڂ̕������n���A���̍s�̃p�����[�^���擾
		std::vector<float> Collisiondata = GetCSVReadData(csv_wire, collisionName[i]);

		//�������̏��Ԃ�csv�̊e�s�̏��Ԃɍ��킹��
		//v�̓Y������namespace�Ő錾�����񋓌^���g�p
	    CollisionFloat[i]->x = Collisiondata[i_collisionX];
		CollisionFloat[i]->y = Collisiondata[i_collisionY];
		CollisionFloat[i]->z = Collisiondata[i_collisionZ];
	}

	//----------�X�e�[�W�O�I�u�W�F�N�g�̊e�g�����X�t�H�[��������----------
	//csv�t�@�C����ǂݍ���
	CsvReader csv_out;
	csv_out.Load("CSVdata\\StageData\\OutStageData.csv");

	//csv�t�@�C���̊e0��ڂ̕�����̔z����擾

	std::vector<std::string> Cabins = { "Cabin1" };
	std::vector<std::string> Trees = { "Tree1" , "Tree2" , "Tree3" };
	std::vector<std::string> Logs = { "Logs1" };
	std::vector<std::string> Straws = { "Straw1","Straw2","Straw3"};

	std::vector<std::vector<std::string>> OutStageNames = {
		Cabins, Trees,Logs,Straws
	};

	// �eTransform�z��ւ̎Q��
	std::vector<std::vector<Transform>*> OutStageTrans = {
		&CabinData_,
		&TreeData_,
		&LogsData_,
		&StrawData_
	};

	//���ׂĂ̎�ށE�����ꊇ�ŏ�����
	for (size_t type = 0; type < OutStageNames.size(); ++type) 
	{
		//���O�̐���Transform�z�񐔂���v���Ă���Ȃ瑱����
		if (OutStageNames.size() != OutStageTrans.size())
		{
			break;
		}

		//�K�v�Ȑ�����Transform���m��
		OutStageTrans[type]->resize(OutStageNames[type].size());

		//�e�g�����X�t�H�[��[�g�����X�t�H�[���̎��][���̎�ނ�i�Ԗ�]
		for (size_t i = 0; i < OutStageNames[type].size(); ++i)
		{
			InitCSVTransform(csv_out, OutStageNames[type][i], (*OutStageTrans[type])[i]);
		}
	}

}

void StageManager::InitGroundData()
{
	//Ground�N���X�̏������s��

	//�n�ʃN���X�̃C���X�^���X���擾
	pGround_ = (Ground*)FindObject("Ground");

	//CSV����ǂݍ��񂾊g�嗦,��],�ʒu��n��
	pGround_->SetScale(GroundData_.scale_);
	pGround_->SetRotate(GroundData_.rotate_);
	pGround_->SetPosition(GroundData_.position_);

	//GameView�ɒn�ʃN���X�̃|�C���^��n��
	//ImGui�`��p
	GameView::SetGround(pGround_);
}

void StageManager::InitFenceData()
{
	//Fence�N���X�̃g�����X�t�H�[���֌W�̏������s��

	//��N���X�̃C���X�^���X���擾
	pFence_ = (Fence*)FindObject("Fence");

	//CSV����ǂݍ��񂾍�,�S���̃g�����X�t�H�[����n��
	pFence_->InitWireTransform(WireData_);
	pFence_->InitPillarTransform(PillarData_);

	//GameView�ɍ�N���X�̃|�C���^��n��
	//ImGui�`��p
	GameView::SetFence(pFence_);
}

void StageManager::InitEndData()
{
	//Fence�N���X�̒��̈ʒu�A�@���̕�����������

	//��N���X�̃C���X�^���X���擾
	Fence* pFence = (Fence*)FindObject("Fence");

	//���̐��A���̈ʒu��������
	pFence->SetPillarNum(PillarNum_);
	pFence->SetPillar(UpperEnd_,LowerEnd_,RightEnd_,LeftEnd_, PillarData_.position_.y);

	//�e�S���̏Փ˔����������(�����蔻��̈ʒu,�����蔻��̃T�C�Y,�S�������@��)
	pFence->SetWireCollisionUpper(WirePosUpper_, WireSizeUpper_, UpperNormal_);
	pFence->SetWireCollisionLower(WirePosLower_, WireSizeLower_,LowerNormal_);
	pFence->SetWireCollisionRight(WirePosRight_, WireSizeRight_, RightNormal_);
	pFence->SetWireCollisionLeft(WirePosLeft_, WireSizeLeft_, LeftNormal_);

}

void StageManager::InitOutStageThingData()
{
	//�X�e�[�W�O�I�u�W�F�N�g��������

	//OutStageThing�̃C���X�^���X���擾
	pOutStageThing_ = (OutStageThing*)FindObject("OutStageThing");

	//CSV����ǂݍ��񂾏����A�؁A�ۑ��̃g�����X�t�H�[����n��
	pOutStageThing_->SetCabinTransform(CabinData_);
	pOutStageThing_->SetTreeTransform(TreeData_);
	pOutStageThing_->SetLogsTransform(LogsData_);
	pOutStageThing_->SetStrawTransform(StrawData_);

	GameView::SetOutStageThing(pOutStageThing_);
}