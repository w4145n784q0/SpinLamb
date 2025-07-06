#include "StageManager.h"
#include"Engine/CsvReader.h"
#include"Engine/Global.h"

namespace
{
	//csv�ǂݍ��ݎ��̃C���f�b�N�X(�e�X�e�[�W�̒[)
	enum GroundEndIndex
	{
		i_up = 0,
		i_down,
		i_right,
		i_left,
		i_pillernum,
	};

	//csv�ǂݍ��ݎ��̃C���f�b�N�X(�����蔻��̓ǂݍ���)
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
	//Stage�֌W�̒l�͊�{StageManager�N���X�œǂݍ��݁A�n��

	//csv����p�����[�^�ǂݍ���
	SetStageInitSCV();

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

void StageManager::SetStageInitSCV()
{

	//----------�n��,�S��,���̊e�g�����X�t�H�[��������----------

	//csv�t�@�C����ǂݍ���
	CsvReader csv;
	csv.Load("CSVdata\\StageData\\StageData.csv");

	//csv�t�@�C���̊e0��ڂ̕�����̔z����擾
	std::vector<std::string> ParamNames = {
		"Ground","wire","piller"
	};

	//�e�g�����X�t�H�[����z��ɓ����
	std::vector<std::reference_wrapper<Transform>> TransformArray = {
		GroundData_,WireData_,PillerData_
	};

	//�܂Ƃ߂ď�����
	InitCSVTransformArray(csv, ParamNames, TransformArray);


	//----------�X�e�[�W�̊e�[�̈ʒu�A���̖{��----------

	//csv�t�@�C����ǂݍ���
	CsvReader csv_end;
	csv_end.Load("CSVdata\\StageData\\StageEndData.csv");

	//csv�t�@�C����0��ڂ̕�������擾
	std::string end = "end";

	//�w�肵�������񂪂����ꂩ��0��ڂɑ��݂�����
	if (csv_end.IsGetParamName(end))
	{
		//���̍s��z��Ƃ��đS�擾
		std::vector<float> v = csv_end.GetParam(end);
		
		//�������̏��Ԃ�csv�̊e�s�̏��Ԃɍ��킹��
		//v�̓Y������namespace�Ő錾�����񋓌^���g�p
		UpperEnd_ = v[i_up];
		LowerEnd_ = v[i_down];
		RightEnd_ = v[i_right];
		LeftEnd_ = v[i_left];
		PillerNum_ = static_cast<int>(v[i_pillernum]);
	}


	//----------�����蔻��̈ʒu�A�T�C�Y,�@���̏�����----------

	//csv�t�@�C����ǂݍ���
	CsvReader csv_wire;
	csv_wire.Load("CSVdata\\StageData\\StageWireData.csv");

	//csv�t�@�C���̊e0��ڂ̕�����̔z����擾
	std::string collision[] = { "UpperPos","LowerPos","RightPos","LeftPos",
	"UpperSize","LowerSize","RightSize","LeftSize",
	"UpperNormal","LowerNormal","RightNormal","LeftNormal"};

	//�e�����蔻��p��XMFLOAT3�ϐ��̎Q�Ƃ��|�C���^�z��Ɋi�[
	XMFLOAT3* CollisionData[] = { &WirePosUpper_ ,&WirePosLower_,&WirePosRight_,&WirePosLeft_,
	&WireSizeUpper_,&WireSizeLower_,&WireSizeRight_,&WireSizeLeft_,
	&UpperNormal_,&LowerNormal_,&RightNormal_,&LeftNormal_ };

	for (int i = 0; i < sizeof(collision) / sizeof(collision[0]); i++)
	{
		//�w�肵�������񂪂����ꂩ��0��ڂɑ��݂�����
		if (csv_wire.IsGetParamName(collision[i]))
		{
			//���̍s��z��Ƃ��đS�擾
			std::vector<float> v = csv_wire.GetParam(collision[i]);

			//�������̏��Ԃ�csv�̊e�s�̏��Ԃɍ��킹��
			//v�̓Y������namespace�Ő錾�����񋓌^���g�p
	        CollisionData[i]->x = v[i_collisionX];
			CollisionData[i]->y = v[i_collisionY];
			CollisionData[i]->z = v[i_collisionZ];
		}
	}

	//----------�X�e�[�W�O�I�u�W�F�N�g�̊e�g�����X�t�H�[��������----------
	//csv�t�@�C����ǂݍ���
	CsvReader csv_out;
	csv_out.Load("CSVdata\\StageData\\OutStageData.csv");

	//csv�t�@�C���̊e0��ڂ̕�����̔z����擾

	std::vector<std::string> Cabins = { "Cabin1" };
	std::vector<std::string> Trees = { "Tree1","Tree2" };
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
	Ground* pGround = (Ground*)FindObject("Ground");

	//CSV����ǂݍ��񂾊g�嗦,��],�ʒu��n��
	pGround->SetScale(GroundData_.scale_);
	pGround->SetRotate(GroundData_.rotate_);
	pGround->SetPosition(GroundData_.position_);
}

void StageManager::InitFenceData()
{
	//Fence�N���X�̃g�����X�t�H�[���֌W�̏������s��

	//��N���X�̃C���X�^���X���擾
	Fence* pFence = (Fence*)FindObject("Fence");

	//CSV����ǂݍ��񂾍�,�S���̃g�����X�t�H�[����n��
	pFence->InitWireTransform(WireData_);
	pFence->InitPillerTransform(PillerData_);

}

void StageManager::InitEndData()
{
	//Fence�N���X�̒��̈ʒu�A�@���̕�����������

	//��N���X�̃C���X�^���X���擾
	Fence* pFence = (Fence*)FindObject("Fence");

	//���̐��A���̈ʒu��������
	pFence->SetPillerNum(PillerNum_);
	pFence->SetPiller(UpperEnd_,LowerEnd_,RightEnd_,LeftEnd_, PillerData_.position_.y);

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
	OutStageThing* pOutStageThing = (OutStageThing*)FindObject("OutStageThing");

	//CSV����ǂݍ��񂾏����A�؁A�ۑ��̃g�����X�t�H�[����n��
	pOutStageThing->SetCabinTransform(CabinData_);
	pOutStageThing->SetTreeTransform(TreeData_);
	pOutStageThing->SetLogsTransform(LogsData_);
	pOutStageThing->SetStrawTransform(StrawData_);
}