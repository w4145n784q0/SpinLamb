#include "Character.h"
#include"Engine/VFX.h"
#include"Engine/Model.h"
#include"Engine/Audio.h"
#include"Engine/Global.h"

namespace {
	
	//�L�����N�^�[�N���X�̋��ʕϐ�������������C���f�b�N�X

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

	enum RotateIndex
	{
		i_moverot = 0,
		i_fastrot ,
	};

	enum JumpIndex
	{
		i_gravity = 0,
		i_upperlimit,
		i_lowerlimit,
	};

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

	enum WallHitIndex
	{
		i_knockbackpower = 0,
		i_invincibilityvalue,
		i_blinkvalue,

	};

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
	InitCSVEffect();
	InitCSVSound();

	hSoundcharge_ = Audio::Load("Sound\\SE\\charge.wav",false, ChargeSoundCount_);
	assert(hSoundcharge_ >= 0);
	hSoundattack_ = Audio::Load("Sound\\SE\\attack.wav", false, AttackSoundCount_);
	assert(hSoundattack_ >= 0);
	hSoundCollision_ = Audio::Load("Sound\\SE\\collision.wav",false);
	assert(hSoundCollision_ >= 0);

	GetWireNormal();
	InitShadow();
}

Character::~Character()
{
}

void Character::SetcsvStatus(std::string _path)
{
	CsvReader csv;
	csv.Load(_path);

	std::string p_init = "InitializeParam";
	if (csv.IsGetParamName(p_init))
	{
		std::vector<float> v = csv.GetParam(p_init);

		InitParam_.StartPosition_ = { v[pos_x], v[pos_y],v[pos_z] };
		this->transform_.rotate_ = { v[rot_x], v[rot_y],v[rot_z] };
		this->transform_.scale_ = { v[sca_x] , v[sca_y] ,v[sca_z] };
	}

	std::string p_move = "MoveParam";
	if (csv.IsGetParamName(p_move))
	{
		std::vector<float> v = csv.GetParam(p_move);
		MoveParam_.Velocity_ = v[i_vel];
		MoveParam_.AcceleValue_ = v[i_accele_value];
		MoveParam_.FullAccelerate_ = v[i_accele_max];
		MoveParam_.Friction_ = v[i_friction];
		MoveParam_.ArrowRotate_ = { v[i_arrowrotateX],v[i_arrowrotateY],v[i_arrowrotateZ] };
		MoveParam_.ArrowScale_ = { v[i_arrowscaleX],v[i_arrowscaleY],v[i_arrowscaleZ] };
		MoveParam_.AddArrowDepth_ = v[i_addarrowdepth];
	}

	std::string p_rotate = "RotateParam";
	if (csv.IsGetParamName(p_rotate))
	{
		std::vector<float> v = csv.GetParam(p_rotate);
		RotateParam_.MoveRotateX = v[i_moverot];
		RotateParam_.FastRotateX = v[i_fastrot];

	}

	std::string p_jump = "JumpParam";
	if (csv.IsGetParamName(p_jump))
	{
		std::vector<float> v = csv.GetParam(p_jump);
		JumpParam_.Gravity_ = v[i_gravity];
		JumpParam_.HeightLowerLimit_ = v[i_upperlimit];
		JumpParam_.HeightUpperLimit_ = v[i_lowerlimit];
	}

	std::string p_hit = "HitParam";
	if (csv.IsGetParamName(p_hit))
	{
		std::vector<float> v = csv.GetParam(p_hit);
		HitParam_.ColliderSize_ =v[i_collider];

		HitParam_.OriginaRangeMin_ = v[i_originalrangemin];
		HitParam_.OriginaRangeMax_ = v[i_originalrangemax];
		HitParam_.ConvertedRangeMin_ = v[i_convertedrangemin];
		HitParam_.ConvertedRangeMax_ = v[i_convertedrangemax];
		HitParam_.DecelerationRate_ = v[i_deceleration];
		HitParam_.KnockBackEnd_ = v[i_knockbackend];
	}

	std::string p_wallhit = "WallHitParam";
	if (csv.IsGetParamName(p_wallhit))
	{
		std::vector<float> v = csv.GetParam(p_wallhit);
		WallHitParam_.KnockBackPower_ = v[i_knockbackpower];
		WallHitParam_.InvincibilityValue_ = static_cast<int>(v[i_invincibilityvalue]);
		WallHitParam_.blinkValue_ = static_cast<int>(v[i_blinkvalue]);
	}

	std::string p_shadow = "ShadowParam";
	if(csv.IsGetParamName(p_shadow))
	{
		std::vector<float> v = csv.GetParam(p_shadow);
		ShadowParam_.ShadowCorrection_ = v[i_shadowcorrection];
	}

}

