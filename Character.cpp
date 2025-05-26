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
	

	StartPosition_.x = csv.GetValueFloat(1, 1);
	StartPosition_.y = csv.GetValueFloat(1, 2);
	StartPosition_.z = csv.GetValueFloat(1, 3);
	this->transform_.rotate_.x = csv.GetValueFloat(1, 4);
	this->transform_.rotate_.y = csv.GetValueFloat(1, 5);
	this->transform_.rotate_.z = csv.GetValueFloat(1, 6);
	this->transform_.scale_.x = csv.GetValueFloat(1, 7);
	this->transform_.scale_.y = csv.GetValueFloat(1, 8);
	this->transform_.scale_.z = csv.GetValueFloat(1, 9);
	FrontDirection_ = { csv.GetValueFloat(1, 10),csv.GetValueFloat(1, 11),csv.GetValueFloat(1, 12) };
	Velocity_ = csv.GetValueFloat(1, 13);
	
    Acceleration_ = csv.GetValueFloat(1, 14);
    AcceleValue_ = csv.GetValueFloat(1, 15);
    FullAccelerate_ = csv.GetValueFloat(1, 16);
	ForwardVector_ = { csv.GetValueFloat(1, 17), csv.GetValueFloat(1, 18), csv.GetValueFloat(1, 19) };

	MoveRotateX = csv.GetValueFloat(1, 20);
    FastRotateX = csv.GetValueFloat(1, 21);

    Gravity_ = csv.GetValueFloat(1, 22);
	IsOnGround_ = false;
    JumpSpeed_ = csv.GetValueFloat(1, 23);
	HeightLowerLimit_ = csv.GetValueFloat(1, 24);
	HeightUpperLimit_ = csv.GetValueFloat(1, 25);

    ColliderSize_ = csv.GetValueFloat(1, 26);
	KnockBack_Direction_ = { csv.GetValueFloat(1, 27), csv.GetValueFloat(1, 28) , csv.GetValueFloat(1, 29) };
    KnockBack_Velocity_ = { csv.GetValueFloat(1, 30), csv.GetValueFloat(1, 31) , csv.GetValueFloat(1, 32) };
	KnockBackPower_ = csv.GetValueFloat(1, 33);
	DecelerationRate_ = csv.GetValueFloat(1, 34);
	KnockBackEnd_ = csv.GetValueFloat(1, 35);

    InvincibilityTime_ =  csv.GetValueFloat(1, 36);
	IsInvincibility_ = false;
    InvincibilityValue = csv.GetValueFloat(1, 37);
}

void Character::CharacterGravity()
{
	JumpSpeed_ -= Gravity_;//�d�͕��̒l�������A�v���C���[�͏�ɉ������ɗ͂��������Ă���
	this->transform_.position_.y += JumpSpeed_;//�t�B�[���h�ɏ���Ă��邩�͊֌W�Ȃ��d�͂͂����葱����

	if (this->transform_.position_.y <= HeightLowerLimit_)//�v���C���[�߂肱�ݖh�~�Ɉ��ȉ���y���W�Œl���Œ�
	{
		this->transform_.position_.y = HeightLowerLimit_;
		IsOnGround_ = true;
	}
}

void Character::ShadowInit()
{
	pGround_ = (Ground*)FindObject("Ground");
	hShadow_ = Model::Load("Model\\ShadowPoint.fbx");
	assert(hShadow_ >= 0);
}

void Character::ShadowSet()
{
	int hGroundModel = pGround_->GetModelHandle();    //���f���ԍ����擾
	RayCastData data;
	data.start = this->transform_.position_;//���C�̔��ˈʒu
	data.dir = XMFLOAT3(0, -1, 0);    //���C�̕���
	Model::RayCast(hGroundModel, &data); //���C�𔭎�

	//���C������������
	if (data.hit)
	{
		ShadowHeight_ = (this->transform_.position_.y - data.dist) + 0.05;
	}
	this->ShadowTrans_.position_.x = this->transform_.position_.x;
	this->ShadowTrans_.position_.z = this->transform_.position_.z;
	this->ShadowTrans_.position_.y = ShadowHeight_;
}

void Character::ShadowDraw()
{
	Model::SetTransform(hShadow_, this->ShadowTrans_);
	Model::Draw(hShadow_);
}

void Character::CharacterMoveRotate(XMVECTOR _direction,float rotateY)
{
	//�����Ă��郍�[�J���̕����x�N�g�����x����]����
	XMVECTOR prev = RotateVecFront(rotateY, _direction);

	//�P�ʃx�N�g�������A�ړ��������m��
	MoveDirection_ = XMVector3Normalize(prev);

	CreateMoveVector();

	//��O�łȂ���Έʒu�X�V 
	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, NewPositon_);
	if (!IsOutsideStage(tmp))
	{
		MoveConfirm();
	}
}

