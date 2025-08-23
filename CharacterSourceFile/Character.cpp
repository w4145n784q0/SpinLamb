#include "Character.h"
#include"../Engine/VFX.h"
#include"../Engine/Model.h"
#include"../Engine/Audio.h"
#include"../Engine/CsvReader.h"
#include"../StageSourceFile/UpperWire.h"
#include"../StageSourceFile/LowerWire.h"
#include"../StageSourceFile/LeftWire.h"
#include"../StageSourceFile/RightWire.h"


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


Character::Character(GameObject* parent)
	:GameObject(parent,"Character")
{
}

Character::Character(GameObject* parent, const std::string& name)
	:GameObject(parent, name)
{
	//csv����p�����[�^�ǂݍ���
	InitCSVEffect();

	//�T�E���h�̓ǂݍ���
	//�����f�B���N�g��������̃p�X�͏ȗ�
	//�p�X�̈ꕔ�𕶎���ɂ��A����������
	std::string SoundSE = "Sound\\SE\\";

	hSoundcharge_ = Audio::Load(SoundSE + "Charge.wav",false, Audio::GetChargeNum());
	assert(hSoundcharge_ >= 0);

	hSoundattack_ = Audio::Load(SoundSE + "Attack.wav", false, Audio::GetAttackNum());
	assert(hSoundattack_ >= 0);

	hSoundCollision_ = Audio::Load(SoundSE + "Collision.wav",false, Audio::GetCollisionNum());
	assert(hSoundCollision_ >= 0);

	hSoundJump_ = Audio::Load(SoundSE + "Jump.wav", false, Audio::GetJumpNum());
	assert(hSoundJump_ >= 0);

	//���ꂼ��̍�̖@�����擾
	GetWireNormal();

	//�e���f����������
	InitShadow();
}

Character::~Character()
{
	//���̂͏������A�A�h���X�̂ݖ���������
	for (auto& observer : InitParam_.observers)
	{
		if(observer != nullptr)
		{
			//�Ď��Ώۂ̔z�񂩂�폜
			//�ʒm��(Character)���Ď���(BattleScene)�̏��ɉ�������̂ŁA
			//Character�̃f�X�g���N�^��RemoveObserver���Ă�
			RemoveObserver(observer);

			//���S��Ƃ��ăA�h���X�𖳌���
			observer = nullptr;
		}
	}

	if(ShadowParam_.pGround_ != nullptr)
	{
		ShadowParam_.pGround_ = nullptr;
	}
}

void Character::Update()
{
	//Character�N���X���p������N���X�ŌĂԋ��ʍX�V

	//���t���[���e�̈ʒu���X�V
	ShadowSet();

	//���t���[���d�͂�����������
	CharacterGravity();

	//�����̑O���x�N�g��(��]���������܂�)���X�V
	FrontVectorConfirm();
}

void Character::Draw()
{
	//Character�N���X���p������N���X�ŌĂԋ��ʕ`��

	ShadowDraw();
}

void Character::SetCSVStatus(std::string _path)
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
	SetTransformPRS(this->transform_,initData);

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

void Character::GetWireNormal()
{
	//�e�C���X�^���X�����̖@�����擾
	UpperWire* pUpperWire = (UpperWire*)FindObject("UpperWire");
	assert(pUpperWire != nullptr);
	FenceHitParam_.UpperNormal_ = pUpperWire->GetNormal();

	LowerWire* pLowerWire = (LowerWire*)FindObject("LowerWire");
	assert(pLowerWire != nullptr);
	FenceHitParam_.LowerNormal_ = pLowerWire->GetNormal();

	RightWire* pRightWire = (RightWire*)FindObject("RightWire");
	assert(pRightWire != nullptr);
	FenceHitParam_.RightNormal_ = pRightWire->GetNormal();

	LeftWire* pLeftWire = (LeftWire*)FindObject("LeftWire");
	assert(pLeftWire != nullptr);
	FenceHitParam_.LeftNormal_ = pLeftWire->GetNormal();

	//�擾�����@����z��Ɋi�[
	//����炪��ɐڐG�����ۂ̒��˕Ԃ�����ɂȂ�
	FenceHitParam_.NormalArray_ = { FenceHitParam_.UpperNormal_,  FenceHitParam_.LowerNormal_,
		 FenceHitParam_.RightNormal_, FenceHitParam_.LeftNormal_, };
}

void Character::InitArrow()
{
	//��󃂃f���̃g�����X�t�H�[����������
	MoveParam_.ArrowTransform_.position_ = { 0.0f,0.0f, 0.0f };
	MoveParam_.ArrowTransform_.rotate_ = MoveParam_.ArrowRotate_;
	MoveParam_.ArrowTransform_.scale_ = MoveParam_.ArrowScale_;

	//��󃂃f���ǂݍ���
	MoveParam_.hMoveArrow_ = Model::Load("Model\\AttackArrow.fbx");
	assert(MoveParam_.hMoveArrow_ >= 0);
}