void Character::DrawCharacterModel(int _handle, Transform _transform)
{
	if (WallHitParam_.IsInvincibility_)
	{
		if (++WallHitParam_.blinkTimer_ > WallHitParam_.blinkValue_) {

			WallHitParam_.blinkTimer_ = 0;
			DrawModel(_handle, _transform);
		}
	}
	else
	{
		DrawModel(_handle, _transform);
	}
}

void Character::DrawModel(int _handle, Transform _transform)
{
	Model::SetTransform(_handle, _transform);
	Model::Draw(_handle);
}

void Character::DrawCharacterImGui()
{
	if (ImGui::TreeNode("Transform.Position"))
	{
		ImGui::SliderFloat("PositionX:%.3f", &this->transform_.position_.x, WestEnd_, EastEnd_);
		ImGui::SliderFloat("PositionZ:%.3f", &this->transform_.position_.z, SouthEnd_, NorthEnd_);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Transform.Rotate"))
	{
		ImGui::Text("RotateX:%.3f", &this->transform_.rotate_.x);
		ImGui::Text("RotateY:%.3f", &this->transform_.rotate_.y);
		ImGui::Text("RotateZ:%.3f", &this->transform_.rotate_.z);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Forward"))
	{
		XMFLOAT3 tmp;
		XMStoreFloat3(&tmp, MoveParam_.ForwardVector_);
		ImGui::Text("ForwardX:%.3f", tmp.x);
		ImGui::Text("ForwardY:%.3f", tmp.y);
		ImGui::Text("ForwardZ:%.3f", tmp.z);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Move"))
	{
		ImGui::InputFloat("velocity", &this->MoveParam_.Velocity_);
		ImGui::InputFloat("AcceleValue", &this->MoveParam_.AcceleValue_);
		ImGui::InputFloat("FullAccelerate", &this->MoveParam_.FullAccelerate_);
		ImGui::InputFloat("TmpAccele", &this->MoveParam_.TmpAccele_);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Rotate"))
	{
		ImGui::InputFloat("normalRotate", &this->RotateParam_.MoveRotateX);
		ImGui::InputFloat("fastRotate", &this->RotateParam_.FastRotateX);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Jump"))
	{
		ImGui::InputFloat("Gravity", &this->JumpParam_.Gravity_);
		ImGui::InputFloat("HeightLowerLimit", &this->JumpParam_.HeightLowerLimit_);
		ImGui::InputFloat("HeightUpperLimit", &this->JumpParam_.HeightUpperLimit_);
		ImGui::TreePop();
	}

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

	if (ImGui::TreeNode("WallHit"))
	{
		ImGui::InputFloat("Collider", &this->WallHitParam_.KnockBackPower_);
		ImGui::InputInt("InvincibilityTime", &this->WallHitParam_.InvincibilityValue_);
		ImGui::InputInt("blinkValue", &this->WallHitParam_.blinkValue_);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Shadow"))
	{
		ImGui::InputFloat("Collider", &this->ShadowParam_.ShadowCorrection_);
		ImGui::TreePop();
	}

	ImGui::TreePop();
}

void Character::CharacterGravity()
{
	JumpParam_. JumpSpeed_ -= JumpParam_.Gravity_;//�d�͕��̒l�������A�v���C���[�͏�ɉ������ɗ͂��������Ă���
	this->transform_.position_.y += JumpParam_.JumpSpeed_;//�t�B�[���h�ɏ���Ă��邩�͊֌W�Ȃ��d�͂͂����葱����

	if (this->transform_.position_.y <= JumpParam_.HeightLowerLimit_)//�v���C���[�߂肱�ݖh�~�Ɉ��ȉ���y���W�Œl���Œ�
	{
		this->transform_.position_.y = JumpParam_.HeightLowerLimit_;
		JumpParam_.IsOnGround_ = true;
	}
}

void Character::InitShadow()
{
	ShadowParam_.pGround_ = (Ground*)FindObject("Ground");
	ShadowParam_.hShadow_ = Model::Load("Model\\ShadowPoint.fbx");
	assert(ShadowParam_.hShadow_ >= 0);
}

void Character::ShadowSet()
{
	int hGroundModel = ShadowParam_.pGround_->GetModelHandle();//�X�e�[�W�̃��f���ԍ����擾
	RayCastData data;
	data.start = this->transform_.position_;//���C�̔��ˈʒu��ݒ�
	data.dir = XMFLOAT3(0, -1, 0);//���C�̕�����ݒ�
	Model::RayCast(hGroundModel, &data);//���C�𔭎�

	//���C������������
	if (data.hit)
	{
		ShadowParam_.ShadowHeight_ = (this->transform_.position_.y - data.dist) + ShadowParam_.ShadowCorrection_;
	}
	this->ShadowParam_.ShadowTrans_.position_.x = this->transform_.position_.x;
	this->ShadowParam_.ShadowTrans_.position_.z = this->transform_.position_.z;
	this->ShadowParam_.ShadowTrans_.position_.y = ShadowParam_.ShadowHeight_;
}

void Character::ShadowDraw()
{
	Model::SetTransform(ShadowParam_.hShadow_, this->ShadowParam_.ShadowTrans_);
	Model::Draw(ShadowParam_.hShadow_);
}

void Character::CharacterMove(XMVECTOR _direction)
{
	//�P�ʃx�N�g�������A�ړ��������m��
	MoveParam_.MoveDirection_ = XMVector3Normalize(_direction);

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

	//���݈ʒu�ƈړ��x�N�g�������Z
	XMVECTOR PrevPos = XMLoadFloat3(&this->transform_.position_);
	MoveParam_.NewPositon_ = PrevPos + MoveVector;
}

bool Character::IsOutsideStage(XMFLOAT3 _position)
{
	if (_position.x > EastEnd_ || _position.x < WestEnd_ || _position.z > NorthEnd_ || _position.z < SouthEnd_){
			return true;
	}
	else {
		return false;
	}
}

void Character::MoveConfirm()
{
	XMStoreFloat3(&this->transform_.position_, MoveParam_.NewPositon_);
}

float Character::RotateDirectionVector(XMVECTOR _MoveVector)
{
	//�R���g���[���[�����ƑO�����x�N�g���̊O�ϋ��߂�
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
		if (signbit(subVelocity)) 
		{
			subVelocity = -subVelocity;
		}

		//�m�b�N�o�b�N�ʂ̐��`�⊮���s��
		KnockBackValue = LinearCompletion(subVelocity,
			HitParam_.OriginaRangeMin_, HitParam_.OriginaRangeMax_,
			HitParam_.ConvertedRangeMin_, HitParam_.ConvertedRangeMax_);
	}

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

	//�ʒu = �ʒu + ���� * ���x
	XMFLOAT3 TmpPos = this->transform_.position_;
	TmpPos.x += HitParam_.KnockBack_Direction_.x * HitParam_.KnockBack_Velocity_.x;
	TmpPos.z += HitParam_.KnockBack_Direction_.z * HitParam_.KnockBack_Velocity_.z;

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
	for (int i = 0; i < WallHitParam_.WireArray_.size(); i++)
	{
		if (_normal == WallHitParam_.WireArray_[i])
		{
			return WallHitParam_.NormalArray_[i];
		}
	}

	return { 0,0,0 };
}

void Character::WallHit()
{
	SetWallHitEffect();
	AccelerationStop();

	//���ʃx�N�g���̋t�x�N�g�����v�Z
	XMVECTOR negate = XMVectorNegate(MoveParam_.ForwardVector_);
	XMFLOAT3 inverse;
	XMStoreFloat3(&inverse, negate);

	//�m�b�N�o�b�N�����ɑ��
	HitParam_.KnockBack_Direction_ = { inverse.x, inverse.y, inverse.z };

	//�m�b�N�o�b�N�ʂ𑬓x�ɑ��(���l)
	HitParam_.KnockBack_Velocity_.x = WallHitParam_.KnockBackPower_;
	HitParam_.KnockBack_Velocity_.z = WallHitParam_.KnockBackPower_;

	WallHitParam_.IsInvincibility_ = true;
}

void Character::WallReflect(XMVECTOR pos)
{
	SetWallHitEffect();
	AccelerationStop();

	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, pos);

	//�󂯎�����@�����m�b�N�o�b�N�����ɑ��
	HitParam_.KnockBack_Direction_ = { tmp };

	//�m�b�N�o�b�N�ʂ𑬓x�ɑ��(���l)
	HitParam_.KnockBack_Velocity_.x = WallHitParam_.KnockBackPower_;
	HitParam_.KnockBack_Velocity_.z = WallHitParam_.KnockBackPower_;

	WallHitParam_.IsInvincibility_ = true;

	ChargeReset();
}

bool Character::IsKnockBackEnd()
{
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
	if (WallHitParam_.IsInvincibility_)
	{
		if (++WallHitParam_.InvincibilityTime_ > WallHitParam_.InvincibilityValue_)
		{
			WallHitParam_.InvincibilityTime_ = 0;
			WallHitParam_.IsInvincibility_ = false;
		}
	}
}

void Character::Charging()
{
	Audio::Play(hSoundcharge_);

	//�`���[�W��,���̒l�Ɉ��̉����ʂ����Z��������
	//�`���[�W������Ɏ��ۂ�Acceleration_�ɑ������
	if (MoveParam_.TmpAccele_ < MoveParam_.FullAccelerate_)
	{
		MoveParam_.TmpAccele_ += MoveParam_.AcceleValue_;
	}
	else
	{
		MoveParam_.TmpAccele_ = MoveParam_.FullAccelerate_;
	}
}

void Character::EmitCharge()
{
	MoveParam_.Acceleration_ = MoveParam_.TmpAccele_;
	ChargeReset();
}

void Character::ChargeReset()
{
	MoveParam_.TmpAccele_ = 0.0f;
}

void Character::SetArrow()
{
	XMVECTOR frontArrow = XMVectorScale(this->MoveParam_.ForwardVector_, this->MoveParam_.AddArrowDepth_);
	XMVECTOR PosVec = XMLoadFloat3(&this->transform_.position_);
	XMVECTOR arrowPosVec = XMVectorAdd(PosVec, frontArrow);
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
	CsvReader csv;
	csv.Load("CSVData\\VFXData.csv");

	//ChargeParam_...��vector<float>�^�̃p�����[�^
	std::string effects[] = { "Charge","Locus" , "Hit" , "WallHit" };
    std::vector<float>* param[] = { &ChargeParam_, &AttackLocusParam_, &HitEffectParam_, &WallHitEffectParam_ };  
    for (int i = 0; i < sizeof(effects) / sizeof(effects[0]); i++)  
    {  
		if (csv.IsGetParamName(effects[i]))  
        {  
			std::vector<float> v = csv.GetParam(effects[i]);  
			*param[i] = v;  
            // SetEmitter�Ŏ��ۂ�VFX�̃p�����[�^�ɃZ�b�g�����  
        }  
    }
}

void Character::SetChargingEffect(std::string _path)
{
	EmitterData charge;
	VFX::SetEmitter(charge, ChargeParam_);
	charge.textureFileName = _path;
	charge.position = this->transform_.position_;
	VFX::Start(charge);
}

void Character::SetAttackLocusEffect()
{
	EmitterData locus;
	VFX::SetEmitter(locus, AttackLocusParam_);
	locus.textureFileName = "PaticleAssets\\flashB_Y.png";
	locus.position = this->transform_.position_;
	VFX::Start(locus);
}

void Character::SetHitEffect()
{
	EmitterData hit;

	VFX::SetEmitter(hit, HitEffectParam_);
	hit.textureFileName = "PaticleAssets\\flashB_W.png";
	hit.position = this->transform_.position_;
	VFX::Start(hit);
}

void Character::SetWallHitEffect()
{
	EmitterData  wallhit;
	VFX::SetEmitter(wallhit, WallHitEffectParam_);
	wallhit.textureFileName = "PaticleAssets\\flashB_W.png";
	wallhit.position = this->transform_.position_;
	VFX::Start(wallhit);
}

void Character::InitCSVSound()
{
	CsvReader csv;
	csv.Load("CSVData\\SoundData.csv");

	std::string p_sound = "SoundParam";
	if (csv.IsGetParamName(p_sound))
	{
		std::vector<float> v = csv.GetParam(p_sound);
		ChargeSoundCount_ = static_cast<int>(v[i_chargecount]);
		AttackSoundCount_ = static_cast<int>(v[i_attackcount]);
	}
}
