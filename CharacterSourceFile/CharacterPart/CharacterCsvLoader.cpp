#include "CharacterCsvLoader.h"
#include "../Character.h" 

namespace {

	//csv�ǂݍ��ݎ��̃C���f�b�N�X
	//�L�����N�^�[�N���X�̋��ʕϐ���������

	//�ړ��֌W
	enum MoveIndex
	{
		//�ʏ�ړ�
		i_NormalVelocity = 0,
		i_NormalAcceleValue,
		i_NormalFullAccelerate_,

		//�_�b�V���U��
		i_AttackAcceleValue_,
		i_AttackFullAccelerate_,

		//���֌W
		i_ArrowRotateX,
		i_ArrowRotateY,
		i_ArrowRotateZ,
		i_ArrowScaleX,
		i_ArrowScaleY,
		i_ArrowScaleZ,
		i_AddArrowDepth,

		//����
		i_Friction,
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
	:GameObject(parent, "CharacterCsvLoader"), params_(nullptr), character_(nullptr)
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
	Transform tmp;
	SetTransformPRS(tmp, initData);

	//�e�N���X�̃g�����X�t�H�[���ɔ��f
	character_->SetPosition(tmp.position_);
	character_->SetRotate(tmp.rotate_);
	character_->SetScale(tmp.scale_);

	//�����ʒu��ۊǂ���
	params_->InitParam_.StartPosition_ = tmp.position_;

	//--------------------�ړ��֌W�̃p�����[�^--------------------

	//csv�t�@�C����0��ڂ̕�������擾
	std::string p_move = "MoveParam";

	//0��ڂ̕������n���A���̍s�̃p�����[�^���擾
	std::vector<float> MoveData = GetCSVReadData(csv, p_move);

	//�������̏��Ԃ�csv�̊e�s�̏��Ԃɍ��킹��
	//v�̓Y������namespace�Ő錾�����񋓌^���g�p
	params_->MoveParam_.NormalVelocity_ = MoveData[i_NormalVelocity];
	params_->MoveParam_.NormalAcceleValue_ = MoveData[i_NormalAcceleValue];
	params_->MoveParam_.NormalFullAccelerate_ = MoveData[i_NormalFullAccelerate_];
	params_->MoveParam_.AttackAcceleValue_ = MoveData[i_AttackAcceleValue_];
	params_->MoveParam_.AttackFullAccelerate_ = MoveData[i_AttackFullAccelerate_];
	params_->MoveParam_.ArrowRotate_ = { MoveData[i_ArrowRotateX],MoveData[i_ArrowRotateY],MoveData[i_ArrowRotateZ] };
	params_->MoveParam_.ArrowScale_ = { MoveData[i_ArrowScaleX],MoveData[i_ArrowScaleY],MoveData[i_ArrowScaleZ] };
	params_->MoveParam_.AddArrowDepth_ = MoveData[i_AddArrowDepth];
	params_->MoveParam_.Friction_ = MoveData[i_Friction];

	//--------------------��]�֌W�̃p�����[�^--------------------

	//csv�t�@�C����0��ڂ̕�������擾
	std::string p_rotate = "RotateParam";

	//0��ڂ̕������n���A���̍s�̃p�����[�^���擾
	std::vector<float> RotData = GetCSVReadData(csv, p_rotate);

	//�������̏��Ԃ�csv�̊e�s�̏��Ԃɍ��킹��
	//v�̓Y������namespace�Ő錾�����񋓌^���g�p
	params_->RotateParam_.MoveRotateX = RotData[i_MoveRotate];
	params_->RotateParam_.FastRotateX = RotData[i_FastRotate];


	//--------------------�󒆊֌W�̃p�����[�^--------------------

	//csv�t�@�C����0��ڂ̕�������擾	
	std::string p_jump = "JumpParam";

	//0��ڂ̕������n���A���̍s�̃p�����[�^���擾
	std::vector<float> JumpData = GetCSVReadData(csv, p_jump);

	//�������̏��Ԃ�csv�̊e�s�̏��Ԃɍ��킹��
	//v�̓Y������namespace�Ő錾�����񋓌^���g�p
	params_->JumpParam_.Gravity_ = JumpData[i_Gravity];
	params_->JumpParam_.JumpHeight = JumpData[i_JumpHeight];
	params_->JumpParam_.HeightLowerLimit_ = JumpData[i_UpperLimit];
	params_->JumpParam_.HeightUpperLimit_ = JumpData[i_LowerLimit];
	params_->JumpParam_.MinusLimit_ = JumpData[i_MinusLimit];

	//--------------------��e�֌W�̃p�����[�^--------------------

	//csv�t�@�C����0��ڂ̕�������擾	
	std::string p_hit = "HitParam";

	//0��ڂ̕������n���A���̍s�̃p�����[�^���擾
	std::vector<float> HitData = GetCSVReadData(csv, p_hit);

	//�������̏��Ԃ�csv�̊e�s�̏��Ԃɍ��킹��
	//v�̓Y������namespace�Ő錾�����񋓌^���g�p
	params_->HitParam_.ColliderSize_ = HitData[i_Collider];
	params_->HitParam_.OriginalRangeMin_ = HitData[i_OriginalRangeMin];
	params_->HitParam_.OriginalRangeMax_ = HitData[i_OriginalRangeMax];
	params_->HitParam_.ConvertedRangeMin_ = HitData[i_ConvertedRangeMin];
	params_->HitParam_.ConvertedRangeMax_ = HitData[i_ConvertedRangeMax];
	params_->HitParam_.DecelerationRate_ = HitData[i_DecelerationRate];
	params_->HitParam_.KnockBackEnd_ = HitData[i_KnockBackEnd];

	//--------------------��ɐڐG�֌W�̃p�����[�^--------------------

	//csv�t�@�C����0��ڂ̕�������擾	
	std::string p_fencehit = "FenceHitParam";

	//0��ڂ̕������n���A���̍s�̃p�����[�^���擾
	std::vector<float> FenceHitData = GetCSVReadData(csv, p_fencehit);

	//�������̏��Ԃ�csv�̊e�s�̏��Ԃɍ��킹��
	//v�̓Y������namespace�Ő錾�����񋓌^���g�p
	params_->FenceHitParam_.KnockBackPower_ = FenceHitData[i_KnockBackPower];
	params_->FenceHitParam_.InvincibilityValue_ = static_cast<int>(FenceHitData[i_InvincibilityValue]);
	params_->FenceHitParam_.BlinkValue_ = static_cast<int>(FenceHitData[i_BlinkValue]);



	//--------------------�e�֌W�̃p�����[�^--------------------

	//csv�t�@�C����0��ڂ̕�������擾	
	std::string p_shadow = "ShadowParam";

	//0��ڂ̕������n���A���̍s�̃p�����[�^���擾
	std::vector<float> ShadowData = GetCSVReadData(csv, p_shadow);

	//�������̏��Ԃ�csv�̊e�s�̏��Ԃɍ��킹��
	//v�̓Y������namespace�Ő錾�����񋓌^���g�p
	params_->ShadowParam_.ShadowCorrection_ = ShadowData[i_ShadowCorrection];
}