void Character::AddObserver(IGameObserver* _observer)
{
	//�Ď��҂�z��ɒǉ�����
	InitParam_.observers.push_back(_observer);
}

void Character::RemoveObserver(IGameObserver* _observer)
{
	auto it = InitParam_.observers.begin();

	while (it != InitParam_.observers.end())
	{
		if (*it == _observer)
		{
			//�Ď������Ώۂ̔z�񃋁[�v���ŁA�Ď��Ώۂ���������폜
			//�C�e���[�^�폜��͎��̗v�f���Q�Ƃ���
			it = InitParam_.observers.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void Character::DrawCharacterModel(int _handle, Transform _transform)
{
	//���G���Ԓ����ǂ����Ń��f���̓_�ŕ\�����s��

	if (FenceHitParam_.IsInvincibility_)
	{
		//���G���Ԓ��Ȃ�^�C�}�[���g���A���t���[�������Ƀ��f����`��
		if (++FenceHitParam_.BlinkTimer_ > FenceHitParam_.BlinkValue_) {

			FenceHitParam_.BlinkTimer_ = 0;
			Model::SetAndDraw(_handle, _transform);
		}
	}
	else
	{
		//���G���ԂłȂ��Ȃ�ʏ�̕`��
		Model::SetAndDraw(_handle, _transform);
	}
}

void Character::DrawCharacterImGui()
{
	//ImGui�̕`��
	//Slider:�X���C�_�[���g���Ēl�𒲐��\
	//Input:�l�𒼐ړ��� +-�ɂ�钲�����\
	//Text:�L�q�̂�

	//�L�����N�^�[�̈ʒu(position_.x,y,z)
	if (ImGui::TreeNode("Transform.Position"))
	{
		ImGui::SliderFloat("PositionX", &this->transform_.position_.x, WestEnd_, EastEnd_);
		ImGui::SliderFloat("PositionY", &this->transform_.position_.y, JumpParam_.HeightLowerLimit_,
			JumpParam_.HeightUpperLimit_);
		ImGui::SliderFloat("PositionZ", &this->transform_.position_.z, SouthEnd_, NorthEnd_);
		ImGui::TreePop();
	}

	//�L�����N�^�[�̉�]��(rotate_.x,y,z)
	if (ImGui::TreeNode("Transform.Rotate"))
	{
		ImGui::InputFloat("RotateX", &this->transform_.rotate_.x, ZeroPointOne);
		ImGui::InputFloat("RotateY", &this->transform_.rotate_.y, ZeroPointOne);
		ImGui::InputFloat("RotateZ", &this->transform_.rotate_.z, ZeroPointOne);
		ImGui::TreePop();
	}

	//���[���h���ʃx�N�g��
	if (ImGui::TreeNode("Forward"))
	{
		XMFLOAT3 tmp;
		XMStoreFloat3(&tmp, MoveParam_.ForwardVector_);
		ImGui::Text("ForwardX:%.3f", tmp.x);
		ImGui::Text("ForwardY:%.3f", tmp.y);
		ImGui::Text("ForwardZ:%.3f", tmp.z);
		ImGui::TreePop();
	}

	//�L�����N�^�[ID,�����ʒu
	if (ImGui::TreeNode("Initialize"))
	{
		ImGui::Text("CharacterID:%.1d", InitParam_.CharacterID);
		ImGui::Text("ForwardX:%.3f", InitParam_.StartPosition_.x);
		ImGui::Text("ForwardY:%.3f", InitParam_.StartPosition_.y);
		ImGui::Text("ForwardZ:%.3f", InitParam_.StartPosition_.z);
		ImGui::TreePop();
	}

	//�����x,������,�`���[�W�̍ۂɎg�����̉����x,1f���Ƃ̑�����,�ő����,���C�W��,
	//�ړ������A�ړ���̍X�V�ʒu
	if (ImGui::TreeNode("Move"))
	{
		ImGui::InputFloat("Velocity", &this->MoveParam_.Velocity_, ZeroPointOne);
		ImGui::InputFloat("Acceleration", &this->MoveParam_.Acceleration_, ZeroPointOne);
		ImGui::InputFloat("TmpAccele", &this->MoveParam_.TmpAccele_, ZeroPointOne);
		ImGui::InputFloat("AcceleValue", &this->MoveParam_.AcceleValue_, ZeroPointOne);
		ImGui::InputFloat("FullAccelerate", &this->MoveParam_.FullAccelerate_, ZeroPointOne);
		ImGui::InputFloat("Friction", &this->MoveParam_.Friction_, ZeroPointOne);

		XMFLOAT3 move;
		XMStoreFloat3(&move, MoveParam_.MoveDirection_);
		ImGui::Text("MoveDirectionX:%.3f", move.x);
		ImGui::Text("MoveDirectionY:%.3f", move.y);
		ImGui::Text("MoveDirectionZ:%.3f", move.z);

		XMFLOAT3 NewPos;
		XMStoreFloat3(&NewPos, MoveParam_.NewPosition_);
		ImGui::Text("NewPositionX:%.3f", NewPos.x);
		ImGui::Text("NewPositionY:%.3f", NewPos.y);
		ImGui::Text("NewPositionZ:%.3f", NewPos.z);

		ImGui::TreePop();
	}

	//�L�����N�^�[�ړ����̒ʏ�̉�]�ʁA�`���[�W���̉�]��
	if (ImGui::TreeNode("Rotate"))
	{
		ImGui::InputFloat("NormalRotate", &this->RotateParam_.MoveRotateX, ZeroPointOne);
		ImGui::InputFloat("FastRotate", &this->RotateParam_.FastRotateX, ZeroPointOne);
		ImGui::TreePop();
	}

	//�L�����N�^�[�ɂ�����d��,�L�����N�^�[�̏�����̗́A�W�����v�̍���
	// �����̏��,����
	if (ImGui::TreeNode("Jump"))
	{
		ImGui::InputFloat("Gravity", &this->JumpParam_.Gravity_, ZeroPointOne);
		ImGui::InputFloat("JumpSpeed", &this->JumpParam_.JumpSpeed_, ZeroPointOne);
		ImGui::InputFloat("JumpHeight", &this->JumpParam_.JumpHeight, ZeroPointOne);
		ImGui::InputFloat("HeightLowerLimit", &this->JumpParam_.HeightLowerLimit_, ZeroPointOne);
		ImGui::InputFloat("HeightUpperLimit", &this->JumpParam_.HeightUpperLimit_, ZeroPointOne);
		ImGui::TreePop();
	}

	//�m�b�N�o�b�N�ʌv�Z���̕ϊ��O�͈̔�,�ϊ���͈̔�,������,�m�b�N�o�b�N���I������l
	//�m�b�N�o�b�N��������A�m�b�N�o�b�N�̑��x
	if (ImGui::TreeNode("Hit"))
	{
		ImGui::InputFloat("OriginalRangeMin", &this->HitParam_.OriginalRangeMin_, ZeroPointOne);
		ImGui::InputFloat("OriginalRangeMax", &this->HitParam_.OriginalRangeMax_, ZeroPointOne);
		ImGui::InputFloat("ConvertedRangeMin", &this->HitParam_.ConvertedRangeMin_, ZeroPointOne);
		ImGui::InputFloat("ConvertedRangeMax", &this->HitParam_.ConvertedRangeMax_, ZeroPointOne);
		ImGui::InputFloat("DecelerationRate", &this->HitParam_.DecelerationRate_, ZeroPointOne);
		ImGui::InputFloat("KnockBackEnd", &this->HitParam_.KnockBackEnd_, ZeroPointOne);

		ImGui::Text("KnockBack_DirectionX:%.3f", HitParam_.KnockBack_Direction_.x);
		ImGui::Text("KnockBack_DirectionY:%.3f", HitParam_.KnockBack_Direction_.y);
		ImGui::Text("KnockBack_DirectionZ:%.3f", HitParam_.KnockBack_Direction_.z);

		ImGui::Text("KnockBack_VelocityX:%.3f", HitParam_.KnockBack_Velocity_.x);
		ImGui::Text("KnockBack_VelocityY:%.3f", HitParam_.KnockBack_Velocity_.y);
		ImGui::Text("KnockBack_VelocityZ:%.3f", HitParam_.KnockBack_Velocity_.z);

		ImGui::TreePop();
	}

	//��q�b�g���̃m�b�N�o�b�N��,���G����,���G���Ԃ̏I������l
	// ���G���Ԓ��̕`��Ԋu,�`�撆�ɓ_�ŕ\����������l
	if (ImGui::TreeNode("FenceHit"))
	{
		ImGui::InputFloat("KnockBackPower", &this->FenceHitParam_.KnockBackPower_, ZeroPointOne);
		ImGui::InputInt("InvincibilityTime", &this->FenceHitParam_.InvincibilityTime_);
		ImGui::InputInt("InvincibilityValue", &this->FenceHitParam_.InvincibilityValue_);
		ImGui::InputInt("blinkTimer", &this->FenceHitParam_.BlinkTimer_);
		ImGui::InputInt("blinkValue", &this->FenceHitParam_.BlinkValue_);

		ImGui::TreePop();
	}

	//�e�̈ʒu�̕␳
	if (ImGui::TreeNode("Shadow"))
	{
		ImGui::InputFloat("ShadowHeight", &this->ShadowParam_.ShadowHeight_, ZeroPointOne);
		ImGui::InputFloat("ShadowCorrection", &this->ShadowParam_.ShadowCorrection_, ZeroPointOne);
		ImGui::TreePop();
	}

	//��󃂃f���̈ʒu�E��]�E�g�嗦�A�ʒu�̕␳
	if (ImGui::TreeNode("Arrow"))
	{
		ImGui::InputFloat("ArrowTransform.PositionX", &this->MoveParam_.ArrowTransform_.position_.x, ZeroPointOne);
		ImGui::InputFloat("ArrowTransform.PositionY", &this->MoveParam_.ArrowTransform_.position_.y, ZeroPointOne);
		ImGui::InputFloat("ArrowTransform.PositionZ", &this->MoveParam_.ArrowTransform_.position_.z, ZeroPointOne);

		ImGui::InputFloat("ArrowTransform.RotateX", &this->MoveParam_.ArrowTransform_.rotate_.x, ZeroPointOne);
		ImGui::InputFloat("ArrowTransform.RotateY", &this->MoveParam_.ArrowTransform_.rotate_.y, ZeroPointOne);
		ImGui::InputFloat("ArrowTransform.RotateZ", &this->MoveParam_.ArrowTransform_.rotate_.z, ZeroPointOne);

		ImGui::InputFloat("ArrowTransform.ScaleX", &this->MoveParam_.ArrowTransform_.scale_.x, ZeroPointOne);
		ImGui::InputFloat("ArrowTransform.ScaleY", &this->MoveParam_.ArrowTransform_.scale_.y, ZeroPointOne);
		ImGui::InputFloat("ArrowTransform.ScaleZ", &this->MoveParam_.ArrowTransform_.scale_.z, ZeroPointOne);

		ImGui::InputFloat("AddArrowDepth", &this->MoveParam_.AddArrowDepth_, ZeroPointOne);

		ImGui::TreePop();
	}

	ImGui::TreePop();
}

void Character::CharacterGravity()
{
	//�d�͕��̒l�������A�v���C���[�͏�ɉ������ɗ͂��������Ă���
	JumpParam_.JumpSpeed_ -= JumpParam_.Gravity_;

	//�t�B�[���h�ɏ���Ă��邩�͊֌W�Ȃ��d�͂͂����葱����
	this->transform_.position_.y += JumpParam_.JumpSpeed_;

	//�v���C���[�߂肱�ݖh�~�Ɉ��ȉ���y���W�ɂȂ�����l���Œ肷��
	if (this->transform_.position_.y <= JumpParam_.HeightLowerLimit_)
	{
		this->transform_.position_.y = JumpParam_.HeightLowerLimit_;

		//���ȉ���y���W�ɂȂ����ꍇ,���n���Ă��锻��ɂ���
		JumpParam_.IsOnGround_ = true;
	}

	//�}�C�i�X��������̂ŁA�O�̂��߃I�[�o�[�t���[�h�~
	//JumpSpeed���Œ�l�ȉ��ɂȂ�����Œ�l�̒l�ŌŒ�
	if (JumpParam_.JumpSpeed_ <= JumpParam_.MinusLimit_)
	{
		JumpParam_.JumpSpeed_ = JumpParam_.MinusLimit_;
	}
}

void Character::SetJump()
{
	//�W�����v���J�n���鏈��

	//�n�㔻���false�ɂ���
	JumpParam_.IsOnGround_ = false;

	//�ꎞ�I��y�����Ƀ}�C�i�X����Ă���l��傫�����邱�ƂŁA�L�����N�^�[����яオ��
	JumpParam_.JumpSpeed_ = JumpParam_.JumpHeight;

	//�`���[�W����SE�Đ�
	Audio::Play(hSoundJump_);
}

void Character::InitShadow()
{
	//�������̎��_�ŃX�e�[�W�N���X�̃C���X�^���X���擾
	ShadowParam_.pGround_ = (Ground*)FindObject("Ground");
	assert(ShadowParam_.pGround_ != nullptr);

	//�e���f���ǂݍ���
	ShadowParam_.hShadow_ = Model::Load("Model\\ShadowPoint.fbx");
	assert(ShadowParam_.hShadow_ >= 0);
}

void Character::ShadowSet()
{
	//�X�e�[�W�̃��f���ԍ����擾
	int hGroundModel = ShadowParam_.pGround_->GetModelHandle();

	//�X�e�[�W�Ɍ������ă��C���΂��ݒ������
	RayCastData data;
	
	//���C�̔��ˈʒu��ݒ�
	data.start = this->transform_.position_;

	//���C�̕�����ݒ�(0, -1, 0�Ȃ̂ŉ�����)
	data.dir = XMFLOAT3(0, -1, 0);

	//���C�𔭎�
	Model::RayCast(hGroundModel, &data);

	//���C������������
	if (data.hit)
	{
		//���݈ʒu-�Փ˓_�܂ł̋��� + �␳�l�ŉe�̍�����ݒ�
		ShadowParam_.ShadowHeight_ = (this->transform_.position_.y - data.dist) + ShadowParam_.ShadowCorrection_;
	}

	//y���W�ȊO�̓L�����N�^�[�Ɠ����ʒu�ɐݒ�
	this->ShadowParam_.ShadowTrans_.position_.x = this->transform_.position_.x;
	this->ShadowParam_.ShadowTrans_.position_.z = this->transform_.position_.z;

	//�e�̍������g�����X�t�H�[���ɐݒ肷��
	this->ShadowParam_.ShadowTrans_.position_.y = ShadowParam_.ShadowHeight_;
}

void Character::ShadowDraw()
{
	//ShadowSet�ňʒu��ݒ肵���e��`��
	Model::SetAndDraw(ShadowParam_.hShadow_, this->ShadowParam_.ShadowTrans_);
}

void Character::CharacterMove(XMVECTOR _direction)
{
	//�󂯎����������0�x�N�g���Ȃ珈�����Ȃ�(0�x�N�g���𐳋K���͂ł��Ȃ�)
	if (XMVector3Equal(_direction, XMVectorZero()))
	{
		return;
	}

	//�P�ʃx�N�g�������A�ړ��������m��
	MoveParam_.MoveDirection_ = XMVector3Normalize(_direction);

	//�ړ��x�N�g�����쐬
	CreateMoveVector();

	//��O�łȂ���Έʒu�X�V 
	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, MoveParam_.NewPosition_);
	if (!IsOutsideStage(tmp))
	{
		MoveConfirm();
	}
}

void Character::CreateMoveVector()
{
	//�ړ��x�N�g�� = �ړ����� * ((�����x + �����x) * 1f�̈ړ��ʂ̃X�P�[�����O)
	//�ړ��x�N�g��������
	XMVECTOR MoveVector = XMVectorScale(MoveParam_.MoveDirection_, (MoveParam_.Velocity_ + MoveParam_.Acceleration_) * DeltaTime);

	//���݈ʒu�ƈړ��x�N�g�������Z��
	//�ړ���̃x�N�g�����쐬(���̎��_�ł͈ړ��m�肵�Ă��Ȃ�)
	XMVECTOR PrevPos = XMLoadFloat3(&this->transform_.position_);
	MoveParam_.NewPosition_ = PrevPos + MoveVector;
}

bool Character::IsOutsideStage(XMFLOAT3 _position)
{
	//�w��ʒu����ł��X�e�[�W�[�𒴂��邩�ǂ������肵�A�^�U��Ԃ�
	//�X�e�[�W�O���������ۂɎg�p

	if (_position.x > EastEnd_ || _position.x < WestEnd_ || _position.z > NorthEnd_ || _position.z < SouthEnd_)
	{
			return true;
	}
	else 
	{
		return false;
	}
}

void Character::MoveConfirm()
{
	//�ړ���̃x�N�g����transform_.position�ɑ�����A�ړ����m�肷��
	XMStoreFloat3(&this->transform_.position_, MoveParam_.NewPosition_);
}

float Character::RotateDirectionVector(XMVECTOR _MoveVector)
{
	//��ɃR���g���[���[�E�L�[�{�[�h�̓��͂Ɏg��
	//�󂯎���������x�N�g���ƑO�����x�N�g���̊O�ϋ��߂�
	XMVECTOR cross = XMVector3Cross(_MoveVector, InitParam_.FrontDirection_);

	//Y�O�ς��Ƃ�+��-���œ|����]���������߂�
	float crossY = XMVectorGetY(cross);

	//���ʃx�N�g���Ƃ̃��W�A���p���Ƃ�
	XMVECTOR r = XMVector3AngleBetweenVectors(_MoveVector, InitParam_.FrontDirection_);

	//���W�A���p�x���擾
	float angle = XMVectorGetX(r);

	//�f�B�O���[�p�ɒ���
	float angleDeg = XMConvertToDegrees(angle);

	// crossY�̐����ɉ����ĉ�]�p�x�̕�����ς���
	if (crossY > 0)
	{
		angleDeg = -angleDeg;
	}

	return angleDeg;
}

void Character::Reflect(XMVECTOR _myVector, XMVECTOR _targetVector, float _myVelocity, float _targetVelocity)
{
	//���G��ԂȂ珈�����Ȃ�
	if (this->FenceHitParam_.IsInvincibility_)
	{
		return;
	}

	//�ڐG����̃x�N�g�����玩�g�̃x�N�g��������
	XMVECTOR subVector = XMVectorSubtract(_targetVector, _myVector);

	//�������x�N�g����0�Ȃ炨�݂��̈ʒu�������Ȃ̂ŏ������Ȃ�(0�x�N�g���𐳋K���͂ł��Ȃ�)
	if (XMVector3Equal(subVector, XMVectorZero()))
	{
		return;
	}

	//�P�ʃx�N�g���ɕϊ�
	subVector = XMVector3Normalize(subVector);
	
	//�t�x�N�g���ɂ��Ĕ��˕���������
	subVector = XMVectorNegate(subVector);
	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp,subVector);

	//���˕�����ݒ�
	HitParam_.KnockBack_Direction_ = tmp;

	//���g�̑��x�Ƒ���̑��x�̍������Ƃ�
	//���x�̍����ɉ����ăm�b�N�o�b�N�ʂ�ω�������
	float subVelocity = _myVelocity - _targetVelocity;

	//�m�b�N�o�b�N�ʂ̏�����
	//����l���ω�����̂Ń��[�J���ϐ�
	float KnockBackValue = 0.0f;

	//���x���̔���͐��`�⊮���̍ő�l��K�p
	//���x�������g�̕������ȏ�Ȃ�A���g�̃m�b�N�o�b�N�ʂ�0
	if (subVelocity >= HitParam_.OriginalRangeMax_)
	{
		KnockBackValue = 0.0f;
	}
	else 
	{
		//�l���}�C�i�X�Ȃ�v���X�ɕύX
		if (signbit(subVelocity)) 
		{
			subVelocity = -subVelocity;
		}

		//�m�b�N�o�b�N�ʂ̐��`�⊮���s��
		KnockBackValue = LinearInterpolation(subVelocity,
			HitParam_.OriginalRangeMin_, HitParam_.OriginalRangeMax_,
			HitParam_.ConvertedRangeMin_, HitParam_.ConvertedRangeMax_);
	}

	//�ϊ������m�b�N�o�b�N�ʂ��m�b�N�o�b�N���̑��xx,z�ɑ��
	HitParam_.KnockBack_Velocity_.x = KnockBackValue;
	HitParam_.KnockBack_Velocity_.z = KnockBackValue;

	//���߂Ă��鑬�x�����Z�b�g
	ChargeReset();

	//�m�b�N�o�b�N����Y����]�p�̌Œ�
	KnockBackAngleY(HitParam_.KnockBack_Direction_);
}

void Character::KnockBackAngleY(XMFLOAT3 _KnockBackVector)
{
	//�m�b�N�o�b�N����Y���̉�]�p�����߂�
	float angleY = static_cast<float>(atan2(_KnockBackVector.x, _KnockBackVector.z));

	//angleY�̓��W�A���p�Ȃ̂Ńf�B�O���[�p�ɕϊ����AY����]�ɃZ�b�g
	this->transform_.rotate_.y = XMConvertToDegrees(angleY);
}

void Character::KnockBack()
{
	//x����+��]���s��
	MoveRotateX();

	//���t���[���m�b�N�o�b�N���x������
	HitParam_.KnockBack_Velocity_.x *= HitParam_.DecelerationRate_;
	HitParam_.KnockBack_Velocity_.z *= HitParam_.DecelerationRate_;

	//�m�b�N�o�b�N��̈ʒu���v�Z
	//�ʒu = �ʒu + ���� * ���x
	XMFLOAT3 TmpPos = this->transform_.position_;
	TmpPos.x += HitParam_.KnockBack_Direction_.x * HitParam_.KnockBack_Velocity_.x;
	TmpPos.z += HitParam_.KnockBack_Direction_.z * HitParam_.KnockBack_Velocity_.z;

	//���̎��_�ł͕ύX�����A�ړ���̉��̈ʒu�ɕۊ�
	MoveParam_.NewPosition_ = XMLoadFloat3(&TmpPos);

	//��O�łȂ���Έʒu�X�V 
	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, MoveParam_. NewPosition_);
	if (!IsOutsideStage(tmp))
	{
		MoveConfirm();
	}
}

