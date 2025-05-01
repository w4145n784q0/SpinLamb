#include "Enemy.h"

#include"Engine/Model.h"
#include"Engine/Input.h"
#include"Engine/Camera.h"
#include"Engine/SphereCollider.h"
#include"BattleScene.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

namespace
{
	float speed = 6.0f;
	const int EyeAngle = 60;
	const float EyeLength = 10.0f;
	const float DeltaTime = 0.016f;

	const float FullAccelerate = 40.0f;//最大加速度
	const float MoveRotateX = 10.0f;//移動時の1fの回転量
	const float FastRotateX = 30.0f;//(チャージ中など)高速回転中の1fの回転量

	const float Enemy_Gravity = 0.08; //0.16333f
	const float KnockBackPower = 2.0f; //ノックバックする強さ
	const int EnemyAttackTime = 180;//敵が攻撃するまでの時間

	const int deadTimerValue = 60;//復活までの時間
	const int Invincibility = 120;//無敵時間の定数

}

Enemy::Enemy(GameObject* parent)
	:GameObject(parent, "Enemy"), hEnemy_(-1), pPlayer_(nullptr), IsHit_(false), FrontLength_(EyeLength),
	Eye_(XMConvertToRadians(EyeAngle)), EnemyFrontDirection_({ 0,0,1 }), IsOnGround_(true),Acceleration_(0.0f), AcceleValue_(1.0f),
	HitStopTimer_(0), deadTimer_(deadTimerValue),IsInvincibility_(false),InvincibilityTime_(Invincibility),ColliderSize_(1.5f), CharacterLife_(3)
{
	transform_.position_ = { 0,0,0 };
}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{

	hEnemy_ = Model::Load("chara.fbx");
	assert(hEnemy_ >= 0);

	transform_.position_ = { 0.0,0.5 ,5.0 };
	//transform_.scale_ = { 1.5,1.5,1.5 };
	//transform_.rotate_.y = 180.0;

	//基準ベクトルをつくる　0,0,1
	//EnemyFrontDirection = XMVector3TransformCoord(EnemyFrontDirection, GetWorldMatrix());  //getworldmatrixで変換


	pPlayer_ = (Player*)FindObject("Player");
	pGround_ = (Ground*)FindObject("Ground");
	SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), ColliderSize_);
	this->AddCollider(collision);

	EnemyState_ = S_AIM;

}

void Enemy::Update()
{
	XMFLOAT3 tmp = pPlayer_->GetWorldPosition();
	pPositionVec_ = XMLoadFloat3(&tmp);
	EnemyPosition_ = XMLoadFloat3(&this->transform_.position_);

	switch (EnemyState_)
	{
	case Enemy::S_IDLE:
		UpdateIdle();
		break;
	case Enemy::S_CHASE:
		UpdateChase();
		break;
	case Enemy::S_ATTACK:
		UpdateAttack();
		break;
	case Enemy::S_HITSTOP:
		UpdateHitStop();
		break;
	case Enemy::S_HIT:
		UpdateHit();
		break;
	case Enemy::S_WALLHIT:
		UpdateWallHit();
		break;
	case Enemy::S_AIM:
		UpdateAim();
		break;
	case Enemy::S_ONALEAT:
		UpdateOnAlert();
		break;
	case Enemy::S_MAX:
		break;
	default:
		break;
	}

	if (!IsInvincibility_ && !(EnemyState_ == S_WALLHIT))//壁ダメージ判定
	{
		if (transform_.position_.x > 60.0f || transform_.position_.x < -60.0f ||
			transform_.position_.z > 60.0f || transform_.position_.z < -60.0f)
		{
			EnemyState_ = S_WALLHIT;
		}
	}

	//無敵時間の計算
	if (IsInvincibility_)
	{
		if (--InvincibilityTime_ < 0)
		{
			InvincibilityTime_ = Invincibility;
			IsInvincibility_ = false;
		}
	}

	JumpSpeed_ -= Enemy_Gravity;//重力分の値を引き、プレイヤーは常に下方向に力がかかっている
	this->transform_.position_.y += JumpSpeed_;//フィールドに乗っているかは関係なく重力はかかり続ける

	if (this->transform_.position_.y <= 0.5f && IsOnGround_)//めりこみ防止に一定以下のy座標で値を固定
	{
		this->transform_.position_.y = 0.5f;
	}
	if (this->transform_.position_.y < -400)
	{
		//this->transform_.position_.y = -200;//高さの最低値
		//BossBattleScene* pBossBattleScene = (BossBattleScene*)FindObject("BossBattleScene");
		//pBossBattleScene->PhasePlus();
		KillMe();
	}
}

