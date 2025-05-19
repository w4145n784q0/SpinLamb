#include "Character.h"
#include"Engine/VFX.h"

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
	JumpSpeed_ -= Gravity_;//重力分の値を引き、プレイヤーは常に下方向に力がかかっている
	this->transform_.position_.y += JumpSpeed_;//フィールドに乗っているかは関係なく重力はかかり続ける

	if (this->transform_.position_.y <= HeightLowerLimit_)//プレイヤーめりこみ防止に一定以下のy座標で値を固定
	{
		this->transform_.position_.y = HeightLowerLimit_;
		IsOnGround_ = true;
	}
}

void Character::CharacterMoveRotate(XMVECTOR _direction,float rotateY)
{
	//向いているローカルの方向ベクトルをＹ軸回転する
	XMVECTOR prev = RotateVecFront(rotateY, _direction);

	//単位ベクトル化し、移動方向を確定
	MoveDirection_ = XMVector3Normalize(prev);

	CreateMoveVector();

	//場外でなければ位置更新 
	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, NewPositon_);
	if (!(tmp.x > 60.0f || tmp.x < -60.0f || tmp.z > 60.0f || tmp.z < -60.0f))
	{
		XMStoreFloat3(&this->transform_.position_, NewPositon_);
	}
}

void Character::CharacterMove(XMVECTOR _direction)
{
	//単位ベクトル化し、移動方向を確定
	MoveDirection_ = XMVector3Normalize(_direction);

	CreateMoveVector();

	//場外でなければ位置更新 
	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, NewPositon_);
	if (!(tmp.x > 60.0f || tmp.x < -60.0f || tmp.z > 60.0f || tmp.z < -60.0f))
	{
		XMStoreFloat3(&this->transform_.position_, NewPositon_);
	}
}

void Character::CreateMoveVector()
{
	//移動ベクトル = 移動方向 * ((初速度 + 加速度) * 1fの移動量のスケーリング)
	//移動ベクトル化する
	XMVECTOR MoveVector = XMVectorScale(MoveDirection_, (Velocity_ + Acceleration_) * DeltaTime);

	//現在位置と移動ベクトルを加算
	XMVECTOR PrevPos = XMLoadFloat3(&this->transform_.position_);
	NewPositon_ = PrevPos + MoveVector;
}

void Character::KnockBack()
{
	this->transform_.rotate_.x += MoveRotateX;

	//毎フレーム速度を減少
	KnockBack_Velocity_.x *= DecelerationRate_;
	KnockBack_Velocity_.z *= DecelerationRate_;

	//位置 = 位置 + 方向 * 速度
	XMFLOAT3 TmpPos = this->transform_.position_;
	TmpPos.x += KnockBack_Direction_.x * KnockBack_Velocity_.x;
	TmpPos.z += KnockBack_Direction_.z * KnockBack_Velocity_.z;

	NewPositon_ = XMLoadFloat3(&TmpPos);

	//場外でなければ位置更新 
	XMFLOAT3 f;
	XMStoreFloat3(&f, NewPositon_);
	if (!(f.x > 60.0f || f.x < -60.0f || f.z > 60.0f || f.z < -60.0f))
	{
		XMStoreFloat3(&this->transform_.position_, NewPositon_);
	}
}

void Character::Charging()
{
	//チャージ中一定の加速量を加算し続ける
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
	//回転させたいベクトル（方向）を代入
	//基本はローカル正面ベクトル
	XMVECTOR v = front;

	//Y回転をラジアンにし、回転行列を作成
	XMMATRIX m = XMMatrixRotationY(XMConvertToRadians(rotY));

	//方向ベクトルを回転行列で変換し、ワールド座標での向いている方向が出る
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
	//プレイヤーの背後に光の粒子
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
