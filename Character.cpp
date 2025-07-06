#include "Character.h"
#include"Engine/VFX.h"
#include"Engine/Model.h"
#include"Engine/Audio.h"
#include"Engine/Global.h"

namespace {
	
	//�L�����N�^�[�N���X�̋��ʕϐ�������������C���f�b�N�X
	//�������֌W
	enum InitializeIndex
	{
		i_posX = 0,
		i_posY,
		i_posZ,
		i_rotateX,
		i_rotateY,
		i_rotateZ,
		i_scaleX,
		i_scaleY,
		i_scaleZ,
		i_frontX,
		i_frontY,
		i_frontZ,
	};

	//�ړ��֌W
	enum moveIndex
	{
		i_vel = 0,
		i_accele_value,
		i_accele_max,
		i_friction,
		i_arrowrotateX,
		i_arrowrotateY,
		i_arrowrotateZ,
		i_arrowscaleX,
		i_arrowscaleY,
		i_arrowscaleZ,
		i_addarrowdepth
	};

	//��]�֌W
	enum RotateIndex
	{
		i_moverot = 0,
		i_fastrot ,
	};

	//�󒆊֌W
	enum JumpIndex
	{
		i_gravity = 0,
		i_upperlimit,
		i_lowerlimit,
	};

	//��e�֌W
	enum HitIndex
	{
		i_collider = 0,
		i_originalrangemin,
		i_originalrangemax,
		i_convertedrangemin,
		i_convertedrangemax,

		i_deceleration,
		i_knockbackend,
	};

	//��ɐڐG�֌W
	enum WallHitIndex
	{
		i_knockbackpower = 0,
		i_invincibilityvalue,
		i_blinkvalue,

	};

	//�e�t���֌W
	enum ShadowIndex
	{
		i_shadowcorrection = 0,
	};


	//�G�t�F�N�g���������̃C���f�b�N�X
	enum EffectParamIndex
	{
		i_Charge = 0,
		i_Locus,
		i_Hit,
		i_WallHit,
	};

	//�T�E���h���������̃C���f�b�N�X
	enum SoundParamIndex
	{
		i_chargecount = 0,
		i_attackcount,
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
	InitCSVSound();

	//�e�摜�E�T�E���h�̓ǂݍ���
	hSoundcharge_ = Audio::Load("Sound\\SE\\charge.wav",false, ChargeSoundCount_);
	assert(hSoundcharge_ >= 0);
	hSoundattack_ = Audio::Load("Sound\\SE\\attack.wav", false, AttackSoundCount_);
	assert(hSoundattack_ >= 0);
	hSoundCollision_ = Audio::Load("Sound\\SE\\collision.wav",false);
	assert(hSoundCollision_ >= 0);

	//���ꂼ��̍�̖@�����擾
	GetWireNormal();