void Enemy::Draw()
{
	Model::SetTransform(hEnemy_, transform_);
	Model::Draw(hEnemy_);

#ifdef _DEBUG
	if (ImGui::Button("EnemystateChange"))
	{
		if (!EnemyState_ == S_IDLE)
			EnemyState_ = S_IDLE;
		else
			EnemyState_ = S_AIM;

	}

	//ImGui::Text("E:mutekijkan:%.3f", (float)InvincibilityTime_);
	ImGui::Text("EnemyLife:%.3f", (float)CharacterLife_);
#endif
}

void Enemy::Release()
{
}

void Enemy::UpdateIdle()
{	
	////自分(enemy)と相手(player)の距離をはかる(ベクトル)
	//XMVECTOR DistVec = XMVectorSubtract(EnemyPosition_, pPositionVec_);
	//float Pointdist = XMVectorGetX(XMVector3Length(DistVec));

	//XMVECTOR forward = RotateVecFront(this->transform_.rotate_.y,EnemyFrontDirection_);//自分の前方ベクトル(回転した分も含む)
	//DistVec = XMVector3Normalize(DistVec);//自分と相手の距離ベクトル 内積の計算用
	//XMVECTOR dot = XMVector3Dot(DistVec, forward);//相手とのベクトルと自分の前方ベクトルの内積をとる
	//float cosine = XMVectorGetX(dot);

	//if (cosine > cosf(Eye_) && Pointdist < FrontLength_) //距離は60度以内か相手との距離がFrontLengthより小さい
	//{
	//	IsHit_ = true;
	//}
	//else {
	//	IsHit_ = false;
	//}
	//
	//if (IsHit_)
	//{
	//	EnemyState_ = S_CHASE;
	//}
}

void Enemy::UpdateChase()
{
	LookPlayer();
	XMVECTOR MoveVector = XMVectorScale(AttackVector_, speed * DeltaTime);//移動ベクトル化する
	XMVECTOR PrevPos = EnemyPosition_;
	XMVECTOR NewPos = PrevPos + MoveVector;
	
	XMStoreFloat3(&this->transform_.position_, NewPos);
	this->transform_.position_.y = 0.5f;

}

void Enemy::UpdateHitStop()
{
	if (++HitStopTimer_ > 2)
	{
		HitStopTimer_ = 0;
		EnemyState_ = S_HIT;
	}
}

void Enemy::UpdateHit()
{

	this->transform_.rotate_.x -= FastRotateX;

	//速度を下げていく
	KnockBack_Velocity_.x *= 0.9;
	KnockBack_Velocity_.z *= 0.9;

	//毎フレームpositionに方向を加算
	//位置 = 位置 + 方向 * 速度
	transform_.position_.x += KnockBack_Direction_.x * KnockBack_Velocity_.x;
	transform_.position_.z += KnockBack_Direction_.z * KnockBack_Velocity_.z;

	if (KnockBack_Velocity_.x <= 0.5f || KnockBack_Velocity_.z <= 0.5f)
	{
		transform_.rotate_.x = 0.0f;
		EnemyState_ = S_AIM;
	}

}

void Enemy::UpdateWallHit()
{
	if (--deadTimer_ < 0)
	{
		CharacterLife_--;
		deadTimer_ = deadTimerValue;
		EnemyState_ = S_AIM;
		IsInvincibility_ = true;
		this->transform_.position_ = { 0,0,0 };
	}
}