XMVECTOR Character::HitNormal(std::string _normal)
{
	
	//�w�肵�����O�̓S����WireArray���猩��������Ή������@����Ԃ�
	//�Ԃ�����������ɐڐG�����ۂ̃m�b�N�o�b�N�����ƂȂ�
	//������Ȃ��ꍇ��0��Ԃ�
	for (int i = 0; i < FenceHitParam_.WireArray_.size(); i++)
	{
		if (_normal == FenceHitParam_.WireArray_[i])
		{
			return FenceHitParam_.NormalArray_[i];
		}
	}

	return { 0,0,0 };
}

void Character::FenceReflect(XMVECTOR _normal)
{
	//�ڐG�G�t�F�N�g���w��
	SetFenceHitEffect();

	//���߂Ă��鑬�x�����Z�b�g
	ChargeReset();

	//�_�b�V�����̑��x���Z�b�g
	AccelerationStop();

	//�O�̂��ߐ��K������
	//���˕�����0�Ȃ珈�����Ȃ�(0�x�N�g���𐳋K���͂ł��Ȃ�)
	if (XMVector3Equal(_normal, XMVectorZero()))
	{
		return;
	}

	//�P�ʃx�N�g���ɕϊ�
	_normal = XMVector3Normalize(_normal);

	//XMFLOAT3�^�ɕϊ�
	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, _normal);

	//�󂯎�����@�����m�b�N�o�b�N�����ɑ��
	HitParam_.KnockBack_Direction_ = { tmp };

	//�m�b�N�o�b�N�ʂ𑬓x�ɑ��(���l)
	HitParam_.KnockBack_Velocity_.x = FenceHitParam_.KnockBackPower_;
	HitParam_.KnockBack_Velocity_.z = FenceHitParam_.KnockBackPower_;

	//���G��Ԃ�ݒ�
	FenceHitParam_.IsInvincibility_ = true;

	//�ڐG�ʒm
	NotifyFenceHit();

	//�m�b�N�o�b�N����Y����]�p�̌Œ�
	KnockBackAngleY(HitParam_.KnockBack_Direction_);
}

