#include "Character.h"
#include"Engine/VFX.h"
#include"Engine/Model.h"
#include"Engine/Audio.h"
#include"Engine/Global.h"

namespace {
	
	//キャラクタークラスの共通変数を初期化するインデックス

	enum moveIndex
	{
		i_vel = 0,
		i_accele_value,
		i_accele_max,
		i_friction
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

	//ゲームに登場するキャラクターの数
	enum GameCharaIndex
	{
		i_player = 0,
		i_enemy,
		i_MaxChara
	};

	//エフェクト初期化時のインデックス
	enum EffectParamIndex
	{
		i_Charge = 0,
		i_Locus,
		i_Hit,
		i_WallHit,
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
	hSoundcharge_ = Audio::Load("Sound\\SE\\charge.wav",false, i_MaxChara);
	assert(hSoundcharge_ >= 0);
	hSoundattack_ = Audio::Load("Sound\\SE\\attack.wav", false, i_MaxChara);
	assert(hSoundattack_ >= 0);
	hSoundCollision_ = Audio::Load("Sound\\SE\\collision.wav",false,1);
	assert(hSoundCollision_ >= 0);

	GetWireNormal();
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

		//HitParam_.KnockBackPower_ = v[i_knockbackpower];
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
			Model::SetTransform(_handle, _transform);
			Model::Draw(_handle);
		}
	}
	else
	{
		Model::SetTransform(_handle, _transform);
		Model::Draw(_handle);
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
	int hGroundModel = ShadowParam_.pGround_->GetModelHandle();//ステージのモデル番号を取得
	RayCastData data;
	data.start = this->transform_.position_;//レイの発射位置を設定
	data.dir = XMFLOAT3(0, -1, 0);//レイの方向を設定
	Model::RayCast(hGroundModel, &data);//レイを発射

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

	//接触相手のベクトルから自身のベクトルを引き、正規化
	XMVECTOR subVector = XMVector3Normalize(XMVectorSubtract(eVector ,myVector));
	
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

	//毎フレームノックバック速度を減少
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

	//受け取った法線をノックバック方向に代入
	HitParam_.KnockBack_Direction_ = { tmp };

	//ノックバック量を速度に代入(一定値)
	HitParam_.KnockBack_Velocity_.x = WallHitParam_.KnockBackPower_;
	HitParam_.KnockBack_Velocity_.z = WallHitParam_.KnockBackPower_;

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