void Enemy::UpdateAim()
{
	LookPlayer();
	//this->transform_.rotate_.x -= ChargeRotateX;

	if (++AimTimer_ > EnemyAttackTime)
	{
		AimTimer_ = 0;
		Acceleration_ = FullAccelerate;
		EnemyState_ = S_ATTACK;
	}

}

void Enemy::UpdateOnAlert()
{
	LookPlayer();
}

void Enemy::UpdateAttack()
{
	//移動ベクトルを計算(方向 * 速度(初速 + 加速))
	XMVECTOR MoveVector = XMVectorScale(AttackVector_,(speed + Acceleration_) * DeltaTime);

	//敵の位置に移動ベクトルを加算
	XMVECTOR PrevPos = EnemyPosition_;
	XMVECTOR NewPos = PrevPos + MoveVector;

	XMStoreFloat3(&this->transform_.position_, NewPos);

	//速度を毎フレーム減少
	Acceleration_ -= AcceleValue_;

	//ノックバック時回転
	this->transform_.rotate_.x -= FastRotateX;

	if (Acceleration_ <= 0.0f)
	{
		transform_.rotate_.x = 0.0f;
		EnemyState_ = S_AIM;
	}
}

void Enemy::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Fence")
	{
		if (!IsInvincibility_ && !(EnemyState_ == S_WALLHIT))
		{
			KnockBack_Velocity_ = { 0,0,0 };
			Acceleration_ = 0.0f;
			EnemyState_ = S_WALLHIT;
		}
	}
}

void Enemy::PlayerReflect(XMVECTOR _vector,bool _isDush)
{
	XMFLOAT3 f;
	XMStoreFloat3(&f, _vector);
	KnockBack_Direction_ = f;

	if (_isDush)
	{
		KnockBack_Velocity_.x = KnockBackPower * 1.5;
		KnockBack_Velocity_.z = KnockBackPower * 1.5;
	}
	else
	{
		KnockBack_Velocity_.x = KnockBackPower;
		KnockBack_Velocity_.z = KnockBackPower;
	}

	EnemyState_ = S_HIT;
}

void Enemy::LookPlayer()
{
	//自分(enemy)と相手(player)の距離をはかる(ベクトル)
	XMVECTOR DistVec = XMVectorSubtract(EnemyPosition_, pPositionVec_);
	float Pointdist = XMVectorGetX(XMVector3Length(DistVec));

	//敵と自機の距離
	XMFLOAT3 playerPos = pPlayer_->GetWorldPosition();//プレイヤーの位置（ワールド座標）
	ChasePoint_ = playerPos - this->transform_.position_;//プレイヤーの位置-敵の位置で距離をとる

	//敵と自機の回転処理
	XMVECTOR front = EnemyFrontDirection_;//計算用の前向きベクトル（初期値が入る）
	XMMATRIX mvec = transform_.matRotate_;//現在の回転している方向（自分の回転行列）
	front = XMVector3Transform(front, mvec);
	XMVECTOR PlayerDist = XMLoadFloat3(&ChasePoint_);//ベクトルにする
	XMVECTOR normDist = XMVector3Normalize(PlayerDist);//プレイヤーとの距離を正規化
	XMVECTOR angle = XMVector3AngleBetweenVectors(normDist, front);//二つのベクトル間のラジアン角を求める
	XMVECTOR cross = XMVector3Cross(front, normDist);

	AttackVector_ = normDist;//攻撃方向を保存

	float crossY = XMVectorGetY(cross);//外積のY軸（+か-で左右どちらにいるか判断）

	//float angleX = XMVectorGetX(angle);
	//float dig = XMConvertToDegrees(angleX);

	float Dig = XMConvertToDegrees(XMVectorGetX(angle));
	if (Dig > 3)
	{
		if (crossY > 0.0)
		{
			transform_.rotate_.y -= 1.5f;
		}
		else if (crossY < 0.0)
		{
			transform_.rotate_.y += 1.5f;
		}
	}

	transform_.Calclation();
}