bool Character::IsKnockBackEnd()
{
	//�m�b�N�o�b�N���x���I���l�ɓ��B���������肵�A�^�U��Ԃ�

	if (HitParam_.KnockBack_Velocity_.x <= HitParam_.KnockBackEnd_ ||
		HitParam_.KnockBack_Velocity_.z <= HitParam_.KnockBackEnd_)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Character::InvincibilityTimeCalculation()
{
	//���G���Ԃ̌v�Z

	//���G���Ԃ��ǂ�������(���̊֐��͖��t���[���Ă΂�邽��)
	if (FenceHitParam_.IsInvincibility_)
	{
		//�^�C�}�[���I���l�𒴂����疳�G���Ԃ��I���
		if (++FenceHitParam_.InvincibilityTime_ > FenceHitParam_.InvincibilityValue_)
		{
			FenceHitParam_.InvincibilityTime_ = 0;
			FenceHitParam_.IsInvincibility_ = false;
		}
	}
}

void Character::NotifyFenceHit()
{
	//�ʒm���󂯎�鑤��override���Ă��Ȃ������ꍇ�͉����N����Ȃ�
	//BattleScene�ł̂ݗL��(�Ď��Ώۂ��Ȃ�=AddObserver���Ă΂Ȃ��ꍇ�A
	//InitParam_.observers���̂���Ȃ̂�for�����X���[�����)
	for (IGameObserver* observer : InitParam_.observers) 
	{

		//�Ď��҂֍�Ƀq�b�g�������Ɓi��������Character��ID�j��ʒm
		observer->OnCharacterFenceHit(this->InitParam_.CharacterID);
	}
}

void Character::Charging()
{
	//�`���[�W����SE�Đ�
	Audio::Play(hSoundcharge_);

	//�`���[�W��,���̒l�Ɉ��̉����ʂ����Z��������
	//�`���[�W������Ɏ��ۂ�Acceleration_�ɑ������
	if (MoveParam_.TmpAccele_ < MoveParam_.FullAccelerate_)
	{
		MoveParam_.TmpAccele_ += MoveParam_.AcceleValue_;
	}
	else
	{
		SetFullChargeEffect();
		MoveParam_.TmpAccele_ = MoveParam_.FullAccelerate_;
	}
}

void Character::ChargeRelease()
{
	//���ۂ̉����x�ɗ��߂����̒l����
	MoveParam_.Acceleration_ = MoveParam_.TmpAccele_;

	//���߂Ă��鑬�x�����Z�b�g
	ChargeReset();
}

void Character::ChargeReset()
{
	MoveParam_.TmpAccele_ = 0.0f;
}

void Character::SetArrow()
{
	//���ʃx�N�g���ɑO�����̒����l����Z���A���̐��ʈʒu���v�Z
	XMVECTOR FrontArrow = XMVectorScale(this->MoveParam_.ForwardVector_, this->MoveParam_.AddArrowDepth_);
	
	//���݈ʒu���擾
	XMVECTOR PosVec = XMLoadFloat3(&this->transform_.position_);

	//���̐��ʈʒu�ƌ��݈ʒu�����Z
	XMVECTOR arrowPosVec = XMVectorAdd(PosVec, FrontArrow);

	//���̃g�����X�t�H�[���ɑ��
	XMStoreFloat3(&this->MoveParam_.ArrowTransform_.position_, arrowPosVec);
}

void Character::DrawArrow()
{
	//��󃂃f����`��
	Model::SetAndDraw(MoveParam_.hMoveArrow_, this->MoveParam_.ArrowTransform_);
}

XMVECTOR Character::RotateVecFront(float _rotY, XMVECTOR _front)
{
	//��]���������x�N�g���i�����j����
	//��{�̓��[�J�����ʃx�N�g��
	XMVECTOR v = _front;

	//Y��]�����W�A���ɂ��A��]�s����쐬
	XMMATRIX m = XMMatrixRotationY(XMConvertToRadians(_rotY));

	//�����x�N�g������]�s��ŕϊ����A���[���h���W�ł̌����Ă���������o��
	v = XMVector3TransformCoord(v, m);

	return v;
}

void Character::FrontVectorConfirm()
{
	//���[�J�����ʃx�N�g�������݂�y����]�ʂŕό`����ƁA���ʂ���ǂꂾ����]���������v�Z�����
	//���̒l�����[���h���ʃx�N�g���ƂȂ�
	MoveParam_.ForwardVector_ = RotateVecFront(this->transform_.rotate_.y, InitParam_.FrontDirection_);
}

void Character::InitCSVEffect()
{
	//csv�t�@�C����ǂݍ���
	CsvReader csv;
	csv.Load("CSVData\\EffectData\\VFXData.csv");

	//csv�t�@�C���̊e0��ڂ̕�����̔z����擾
	std::string Effects[] = { "Charge","FullCharge" ,"Locus" , "Hit" , "FenceHit" };
  
	//ChargeParam_����n�܂�VFX�̃p�����[�^(vector<float>�^�̔z��)�̎Q�Ƃ�
	//�|�C���^�z��Ɋi�[
	std::vector<float>* Param[] = { &ChargeParam_,&FullChargeParam,
		&AttackLocusParam_, &HitEffectParam_, &FenceHitEffectParam_ };  


    for (int i = 0; i < sizeof(Effects) / sizeof(Effects[0]); i++)  
    {  
		//0��ڂ̕������n���A���̍s�̃p�����[�^���擾
		std::vector<float> v = GetCSVReadData(csv, Effects[i]);

		//���̎��_�ł͑���̂ݍs����
		// SetEmitter�Ŏ��ۂ�VFX�̃p�����[�^�ɃZ�b�g����� 
		*Param[i] = v;  
            
    }
}

void Character::SetChargingEffect(std::string _path)
{
	//csv����ǂݍ���,�`���[�W���G�t�F�N�g�̃p�����[�^�����ۂɃZ�b�g����
	EmitterData charge;
	VFX::SetEmitter(charge, ChargeParam_);

	//�g�p����摜�̃p�X���Z�b�g����
	charge.textureFileName = _path;

	//���ˈʒu���Z�b�g
	charge.position = this->transform_.position_;

	//�G�t�F�N�g���J�n
	VFX::Start(charge);
}

void Character::SetFullChargeEffect()
{
	//csv����ǂݍ���,�ő�`���[�W�G�t�F�N�g�̃p�����[�^�����ۂɃZ�b�g����
	EmitterData fullcharge;
	VFX::SetEmitter(fullcharge, FullChargeParam);

	//�g�p����摜�̃p�X���Z�b�g����
	fullcharge.textureFileName = "ParticleAssets\\circle_W.png";

	//���ˈʒu���Z�b�g
	fullcharge.position = this->transform_.position_;

	//�G�t�F�N�g���J�n
	VFX::Start(fullcharge);
}

void Character::SetAttackLocusEffect()
{
	//csv����ǂݍ���,�U�����̋O�ՃG�t�F�N�g�̃p�����[�^�����ۂɃZ�b�g����
	EmitterData locus;
	VFX::SetEmitter(locus, AttackLocusParam_);

	//�g�p����摜�̃p�X���Z�b�g����
	locus.textureFileName = "ParticleAssets\\flashB_Y.png";

	//���ˈʒu���Z�b�g
	locus.position = this->transform_.position_;

	//�G�t�F�N�g���J�n
	VFX::Start(locus);
}

void Character::SetHitEffect()
{
	//csv����ǂݍ���,��e�G�t�F�N�g�̃p�����[�^�����ۂɃZ�b�g����
	EmitterData hit;
	VFX::SetEmitter(hit, HitEffectParam_);

	//�g�p����摜�̃p�X���Z�b�g����
	hit.textureFileName = "ParticleAssets\\flashB_W.png";

	//���ˈʒu���Z�b�g
	hit.position = this->transform_.position_;

	//�G�t�F�N�g���J�n
	VFX::Start(hit);
}

void Character::SetFenceHitEffect()
{
	//csv����ǂݍ���,��ɐڐG���G�t�F�N�g�̃p�����[�^�����ۂɃZ�b�g����
	EmitterData  fencehit;
	VFX::SetEmitter(fencehit, FenceHitEffectParam_);

	//�g�p����摜�̃p�X���Z�b�g����
	fencehit.textureFileName = "ParticleAssets\\flashB_W.png";

	//���ˈʒu���Z�b�g
	fencehit.position = this->transform_.position_;

	//�G�t�F�N�g���J�n
	VFX::Start(fencehit);
}
