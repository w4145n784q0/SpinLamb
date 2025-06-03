#include "Character.h"
#include"Engine/VFX.h"
#include"Engine/Model.h"
#include"Engine/Audio.h"
#include"Engine/Global.h"

namespace {
	
	//キャラクタークラスの共通変数を初期化するインデックス

	enum move
	{
		vel = 0,
		accele_value,
		accele_max,
	};

	enum rotate
	{
		moverot = 0,
		fastrot ,
		
	};

	enum Jump
	{
		gravity = 0,
		upperlimit,
		lowerlimit,
	};

	enum Hit
	{
		collider = 0,
		originalrangemin,
		originalrangemax,
		convertedrangemin,
		convertedrangemax,
		knockbackpower,
		deceleration,
		knockbackend,
	};

	enum WallHit
	{
		invincibilityvalue = 0,
		blinkvalue
	};

	enum Shadow
	{
		shadowcorrection = 0,
	};

	//ゲームに登場するキャラクターの数
	enum GameChara
	{
		player = 0,
		enemy,
		MaxChara
	};

	//エフェクト初期化時のインデックス
	enum EffectParam
	{
		Charge = 0,
		Locus,
		Hit,
		WallHit,
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
	hSoundcharge_ = Audio::Load("Sound\\SE\\charge.wav",false,MaxChara);
	assert(hSoundcharge_ >= 0);
	hSoundattack_ = Audio::Load("Sound\\SE\\attack.wav", false, MaxChara);
	assert(hSoundattack_ >= 0);
	hSoundCollision_ = Audio::Load("Sound\\SE\\collision.wav",false,1);
	assert(hSoundCollision_ >= 0);
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
		MoveParam_.Velocity_ = v[vel];
		MoveParam_.AcceleValue_ = v[accele_value];
		MoveParam_.FullAccelerate_ = v[accele_max];
	}

	std::string p_rotate = "RotateParam";
	if (csv.IsGetParamName(p_rotate))
	{
		std::vector<float> v = csv.GetParam(p_rotate);
		RotateParam_.MoveRotateX = v[moverot];
		RotateParam_.FastRotateX = v[fastrot];

	}

	std::string p_jump = "JumpParam";
	if (csv.IsGetParamName(p_jump))
	{
		std::vector<float> v = csv.GetParam(p_jump);
		JumpParam_.Gravity_ = v[gravity];
		JumpParam_.HeightLowerLimit_ = v[upperlimit];
		JumpParam_.HeightUpperLimit_ = v[lowerlimit];
	}

	std::string p_hit = "HitParam";
	if (csv.IsGetParamName(p_hit))
	{
		std::vector<float> v = csv.GetParam(p_hit);
		HitParam_.ColliderSize_ =v[collider];

		HitParam_.OriginaRangeMin_ = v[originalrangemin];
		HitParam_.OriginaRangeMax_ = v[originalrangemax];
		HitParam_.ConvertedRangeMin_ = v[convertedrangemin];
		HitParam_.ConvertedRangeMax_ = v[convertedrangemax];

		HitParam_.KnockBackPower_ = v[knockbackpower];
		HitParam_.DecelerationRate_ = v[deceleration];
		HitParam_.KnockBackEnd_ = v[knockbackend];
	}

	std::string p_wallhit = "WallHitParam";
	if (csv.IsGetParamName(p_wallhit))
	{
		std::vector<float> v = csv.GetParam(p_wallhit);
		WallHitParam_.InvincibilityValue = static_cast<int>(v[invincibilityvalue]);
		WallHitParam_.blinkValue = static_cast<int>(v[blinkvalue]);
	}

	std::string p_shadow = "ShadowParam";
	if(csv.IsGetParamName(p_shadow))
	{
		std::vector<float> v = csv.GetParam(p_shadow);
		ShadowParam_.ShadowCorrection_ = v[shadowcorrection];
	}
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
	//if (!IsOutsideStage(tmp))
	//{
		MoveConfirm();
	//}
}

void Character::CharacterMove(XMVECTOR _direction)
{
	//単位ベクトル化し、移動方向を確定
	MoveParam_.MoveDirection_ = XMVector3Normalize(_direction);

	CreateMoveVector();

	//場外でなければ位置更新 
	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, MoveParam_.NewPositon_);
	//if (!IsOutsideStage(tmp))
	//
		MoveConfirm();
	//}
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

	//速度差の判定は線形補完元の最大値を適用
	//速度差が自身の方が一定以上なら、自身のノックバック量は0
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

		//ノックバック量の線形補完を行う
		KnockBackValue = LinearCompletion(subVelocity,
			HitParam_.OriginaRangeMin_, HitParam_.OriginaRangeMax_,
			HitParam_.ConvertedRangeMin_, HitParam_.ConvertedRangeMax_);
	}

	HitParam_.KnockBack_Velocity_.x = KnockBackValue;
	HitParam_.KnockBack_Velocity_.z = KnockBackValue;

	AccelerationStop();
}

