#include "CharacterCsvLoader.h"

namespace {

	//csv�ǂݍ��ݎ��̃C���f�b�N�X
	//�L�����N�^�[�N���X�̋��ʕϐ���������

	//�ړ��֌W
	enum MoveIndex
	{
		i_Velocity = 0,
		i_AcceleValue,
		i_AcceleMax,
		i_Friction,
		i_ArrowRotateX,
		i_ArrowRotateY,
		i_ArrowRotateZ,
		i_ArrowScaleX,
		i_ArrowScaleY,
		i_ArrowScaleZ,
		i_AddArrowDepth
	};

	//��]�֌W
	enum RotateIndex
	{
		i_MoveRotate = 0,
		i_FastRotate,
	};

	//�󒆊֌W
	enum JumpIndex
	{
		i_Gravity = 0,
		i_JumpHeight,
		i_UpperLimit,
		i_LowerLimit,
		i_MinusLimit,
	};

	//��e�֌W
	enum HitIndex
	{
		i_Collider = 0,
		i_OriginalRangeMin,
		i_OriginalRangeMax,
		i_ConvertedRangeMin,
		i_ConvertedRangeMax,
		i_DecelerationRate,
		i_KnockBackEnd,
	};

	//��ɐڐG�֌W
	enum FenceHitIndex
	{
		i_KnockBackPower = 0,
		i_InvincibilityValue,
		i_BlinkValue,

	};

	//�e�t���֌W
	enum ShadowIndex
	{
		i_ShadowCorrection = 0,
	};

}

CharacterCsvLoader::CharacterCsvLoader(GameObject* parent)
	:GameObject(parent, "CharacterCsvLoader")
{
}

