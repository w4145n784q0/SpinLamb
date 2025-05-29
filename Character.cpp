#include "Character.h"
#include"Engine/VFX.h"
#include"Engine/Model.h"

Character::Character(GameObject* parent)
	:GameObject(parent,"Character")
{
}

Character::Character(GameObject* parent, const std::string& name):GameObject(parent, name)
{
}

Character::~Character()
{
}

void Character::SetcsvStatus(std::string _path)
{
	CsvReader csv;
	csv.Load(_path);
	
	InitParam_.StartPosition_.x = csv.GetValueFloat(1, 1);
	InitParam_.StartPosition_.y = csv.GetValueFloat(1, 2);
	InitParam_.StartPosition_.z = csv.GetValueFloat(1, 3);
	this->transform_.rotate_.x = csv.GetValueFloat(1, 4);
	this->transform_.rotate_.y = csv.GetValueFloat(1, 5);
	this->transform_.rotate_.z = csv.GetValueFloat(1, 6);
	this->transform_.scale_.x = csv.GetValueFloat(1, 7);
	this->transform_.scale_.y = csv.GetValueFloat(1, 8);
	this->transform_.scale_.z = csv.GetValueFloat(1, 9);

	InitParam_.FrontDirection_ = { csv.GetValueFloat(1, 10),csv.GetValueFloat(1, 11),csv.GetValueFloat(1, 12) };

	MoveParam_.Velocity_ = csv.GetValueFloat(1, 13);
	
	MoveParam_.Acceleration_ = csv.GetValueFloat(1, 14);
	MoveParam_.AcceleValue_ = csv.GetValueFloat(1, 15);
	MoveParam_.FullAccelerate_ = csv.GetValueFloat(1, 16);
	MoveParam_.ForwardVector_ = { csv.GetValueFloat(1, 17), csv.GetValueFloat(1, 18), csv.GetValueFloat(1, 19) };

	RotateParam_. MoveRotateX = csv.GetValueFloat(1, 20);
	RotateParam_.FastRotateX = csv.GetValueFloat(1, 21);

	JumpParam_.Gravity_ = csv.GetValueFloat(1, 22);
	JumpParam_.IsOnGround_ = false;
	JumpParam_.JumpSpeed_ = csv.GetValueFloat(1, 23);
	JumpParam_.HeightLowerLimit_ = csv.GetValueFloat(1, 24);
	JumpParam_.HeightUpperLimit_ = csv.GetValueFloat(1, 25);

	HitParam_.ColliderSize_ = csv.GetValueFloat(1, 26);
	HitParam_.KnockBack_Direction_ = { csv.GetValueFloat(1, 27), csv.GetValueFloat(1, 28) , csv.GetValueFloat(1, 29) };
	HitParam_.KnockBack_Velocity_ = { csv.GetValueFloat(1, 30), csv.GetValueFloat(1, 31) , csv.GetValueFloat(1, 32) };
	HitParam_.KnockBackPower_ = csv.GetValueFloat(1, 33);
	HitParam_.DecelerationRate_ = csv.GetValueFloat(1, 34);
	HitParam_.KnockBackEnd_ = csv.GetValueFloat(1, 35);
	
	WallHitParam_.InvincibilityTime_ =  csv.GetValueFloat(1, 36);
	WallHitParam_.IsInvincibility_ = false;
	WallHitParam_.InvincibilityValue = csv.GetValueFloat(1, 37);
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

void Character::ShadowInit()
{
	ShadowParam_.pGround_ = (Ground*)FindObject("Ground");
	ShadowParam_.hShadow_ = Model::Load("Model\\ShadowPoint.fbx");
	assert(ShadowParam_.hShadow_ >= 0);
}

void Character::ShadowSet()
{
	int hGroundModel = ShadowParam_.pGround_->GetModelHandle();    //���f���ԍ����擾
	RayCastData data;
	data.start = this->transform_.position_;//���C�̔��ˈʒu
	data.dir = XMFLOAT3(0, -1, 0);    //���C�̕���
	Model::RayCast(hGroundModel, &data); //���C�𔭎�

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

void Character::CharacterMoveRotate(XMVECTOR _direction,float rotateY)
{
	//�����Ă��郍�[�J���̕����x�N�g�����x����]����
	XMVECTOR prev = RotateVecFront(rotateY, _direction);

	//�P�ʃx�N�g�������A�ړ��������m��
	MoveParam_.MoveDirection_ = XMVector3Normalize(prev);

	CreateMoveVector();

	//��O�łȂ���Έʒu�X�V 
	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, MoveParam_.NewPositon_);
	if (!IsOutsideStage(tmp))
	{
		MoveConfirm();
	}
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

void Character::Reflect(XMVECTOR myVector, XMVECTOR eVector, float myVelocity, float eVelocity)
{
	//���G��ԂȂ珈�����Ȃ�
	if (this->WallHitParam_.IsInvincibility_)
	{
		return;
	}

	//�ڐG�����ʂɐ����ȕ����ɔ��˂���

	//�ڐG����̃x�N�g�����玩�g�̃x�N�g���������A���K��
	XMVECTOR subVector = XMVector3Normalize(XMVectorSubtract(eVector ,myVector ));
	
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

	//���x�������g�̕������ȏ�Ȃ�A���g�̃m�b�N�o�b�N�ʂ�0
	if (subVelocity >= 20.0f)
	{
		KnockBackValue = 0.0f;
	}
	else 
	{
		if (signbit(subVelocity)) 
		{
			subVelocity = -subVelocity;
		}
		if (subVelocity > 20.0f)
		{
			subVelocity = 20.0f;
		}
		KnockBackValue = LinearCompletion(subVelocity, 0, 20, 2, 4);
	}

	HitParam_.KnockBack_Velocity_.x = KnockBackValue;
	HitParam_.KnockBack_Velocity_.z = KnockBackValue;

	MoveParam_. Acceleration_ = 0;
}

void Character::KnockBack()
{
	//�m�b�N�o�b�N���鑬�x= �m�b�N�o�b�N���鋭��(�萔) * �m�b�N�o�b�N�������
	this->transform_.rotate_.x += RotateParam_.MoveRotateX;

	//���t���[�����x������
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

void Character::WallHit()
{
	SetWallHitEffect();

	//���x���Z�b�g
	MoveParam_.Acceleration_ = 0.0f;

	//���ʃx�N�g���̋t�x�N�g�����v�Z
	XMVECTOR negate = XMVectorNegate(MoveParam_.ForwardVector_);
	XMFLOAT3 inverse;
	XMStoreFloat3(&inverse, negate);

	//�m�b�N�o�b�N�����ɑ��
	HitParam_.KnockBack_Direction_ = { inverse.x, inverse.y, inverse.z };

	//�m�b�N�o�b�N�ʂ𑬓x�ɑ��(���l)
	HitParam_.KnockBack_Velocity_.x = HitParam_.KnockBackPower_;
	HitParam_.KnockBack_Velocity_.z = HitParam_.KnockBackPower_;
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
		if (++WallHitParam_.InvincibilityTime_ > WallHitParam_.InvincibilityValue)
		{
			WallHitParam_.InvincibilityTime_ = WallHitParam_.InvincibilityValue;
			WallHitParam_.IsInvincibility_ = false;
		}
	}
}

void Character::Charging()
{
	//�`���[�W�����̉����ʂ����Z��������
	if (MoveParam_.Acceleration_ < MoveParam_.FullAccelerate_)
	{
		MoveParam_.Acceleration_ += MoveParam_.AcceleValue_;
	}
	else
	{
		MoveParam_.Acceleration_ = MoveParam_.FullAccelerate_;
	}
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

void Character::SetChargingEffect(std::string _path)
{
	EmitterData charge;
	//charge.textureFileName = "PaticleAssets\\circle_B.png";
	charge.textureFileName = _path;
	charge.delay = 0;
	charge.lifeTime = 15;
	charge.position = this->transform_.position_;
	charge.positionRnd = XMFLOAT3(1, 1, 1);
	charge.direction = { 0,1,0 };
	//charge.directionRnd = XMFLOAT3(90, 90, 90);
	charge.speed = 0.18;
	charge.number = (DWORD)1;
	VFX::Start(charge);
}

void Character::SetAttackLocusEffect()
{
	EmitterData locus;
	locus.textureFileName = "PaticleAssets\\flashB_Y.png";
	locus.delay = 0;
	locus.number = (DWORD)3;
	locus.position = this->transform_.position_;
	locus.position.z = this->transform_.position_.z - 0.5f;
	locus.positionRnd = { 1,1,1 };
	locus.direction = { 0,0,1 };
	locus.sizeRnd = { 0.5,0.5 };
	locus.lifeTime = (DWORD)10;
	VFX::Start(locus);
}

void Character::SetHitEffect()
{
	EmitterData hit;
	hit.textureFileName = "PaticleAssets\\flashB_W.png";
	hit.position = this->transform_.position_;
	hit.position.y = this->transform_.position_.y + 1.0f;
	hit.delay = 0;
	hit.direction = { 1,1,0 };
	hit.directionRnd = XMFLOAT3(90, 90, 90);
	hit.speed = 0.5;
	hit.speedRnd = 1.0;
	hit.accel = 1.0;
	hit.lifeTime = 10.0;
	hit.number = (DWORD)10;
	//hit.size = XMFLOAT2(0.1, 0.1);
	hit.sizeRnd = XMFLOAT2(0.4, 0.4);
	//hit.scale = XMFLOAT2(0.99, 0.99);
	//hit.color = XMFLOAT4(1, 1, 0.1, 1);
	//hit.deltaColor = XMFLOAT4(0, 0, 0, 0);
	//hit.gravity = 0.0f;
	VFX::Start(hit);
}

void Character::SetWallHitEffect()
{
	EmitterData  wallhit;
	wallhit.textureFileName = "PaticleAssets\\flashB_W.png";
	wallhit.position = this->transform_.position_;
	wallhit.position.y = this->transform_.position_.y + 1.0f;
	wallhit.direction = { 1,1,0 };
	wallhit.directionRnd = { 360,360,0 };
	wallhit.number = (DWORD)20;
	wallhit.delay = 0;
	wallhit.speed = 0.3f;
	wallhit.lifeTime = 20;
	VFX::Start(wallhit);
}