void Character::KnockBack()
{
	MoveRotateReverse();

	//ノックバックする速度= ノックバックする強さ(定数) * ノックバックする方向
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
	AccelerationStop();

	//正面ベクトルの逆ベクトルを計算
	XMVECTOR negate = XMVectorNegate(MoveParam_.ForwardVector_);
	XMFLOAT3 inverse;
	XMStoreFloat3(&inverse, negate);

	//ノックバック方向に代入
	HitParam_.KnockBack_Direction_ = { inverse.x, inverse.y, inverse.z };

	//ノックバック量を速度に代入(一定値)
	HitParam_.KnockBack_Velocity_.x = HitParam_.KnockBackPower_;
	HitParam_.KnockBack_Velocity_.z = HitParam_.KnockBackPower_;

	WallHitParam_.IsInvincibility_ = true;
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
			WallHitParam_.InvincibilityTime_ = 0;
			WallHitParam_.IsInvincibility_ = false;
		}
	}
}

void Character::Charging()
{
	Audio::Play(hSoundcharge_);

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

void Character::InitCSVEffect()
{
	CsvReader csv;
	csv.Load("CSVData\\VFXData.csv");

	//ChargeParam_...はvector<float>型のパラメータ
	std::string effects[] = { "Charge","Locus" , "Hit" , "WallHit" };
    std::vector<float>* param[] = { &ChargeParam_, &AttackLocusParam_, &HitEffectParam_, &WallHitEffectParam_ };  
    for (int i = 0; i < sizeof(effects) / sizeof(effects[0]); i++)  
    {  
		if (csv.IsGetParamName(effects[i]))  
        {  
			std::vector<float> v = csv.GetParam(effects[i]);  
			*param[i] = v;  
            // SetEmitterで実際にVFXのパラメータにセットされる  
        }  
    }
}

void Character::SetChargingEffect(std::string _path)
{
	EmitterData charge;
	
	VFX::SetEmitter(charge, ChargeParam_);
	charge.textureFileName = _path;
	charge.position = this->transform_.position_;

	/*charge.delay = 0;
	charge.lifeTime = 15;
	
	charge.positionRnd = XMFLOAT3(1, 1, 1);
	charge.direction = { 0,1,0 };
	charge.speed = 0.18;
	charge.number = (DWORD)1;*/

	VFX::Start(charge);
}

void Character::SetAttackLocusEffect()
{
	EmitterData locus;
	VFX::SetEmitter(locus, AttackLocusParam_);
	locus.textureFileName = "PaticleAssets\\flashB_Y.png";
	locus.position = this->transform_.position_;
	

	/*locus.delay = 0;
	locus.number = (DWORD)3;
	locus.position = this->transform_.position_;
	locus.positionRnd = { 1,1,1 };
	locus.direction = { 0,0,1 };
	locus.sizeRnd = { 0.5,0.5 };
	locus.lifeTime = (DWORD)10;*/
	VFX::Start(locus);
}

void Character::SetHitEffect()
{
	EmitterData hit;

	VFX::SetEmitter(hit, HitEffectParam_);
	hit.textureFileName = "PaticleAssets\\flashB_W.png";
	hit.position = this->transform_.position_;
	/*hit.position.y = this->transform_.position_.y + 1.0f;
	hit.delay = 0;
	hit.direction = { 1,1,0 };
	hit.directionRnd = XMFLOAT3(90, 90, 90);
	hit.speed = 0.5;
	hit.speedRnd = 1.0;
	hit.accel = 1.0;
	hit.lifeTime = (DWORD)10.0;
	hit.number = (DWORD)10;
	hit.sizeRnd = XMFLOAT2(0.4, 0.4);*/
	VFX::Start(hit);
}

void Character::SetWallHitEffect()
{
	EmitterData  wallhit;
	VFX::SetEmitter(wallhit, WallHitEffectParam_);
	wallhit.textureFileName = "PaticleAssets\\flashB_W.png";
	wallhit.position = this->transform_.position_;


	/*wallhit.direction = { 1,1,0 };
	wallhit.directionRnd = { 360,360,0 };
	wallhit.number = (DWORD)10;
	wallhit.delay = 0;
	wallhit.speed = 0.3f;
	wallhit.lifeTime = 20;*/
	VFX::Start(wallhit);
}