void CharacterCsvLoader::SetCSVStatus(std::string _path)
{
	//csv�t�@�C����ǂݍ���
	CsvReader csv;
	csv.Load(_path);

	//--------------------�������֌W�̃p�����[�^(���g��transform_,���ʃx�N�g��)--------------------

	//csv�t�@�C����0��ڂ̕�������擾
	std::string p_init = "InitializeParam";

	//0��ڂ̕������n���A���̍s�̃p�����[�^���擾
	std::vector<float> initData = GetCSVReadData(csv, p_init);

	//���g�̃g�����X�t�H�[����������
	SetTransformPRS(this->transform_, initData);

	//�����ʒu��ۊǂ���
	InitParam_.StartPosition_ = this->transform_.position_;

	//--------------------�ړ��֌W�̃p�����[�^--------------------

	//csv�t�@�C����0��ڂ̕�������擾
	std::string p_move = "MoveParam";

	//0��ڂ̕������n���A���̍s�̃p�����[�^���擾
	std::vector<float> MoveData = GetCSVReadData(csv, p_move);

	//�������̏��Ԃ�csv�̊e�s�̏��Ԃɍ��킹��
	//v�̓Y������namespace�Ő錾�����񋓌^���g�p
	MoveParam_.Velocity_ = MoveData[i_Velocity];
	MoveParam_.AcceleValue_ = MoveData[i_AcceleValue];
	MoveParam_.FullAccelerate_ = MoveData[i_AcceleMax];
	MoveParam_.Friction_ = MoveData[i_Friction];
	MoveParam_.ArrowRotate_ = { MoveData[i_ArrowRotateX],MoveData[i_ArrowRotateY],MoveData[i_ArrowRotateZ] };
	MoveParam_.ArrowScale_ = { MoveData[i_ArrowScaleX],MoveData[i_ArrowScaleY],MoveData[i_ArrowScaleZ] };
	MoveParam_.AddArrowDepth_ = MoveData[i_AddArrowDepth];

	//--------------------��]�֌W�̃p�����[�^--------------------

	//csv�t�@�C����0��ڂ̕�������擾
	std::string p_rotate = "RotateParam";

	//0��ڂ̕������n���A���̍s�̃p�����[�^���擾
	std::vector<float> RotData = GetCSVReadData(csv, p_rotate);

	//�������̏��Ԃ�csv�̊e�s�̏��Ԃɍ��킹��
	//v�̓Y������namespace�Ő錾�����񋓌^���g�p
	RotateParam_.MoveRotateX = RotData[i_MoveRotate];
	RotateParam_.FastRotateX = RotData[i_FastRotate];


	//--------------------�󒆊֌W�̃p�����[�^--------------------

	//csv�t�@�C����0��ڂ̕�������擾	
	std::string p_jump = "JumpParam";

	//0��ڂ̕������n���A���̍s�̃p�����[�^���擾
	std::vector<float> JumpData = GetCSVReadData(csv, p_jump);

	//�������̏��Ԃ�csv�̊e�s�̏��Ԃɍ��킹��
	//v�̓Y������namespace�Ő錾�����񋓌^���g�p
	JumpParam_.Gravity_ = JumpData[i_Gravity];
	JumpParam_.JumpHeight = JumpData[i_JumpHeight];
	JumpParam_.HeightLowerLimit_ = JumpData[i_UpperLimit];
	JumpParam_.HeightUpperLimit_ = JumpData[i_LowerLimit];
	JumpParam_.MinusLimit_ = JumpData[i_MinusLimit];

	//--------------------��e�֌W�̃p�����[�^--------------------

	//csv�t�@�C����0��ڂ̕�������擾	
	std::string p_hit = "HitParam";

	//0��ڂ̕������n���A���̍s�̃p�����[�^���擾
	std::vector<float> HitData = GetCSVReadData(csv, p_hit);

	//�������̏��Ԃ�csv�̊e�s�̏��Ԃɍ��킹��
	//v�̓Y������namespace�Ő錾�����񋓌^���g�p
	HitParam_.ColliderSize_ = HitData[i_Collider];
	HitParam_.OriginalRangeMin_ = HitData[i_OriginalRangeMin];
	HitParam_.OriginalRangeMax_ = HitData[i_OriginalRangeMax];
	HitParam_.ConvertedRangeMin_ = HitData[i_ConvertedRangeMin];
	HitParam_.ConvertedRangeMax_ = HitData[i_ConvertedRangeMax];
	HitParam_.DecelerationRate_ = HitData[i_DecelerationRate];
	HitParam_.KnockBackEnd_ = HitData[i_KnockBackEnd];

	//--------------------��ɐڐG�֌W�̃p�����[�^--------------------

	//csv�t�@�C����0��ڂ̕�������擾	
	std::string p_fencehit = "FenceHitParam";

	//0��ڂ̕������n���A���̍s�̃p�����[�^���擾
	std::vector<float> FenceHitData = GetCSVReadData(csv, p_fencehit);

	//�������̏��Ԃ�csv�̊e�s�̏��Ԃɍ��킹��
	//v�̓Y������namespace�Ő錾�����񋓌^���g�p
	FenceHitParam_.KnockBackPower_ = FenceHitData[i_KnockBackPower];
	FenceHitParam_.InvincibilityValue_ = static_cast<int>(FenceHitData[i_InvincibilityValue]);
	FenceHitParam_.BlinkValue_ = static_cast<int>(FenceHitData[i_BlinkValue]);



	//--------------------�e�֌W�̃p�����[�^--------------------

	//csv�t�@�C����0��ڂ̕�������擾	
	std::string p_shadow = "ShadowParam";

	//0��ڂ̕������n���A���̍s�̃p�����[�^���擾
	std::vector<float> ShadowData = GetCSVReadData(csv, p_shadow);

	//�������̏��Ԃ�csv�̊e�s�̏��Ԃɍ��킹��
	//v�̓Y������namespace�Ő錾�����񋓌^���g�p
	ShadowParam_.ShadowCorrection_ = ShadowData[i_ShadowCorrection];
}