	//�e���f����������
	InitShadow();
}

Character::~Character()
{
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

void Character::SetcsvStatus(std::string _path)
{
	//csv�t�@�C����ǂݍ���
	CsvReader csv;
	csv.Load(_path);

	//--------------------�������֌W�̃p�����[�^(���g��transform_)--------------------

	//csv�t�@�C����0��ڂ̕�������擾
	std::string p_init = "InitializeParam";

	//�w�肵�������񂪂����ꂩ��0��ڂɑ��݂�����
	if (csv.IsGetParamName(p_init))
	{
		//���̍s��z��Ƃ��đS�擾
		std::vector<float> v = csv.GetParam(p_init);

		//���g�̃g�����X�t�H�[����������
		SetTransformPRS(this->transform_,v);
		InitParam_.FrontDirection_ = {v[i_frontX],v[i_frontY],v[i_frontZ]};

		//�����ʒu��ۊǂ���
		InitParam_.StartPosition_ = this->transform_.position_;
	}

	//--------------------�ړ��֌W�̃p�����[�^--------------------

	//csv�t�@�C����0��ڂ̕�������擾
	std::string p_move = "MoveParam";

	//�w�肵�������񂪂����ꂩ��0��ڂɑ��݂�����
	if (csv.IsGetParamName(p_move))
	{
		//���̍s��z��Ƃ��đS�擾
		std::vector<float> v = csv.GetParam(p_move);

		//�������̏��Ԃ�csv�̊e�s�̏��Ԃɍ��킹��
		//v�̓Y������namespace�Ő錾�����񋓌^���g�p
		MoveParam_.Velocity_ = v[i_vel];
		MoveParam_.AcceleValue_ = v[i_accele_value];
		MoveParam_.FullAccelerate_ = v[i_accele_max];
		MoveParam_.Friction_ = v[i_friction];
		MoveParam_.ArrowRotate_ = { v[i_arrowrotateX],v[i_arrowrotateY],v[i_arrowrotateZ] };
		MoveParam_.ArrowScale_ = { v[i_arrowscaleX],v[i_arrowscaleY],v[i_arrowscaleZ] };
		MoveParam_.AddArrowDepth_ = v[i_addarrowdepth];
	}

	//--------------------��]�֌W�̃p�����[�^--------------------

	//csv�t�@�C����0��ڂ̕�������擾
	std::string p_rotate = "RotateParam";

	//�w�肵�������񂪂����ꂩ��0��ڂɑ��݂�����
	if (csv.IsGetParamName(p_rotate))
	{
		//���̍s��z��Ƃ��đS�擾
		std::vector<float> v = csv.GetParam(p_rotate);

		//�������̏��Ԃ�csv�̊e�s�̏��Ԃɍ��킹��
		//v�̓Y������namespace�Ő錾�����񋓌^���g�p
		RotateParam_.MoveRotateX = v[i_moverot];
		RotateParam_.FastRotateX = v[i_fastrot];

	}

	//--------------------�󒆊֌W�̃p�����[�^--------------------

	//csv�t�@�C����0��ڂ̕�������擾	
	std::string p_jump = "JumpParam";

	//�w�肵�������񂪂����ꂩ��0��ڂɑ��݂�����
	if (csv.IsGetParamName(p_jump))
	{
		//���̍s��z��Ƃ��đS�擾
		std::vector<float> v = csv.GetParam(p_jump);

		//�������̏��Ԃ�csv�̊e�s�̏��Ԃɍ��킹��
		//v�̓Y������namespace�Ő錾�����񋓌^���g�p
		JumpParam_.Gravity_ = v[i_gravity];
		JumpParam_.HeightLowerLimit_ = v[i_upperlimit];
		JumpParam_.HeightUpperLimit_ = v[i_lowerlimit];
	}

	//--------------------��e�֌W�̃p�����[�^--------------------

	//csv�t�@�C����0��ڂ̕�������擾	
	std::string p_hit = "HitParam";

	//�w�肵�������񂪂����ꂩ��0��ڂɑ��݂�����
	if (csv.IsGetParamName(p_hit))
	{
		//���̍s��z��Ƃ��đS�擾
		std::vector<float> v = csv.GetParam(p_hit);

		//�������̏��Ԃ�csv�̊e�s�̏��Ԃɍ��킹��
		//v�̓Y������namespace�Ő錾�����񋓌^���g�p
		HitParam_.ColliderSize_ =v[i_collider];
		HitParam_.OriginaRangeMin_ = v[i_originalrangemin];
		HitParam_.OriginaRangeMax_ = v[i_originalrangemax];
		HitParam_.ConvertedRangeMin_ = v[i_convertedrangemin];
		HitParam_.ConvertedRangeMax_ = v[i_convertedrangemax];
		HitParam_.DecelerationRate_ = v[i_deceleration];
		HitParam_.KnockBackEnd_ = v[i_knockbackend];
	}

	//--------------------��ɐڐG�֌W�̃p�����[�^--------------------

	//csv�t�@�C����0��ڂ̕�������擾	
	std::string p_wallhit = "WallHitParam";

	//�w�肵�������񂪂����ꂩ��0��ڂɑ��݂�����
	if (csv.IsGetParamName(p_wallhit))
	{
		//���̍s��z��Ƃ��đS�擾
		std::vector<float> v = csv.GetParam(p_wallhit);

		//�������̏��Ԃ�csv�̊e�s�̏��Ԃɍ��킹��
		//v�̓Y������namespace�Ő錾�����񋓌^���g�p
		WallHitParam_.KnockBackPower_ = v[i_knockbackpower];
		WallHitParam_.InvincibilityValue_ = static_cast<int>(v[i_invincibilityvalue]);
		WallHitParam_.blinkValue_ = static_cast<int>(v[i_blinkvalue]);
	}


	//--------------------�e�֌W�̃p�����[�^--------------------

	//csv�t�@�C����0��ڂ̕�������擾	
	std::string p_shadow = "ShadowParam";

	//�w�肵�������񂪂����ꂩ��0��ڂɑ��݂�����
	if(csv.IsGetParamName(p_shadow))
	{
		//���̍s��z��Ƃ��đS�擾
		std::vector<float> v = csv.GetParam(p_shadow);

		//�������̏��Ԃ�csv�̊e�s�̏��Ԃɍ��킹��
		//v�̓Y������namespace�Ő錾�����񋓌^���g�p
		ShadowParam_.ShadowCorrection_ = v[i_shadowcorrection];
	}
}

void Character::GetWireNormal()
{
	//�e�C���X�^���X�����̖@�����擾
	UpperWire* pUpperWire = (UpperWire*)FindObject("UpperWire");
	WallHitParam_.UpperNormal_ = pUpperWire->GetNormal();

	LowerWire* pLowerWire = (LowerWire*)FindObject("LowerWire");
	WallHitParam_.LowerNormal_ = pLowerWire->GetNormal();

	RightWire* pRightWire = (RightWire*)FindObject("RightWire");
	WallHitParam_.RightNormal_ = pRightWire->GetNormal();

	LeftWire* pLeftWire = (LeftWire*)FindObject("LeftWire");
	WallHitParam_.LeftNormal_ = pLeftWire->GetNormal();

	//�擾�����@����z��Ɋi�[
	//����炪��ɐڐG�����ۂ̒��˕Ԃ�����ɂȂ�
	WallHitParam_.NormalArray_ = { WallHitParam_.UpperNormal_,  WallHitParam_.LowerNormal_,
		 WallHitParam_.RightNormal_, WallHitParam_.LeftNormal_, };
}

void Character::InitArrow()
{
	MoveParam_.ArrowTransform_.position_ = { 0.0f,0.0f, 0.0f };
	MoveParam_.ArrowTransform_.rotate_ = MoveParam_.ArrowRotate_;
	MoveParam_.ArrowTransform_.scale_ = MoveParam_.ArrowScale_;
}

void Character::DrawCharacterModel(int _handle, Transform _transform)
{
	//���G���Ԓ����ǂ����Ń��f���̓_�ŕ\�����s��

	if (WallHitParam_.IsInvincibility_)
	{
		//���G���Ԓ��Ȃ�^�C�}�[���g���A���t���[�������Ƀ��f����`��
		if (++WallHitParam_.blinkTimer_ > WallHitParam_.blinkValue_) {

			WallHitParam_.blinkTimer_ = 0;
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
	//Input:�l�𒼐ړ���
	//Text:�L�q�̂�

	//�L�����N�^�[�̈ʒu(position_.x,y,z)
	if (ImGui::TreeNode("Transform.Position"))
	{
		ImGui::SliderFloat("PositionX:%.3f", &this->transform_.position_.x, WestEnd_, EastEnd_);
		ImGui::SliderFloat("PositionZ:%.3f", &this->transform_.position_.z, SouthEnd_, NorthEnd_);
		ImGui::TreePop();
	}

	//�L�����N�^�[�̉�]��(rotate_.x,y,z)
	if (ImGui::TreeNode("Transform.Rotate"))
	{
		ImGui::Text("RotateX:%.3f", &this->transform_.rotate_.x);
		ImGui::Text("RotateY:%.3f", &this->transform_.rotate_.y);
		ImGui::Text("RotateZ:%.3f", &this->transform_.rotate_.z);
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

	//�����x,������,�ő����,�`���[�W�̍ۂɎg�����̉����x
	if (ImGui::TreeNode("Move"))
	{
		ImGui::InputFloat("velocity", &this->MoveParam_.Velocity_);
		ImGui::InputFloat("AcceleValue", &this->MoveParam_.AcceleValue_);
		ImGui::InputFloat("FullAccelerate", &this->MoveParam_.FullAccelerate_);
		ImGui::InputFloat("TmpAccele", &this->MoveParam_.TmpAccele_);
		ImGui::TreePop();
	}

	//�L�����N�^�[�ړ����̒ʏ�̉�]�ʁA�`���[�W���̉�]��
	if (ImGui::TreeNode("Rotate"))
	{
		ImGui::InputFloat("normalRotate", &this->RotateParam_.MoveRotateX);
		ImGui::InputFloat("fastRotate", &this->RotateParam_.FastRotateX);
		ImGui::TreePop();
	}

	//�L�����N�^�[�ɂ�����d��,�����̏��,����
	if (ImGui::TreeNode("Jump"))
	{
		ImGui::InputFloat("Gravity", &this->JumpParam_.Gravity_);
		ImGui::InputFloat("HeightLowerLimit", &this->JumpParam_.HeightLowerLimit_);
		ImGui::InputFloat("HeightUpperLimit", &this->JumpParam_.HeightUpperLimit_);
		ImGui::TreePop();
	}

	//�m�b�N�o�b�N�ʌv�Z���̕ϊ��O�͈̔�,�ϊ���͈̔�,������,�m�b�N�o�b�N���I������l
	if (ImGui::TreeNode("Hit"))
	{
		ImGui::InputFloat("OriginaRangeMin", &this->HitParam_.OriginaRangeMin_);
		ImGui::InputFloat("OriginaRangeMax", &this->HitParam_.OriginaRangeMax_);
		ImGui::InputFloat("ConvertedRangeMin", &this->HitParam_.ConvertedRangeMin_);
		ImGui::InputFloat("ConvertedRangeMax", &this->HitParam_.ConvertedRangeMax_);
		ImGui::InputFloat("DecelerationRate", &this->HitParam_.DecelerationRate_);
		ImGui::InputFloat("KnockBackEnd", &this->HitParam_.KnockBackEnd_);
		ImGui::TreePop();
	}

	//��q�b�g���̃m�b�N�o�b�N��,���G����,���G���Ԓ��̕`��Ԋu
	if (ImGui::TreeNode("WallHit"))
	{
		ImGui::InputFloat("Collider", &this->WallHitParam_.KnockBackPower_);
		ImGui::InputInt("InvincibilityTime", &this->WallHitParam_.InvincibilityValue_);
		ImGui::InputInt("blinkValue", &this->WallHitParam_.blinkValue_);
		ImGui::TreePop();
	}

	//�e�̈ʒu�̕␳
	if (ImGui::TreeNode("Shadow"))
	{
		ImGui::InputFloat("Collider", &this->ShadowParam_.ShadowCorrection_);
		ImGui::TreePop();
	}

	ImGui::TreePop();
}

void Character::CharacterGravity()
{
	//�d�͕��̒l�������A�v���C���[�͏�ɉ������ɗ͂��������Ă���
	JumpParam_. JumpSpeed_ -= JumpParam_.Gravity_;

	//�t�B�[���h�ɏ���Ă��邩�͊֌W�Ȃ��d�͂͂����葱����
	this->transform_.position_.y += JumpParam_.JumpSpeed_;

	//�v���C���[�߂肱�ݖh�~�Ɉ��ȉ���y���W�ɂȂ�����l���Œ肷��
	if (this->transform_.position_.y <= JumpParam_.HeightLowerLimit_)
	{
		this->transform_.position_.y = JumpParam_.HeightLowerLimit_;

		//���ȉ���y���W�ɂȂ����ꍇ,���n���Ă��锻��ɂ���
		JumpParam_.IsOnGround_ = true;
	}
}

void Character::InitShadow()
{
	//�������̎��_�ŃX�e�[�W�N���X�̃C���X�^���X���擾
	ShadowParam_.pGround_ = (Ground*)FindObject("Ground");

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

	//���C�̕�����ݒ�
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
	//�P�ʃx�N�g�������A�ړ��������m��
	MoveParam_.MoveDirection_ = XMVector3Normalize(_direction);

	//�ړ��x�N�g�����쐬
	CreateMoveVector();

	//��O�łȂ���Έʒu�X�V 
	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, MoveParam_.NewPositon_);
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
	MoveParam_.NewPositon_ = PrevPos + MoveVector;
}

bool Character::IsOutsideStage(XMFLOAT3 _position)
{
	//�w��ʒu����ł��X�e�[�W�[�𒴂��邩�ǂ������肵�A�^�U��Ԃ�
	//�X�e�[�W�O���������ۂɎg�p

	if (_position.x > EastEnd_ || _position.x < WestEnd_ || _position.z > NorthEnd_ || _position.z < SouthEnd_){
			return true;
	}
	else {
		return false;
	}
}

void Character::MoveConfirm()
{
	//�ړ���̃x�N�g����transform_.position�ɑ�����A�ړ����m�肷��
	XMStoreFloat3(&this->transform_.position_, MoveParam_.NewPositon_);
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

void Character::Reflect(XMVECTOR myVector, XMVECTOR eVector, float myVelocity, float eVelocity)
{
	//���G��ԂȂ珈�����Ȃ�
	if (this->WallHitParam_.IsInvincibility_)
	{
		return;
	}

	//�ڐG����̃x�N�g�����玩�g�̃x�N�g���������A���K��
	XMVECTOR subVector = XMVector3Normalize(XMVectorSubtract(eVector ,myVector));
	
	//�t�x�N�g���ɂ��Ĕ��˕���������
	subVector = XMVectorNegate(subVector);
	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp,subVector);

	//���˕�����ݒ�
	 HitParam_.KnockBack_Direction_ = tmp;

	//���g�̑��x�Ƒ���̑��x�̍������Ƃ�
	float subVelocity = myVelocity - eVelocity;

	//�m�b�N�o�b�N�ʂ̏�����
	//�l���ω�����̂Ń��[�J���ϐ�
	float KnockBackValue = 0.0f;

	//���x���̔���͐��`�⊮���̍ő�l��K�p
	//���x�������g�̕������ȏ�Ȃ�A���g�̃m�b�N�o�b�N�ʂ�0
	if (subVelocity >= HitParam_.OriginaRangeMax_)
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
		KnockBackValue = LinearCompletion(subVelocity,
			HitParam_.OriginaRangeMin_, HitParam_.OriginaRangeMax_,
			HitParam_.ConvertedRangeMin_, HitParam_.ConvertedRangeMax_);
	}

	//�ϊ������m�b�N�o�b�N�ʂ��m�b�N�o�b�N���̑��xx,z�ɑ��
	HitParam_.KnockBack_Velocity_.x = KnockBackValue;
	HitParam_.KnockBack_Velocity_.z = KnockBackValue;

	//���߂Ă��鑬�x�����Z�b�g
	ChargeReset();

	//�_�b�V�����̑��x���Z�b�g
	AccelerationStop();
}

void Character::KnockBack()
{
	MoveRotateReverse();

	//���t���[���m�b�N�o�b�N���x������
	HitParam_.KnockBack_Velocity_.x *= HitParam_.DecelerationRate_;
	HitParam_.KnockBack_Velocity_.z *= HitParam_.DecelerationRate_;

	//�m�b�N�o�b�N��̈ʒu���v�Z
	//�ʒu = �ʒu + ���� * ���x
	XMFLOAT3 TmpPos = this->transform_.position_;
	TmpPos.x += HitParam_.KnockBack_Direction_.x * HitParam_.KnockBack_Velocity_.x;
	TmpPos.z += HitParam_.KnockBack_Direction_.z * HitParam_.KnockBack_Velocity_.z;

	//���̎��_�ł͕ύX�����A�ړ���̉��̈ʒu�ɕۊ�
	MoveParam_.NewPositon_ = XMLoadFloat3(&TmpPos);

	//��O�łȂ���Έʒu�X�V 
	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, MoveParam_. NewPositon_);
	if (!IsOutsideStage(tmp))
	{
		MoveConfirm();
	}
}

XMVECTOR Character::HitNormal(std::string _normal)
{
	//wallArray[] = { "UpperWire", "LowerWire", "RightWire" ,"LeftWire" };
	
	//�w�肵�����O�̓S����WallArray���猩��������Ή������@����Ԃ�
	//�Ԃ�����������ɐڐG�����ۂ̃m�b�N�o�b�N�����ƂȂ�
	//������Ȃ��ꍇ��0��Ԃ�
	for (int i = 0; i < WallHitParam_.WireArray_.size(); i++)
	{
		if (_normal == WallHitParam_.WireArray_[i])
		{
			return WallHitParam_.NormalArray_[i];
		}
	}

	return { 0,0,0 };
}

void Character::WallReflect(XMVECTOR normal)
{
	//�ڐG�G�t�F�N�g���w��
	SetWallHitEffect();

	//���߂Ă��鑬�x�����Z�b�g
	ChargeReset();

	//�_�b�V�����̑��x���Z�b�g
	AccelerationStop();

	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, normal);

	//�󂯎�����@�����m�b�N�o�b�N�����ɑ��
	HitParam_.KnockBack_Direction_ = { tmp };

	//�m�b�N�o�b�N�ʂ𑬓x�ɑ��(���l)
	HitParam_.KnockBack_Velocity_.x = WallHitParam_.KnockBackPower_;
	HitParam_.KnockBack_Velocity_.z = WallHitParam_.KnockBackPower_;

	//���G��Ԃ�ݒ�
	WallHitParam_.IsInvincibility_ = true;

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

void Character::InvincibilityTimeCalclation()
{
	//���G���Ԃ̌v�Z

	//���G���Ԃ��ǂ�������(���̊֐��͖��t���[���Ă΂�邽��)
	if (WallHitParam_.IsInvincibility_)
	{
		//�^�C�}�[���I���l�𒴂����疳�G���Ԃ��I���
		if (++WallHitParam_.InvincibilityTime_ > WallHitParam_.InvincibilityValue_)
		{
			WallHitParam_.InvincibilityTime_ = 0;
			WallHitParam_.IsInvincibility_ = false;
		}
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
		//SetFullChargeEffect();
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
	XMVECTOR frontArrow = XMVectorScale(this->MoveParam_.ForwardVector_, this->MoveParam_.AddArrowDepth_);
	
	//���݈ʒu���擾
	XMVECTOR PosVec = XMLoadFloat3(&this->transform_.position_);

	//���̐��ʈʒu�ƌ��݈ʒu�����Z
	XMVECTOR arrowPosVec = XMVectorAdd(PosVec, frontArrow);

	//���̃g�����X�t�H�[���ɑ��
	XMStoreFloat3(&this->MoveParam_.ArrowTransform_.position_, arrowPosVec);
}

XMVECTOR Character::RotateVecFront(float rotY, XMVECTOR front)
{
	//��]���������x�N�g���i�����j����
	//��{�̓��[�J�����ʃx�N�g��
	XMVECTOR v = front;

	//Y��]�����W�A���ɂ��A��]�s����쐬
	XMMATRIX m = XMMatrixRotationY(XMConvertToRadians(rotY));

	//�����x�N�g������]�s��ŕϊ����A���[���h���W�ł̌����Ă���������o��
	v = XMVector3TransformCoord(v, m);

	return v;
}

void Character::InitCSVEffect()
{
	//csv�t�@�C����ǂݍ���
	CsvReader csv;
	csv.Load("CSVData\\EffectData\\VFXData.csv");

	//csv�t�@�C���̊e0��ڂ̕�����̔z����擾

	std::string effects[] = { "Charge","FullCharge" ,"Locus" , "Hit" , "WallHit" };
  
	//ChargeParam_����n�܂�VFX�̃p�����[�^(vector<float>�^�̔z��)�̎Q�Ƃ�
	//�|�C���^�z��Ɋi�[
	std::vector<float>* param[] = { &ChargeParam_,&FullChargeParam,
		&AttackLocusParam_, &HitEffectParam_, &WallHitEffectParam_ };  


    for (int i = 0; i < sizeof(effects) / sizeof(effects[0]); i++)  
    {  
		//�w�肵�������񂪂����ꂩ��0��ڂɑ��݂�����
		if (csv.IsGetParamName(effects[i]))
        {  
			//���̍s��z��Ƃ��đS�擾
			std::vector<float> v = csv.GetParam(effects[i]);  

			//���̎��_�ł͑���̂ݍs����
			// SetEmitter�Ŏ��ۂ�VFX�̃p�����[�^�ɃZ�b�g����� 
			*param[i] = v;  
            
        }  
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
	fullcharge.textureFileName = "PaticleAssets\\flashA_W.png";

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
	locus.textureFileName = "PaticleAssets\\flashB_Y.png";

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
	hit.textureFileName = "PaticleAssets\\flashB_W.png";

	//���ˈʒu���Z�b�g
	hit.position = this->transform_.position_;

	//�G�t�F�N�g���J�n
	VFX::Start(hit);
}

void Character::SetWallHitEffect()
{
	//csv����ǂݍ���,��ɐڐG���G�t�F�N�g�̃p�����[�^�����ۂɃZ�b�g����
	EmitterData  wallhit;
	VFX::SetEmitter(wallhit, WallHitEffectParam_);

	//�g�p����摜�̃p�X���Z�b�g����
	wallhit.textureFileName = "PaticleAssets\\flashB_W.png";

	//���ˈʒu���Z�b�g
	wallhit.position = this->transform_.position_;

	//�G�t�F�N�g���J�n
	VFX::Start(wallhit);
}

void Character::InitCSVSound()
{
	//csv�t�@�C����ǂݍ���
	CsvReader csv;
	csv.Load("CSVData\\EngineData\\SoundData.csv");

	//csv�t�@�C����0��ڂ̕�������擾
	std::string p_sound = "SoundParam";
	
	//�w�肵�������񂪂����ꂩ��0��ڂɑ��݂�����
	if (csv.IsGetParamName(p_sound))
	{
		//���̍s��z��Ƃ��đS�擾
		std::vector<float> v = csv.GetParam(p_sound);
		
		//�������̏��Ԃ�csv�̊e�s�̏��Ԃɍ��킹��
		//v�̓Y������namespace�Ő錾�����񋓌^���g�p
		ChargeSoundCount_ = static_cast<int>(v[i_chargecount]);
		AttackSoundCount_ = static_cast<int>(v[i_attackcount]);
	}
}
