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
	JumpParam_. JumpSpeed_ -= JumpParam_.Gravity_;//重力分の値を引き、プレイヤーは常に下方向に力がかかっている
	this->transform_.position_.y += JumpParam_.JumpSpeed_;//フィールドに乗っているかは関係なく重力はかかり続ける

	if (this->transform_.position_.y <= JumpParam_.HeightLowerLimit_)//プレイヤーめりこみ防止に一定以下のy座標で値を固定
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
	int hGroundModel = ShadowParam_.pGround_->GetModelHandle();    //モデル番号を取得
	RayCastData data;
	data.start = this->transform_.position_;//レイの発射位置
	data.dir = XMFLOAT3(0, -1, 0);    //レイの方向
	Model::RayCast(hGroundModel, &data); //レイを発射

	//レイが当たったら
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
	//向いているローカルの方向ベクトルをＹ軸回転する
	XMVECTOR prev = RotateVecFront(rotateY, _direction);

	//単位ベクトル化し、移動方向を確定
	MoveParam_.MoveDirection_ = XMVector3Normalize(prev);

	CreateMoveVector();

	//場外でなければ位置更新 
	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, MoveParam_.NewPositon_);
	if (!IsOutsideStage(tmp))
	{
		MoveConfirm();
	}
}

void Character::CharacterMove(XMVECTOR _direction)
{
	//単位ベクトル化し、移動方向を確定
	MoveParam_.MoveDirection_ = XMVector3Normalize(_direction);

	CreateMoveVector();

	//場外でなければ位置更新 
	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, MoveParam_.NewPositon_);
	if (!IsOutsideStage(tmp))
	{
		MoveConfirm();
	}
}

void Character::CreateMoveVector()
{
	//移動ベクトル = 移動方向 * ((初速度 + 加速度) * 1fの移動量のスケーリング)
	//移動ベクトル化する
	XMVECTOR MoveVector = XMVectorScale(MoveParam_.MoveDirection_, (MoveParam_.Velocity_ + MoveParam_.Acceleration_) * DeltaTime);

	//現在位置と移動ベクトルを加算
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
	//無敵状態なら処理しない
	if (this->WallHitParam_.IsInvincibility_)
	{
		return;
	}

	//接触した面に垂直な方向に反射する

	//接触相手のベクトルから自身のベクトルを引き、正規化
	XMVECTOR subVector = XMVector3Normalize(XMVectorSubtract(eVector ,myVector ));
	
	//逆ベクトルにして反射方向を決定
	subVector = XMVectorNegate(subVector);
	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp,subVector);

	//反射方向を設定
	 HitParam_.KnockBack_Direction_ = tmp;

	//自身の速度と相手の速度の差分をとる
	float subVelocity = myVelocity - eVelocity;

	//ノックバック量の初期化
	//値が変化するのでローカル変数
	float KnockBackValue = 0.0f;

	//速度差が自身の方が一定以上なら、自身のノックバック量は0
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
	//ノックバックする速度= ノックバックする強さ(定数) * ノックバックする方向
	this->transform_.rotate_.x += RotateParam_.MoveRotateX;

	//毎フレーム速度を減少
	HitParam_.KnockBack_Velocity_.x *= HitParam_.DecelerationRate_;
	HitParam_.KnockBack_Velocity_.z *= HitParam_.DecelerationRate_;

	//位置 = 位置 + 方向 * 速度
	XMFLOAT3 TmpPos = this->transform_.position_;
	TmpPos.x += HitParam_.KnockBack_Direction_.x * HitParam_.KnockBack_Velocity_.x;
	TmpPos.z += HitParam_.KnockBack_Direction_.z * HitParam_.KnockBack_Velocity_.z;

	MoveParam_.NewPositon_ = XMLoadFloat3(&TmpPos);

	//場外でなければ位置更新 
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

	//速度リセット
	MoveParam_.Acceleration_ = 0.0f;

	//正面ベクトルの逆ベクトルを計算
	XMVECTOR negate = XMVectorNegate(MoveParam_.ForwardVector_);
	XMFLOAT3 inverse;
	XMStoreFloat3(&inverse, negate);

	//ノックバック方向に代入
	HitParam_.KnockBack_Direction_ = { inverse.x, inverse.y, inverse.z };

	//ノックバック量を速度に代入(一定値)
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
	//無敵時間の計算
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
	//チャージ中一定の加速量を加算し続ける
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
	//回転させたいベクトル（方向）を代入
	//基本はローカル正面ベクトル
	XMVECTOR v = front;

	//Y回転をラジアンにし、回転行列を作成
	XMMATRIX m = XMMatrixRotationY(XMConvertToRadians(rotY));

	//方向ベクトルを回転行列で変換し、ワールド座標での向いている方向が出る
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