void Character::CharacterMove(XMVECTOR _direction)
{
	//�P�ʃx�N�g�������A�ړ��������m��
	MoveDirection_ = XMVector3Normalize(_direction);

	CreateMoveVector();

	//��O�łȂ���Έʒu�X�V 
	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, NewPositon_);
	if (!IsOutsideStage(tmp))
	{
		MoveConfirm();
	}
}

void Character::CreateMoveVector()
{
	//�ړ��x�N�g�� = �ړ����� * ((�����x + �����x) * 1f�̈ړ��ʂ̃X�P�[�����O)
	//�ړ��x�N�g��������
	XMVECTOR MoveVector = XMVectorScale(MoveDirection_, (Velocity_ + Acceleration_) * DeltaTime);

	//���݈ʒu�ƈړ��x�N�g�������Z
	XMVECTOR PrevPos = XMLoadFloat3(&this->transform_.position_);
	NewPositon_ = PrevPos + MoveVector;
}

bool Character::IsOutsideStage(XMFLOAT3 _position)
{
	if (_position.x > EastEnd || _position.x < WestEnd || _position.z > NorthEnd || _position.z < SouthEnd){
			return true;
	}
	else {
		return false;
	}
}

void Character::MoveConfirm()
{
	XMStoreFloat3(&this->transform_.position_, NewPositon_);
}

void Character::Reflect(XMVECTOR myVector, XMVECTOR eVector, float myVelocity, float eVelocity)
{
	//���G��ԂȂ珈�����Ȃ�
	if (this->IsInvincibility_)
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
	KnockBack_Direction_ = tmp;

	//���g�̑��x�Ƒ���̑��x�̍������Ƃ�
	float subVelocity = myVelocity - eVelocity;

	//���x�������g�̕������ȏ�Ȃ�A���g�̃m�b�N�o�b�N�ʂ�0
	if (subVelocity >= 20.0f)
	{
		KnockBackPower_ = 0.0f;
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
		KnockBackPower_ = LinearCompletion(subVelocity, 0, 20, 2, 4);
	}

	KnockBack_Velocity_.x = KnockBackPower_;
	KnockBack_Velocity_.z = KnockBackPower_;

	Acceleration_ = 0;
}

void Character::KnockBack()
{
	this->transform_.rotate_.x += MoveRotateX;

	//���t���[�����x������
	KnockBack_Velocity_.x *= DecelerationRate_;
	KnockBack_Velocity_.z *= DecelerationRate_;

	//�ʒu = �ʒu + ���� * ���x
	XMFLOAT3 TmpPos = this->transform_.position_;
	TmpPos.x += KnockBack_Direction_.x * KnockBack_Velocity_.x;
	TmpPos.z += KnockBack_Direction_.z * KnockBack_Velocity_.z;

	NewPositon_ = XMLoadFloat3(&TmpPos);

	//��O�łȂ���Έʒu�X�V 
	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, NewPositon_);
	if (!IsOutsideStage(tmp))
	{
		MoveConfirm();
	}
}

void Character::WallHit()
{
	//���x���Z�b�g
	Acceleration_ = 0.0f;

	//���ʃx�N�g���̋t�x�N�g�����v�Z
	XMVECTOR negate = XMVectorNegate(ForwardVector_);
	XMFLOAT3 inverse;
	XMStoreFloat3(&inverse, negate);

	//�m�b�N�o�b�N�����ɑ��
	KnockBack_Direction_ = { inverse.x, inverse.y, inverse.z };

	KnockBack_Velocity_.x = KnockBackPower_;
	KnockBack_Velocity_.z = KnockBackPower_;
}

void Character::InvincibilityTimeCalclation()
{
	//���G���Ԃ̌v�Z
	if (IsInvincibility_)
	{
		if (--InvincibilityTime_ < 0)
		{
			InvincibilityTime_ = InvincibilityValue;
			IsInvincibility_ = false;
		}
	}
}

void Character::Charging()
{
	//�`���[�W�����̉����ʂ����Z��������
	if (Acceleration_ < FullAccelerate_)
	{
		Acceleration_ += AcceleValue_;
	}
	else
	{
		Acceleration_ = FullAccelerate_;
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

XMVECTOR Character::CalclationForward(float rotY, XMVECTOR front)
{
	XMVECTOR v = front;
	XMMATRIX m = XMMatrixRotationY(XMConvertToRadians(rotY));
	XMVECTOR forward = XMVector3TransformNormal(v,m);
	return  forward;
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
	//�v���C���[�̔w��Ɍ��̗��q
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
