#include "Enemy.h"

#include"Engine/Model.h"
#include"Engine/Input.h"
#include"Engine/Camera.h"
#include"Engine/SphereCollider.h"
#include"BattleScene.h"
#include"Engine/SceneManager.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

namespace
{
	//const int EyeAngle = 60;
	//const float EyeLength = 10.0f;
	const int HitStop = 2;//ヒットストップする時間
	const float ChaseLength = 10.0f;//追跡状態から攻撃準備に移る距離
	const int EnemyAttackTime = 180;//敵が攻撃するまでの時間

	const XMVECTOR EnemyFrontDirection = { 0,0,1 };//敵の正面の基準ベクトル ここからどれだけ回転したか
	float velocity = 6.0f;//初速度
	const float Enemy_Gravity = 0.08;
	
	const float MoveRotateX = 10.0f;//移動時の1fの回転量
	const float FastRotateX = 30.0f;//(チャージ中など)高速回転中の1fの回転量
	const float FullAccelerate = 50.0f;//最大加速度


	const float KnockBackPower = 2.0f; //ノックバックする強さ
	
	const int InvincibilityValue = 120;//無敵時間の定数

	}

Enemy::Enemy(GameObject* parent)
	:GameObject(parent, "Enemy"), hEnemy_(-1), pPlayer_(nullptr),pGround_(nullptr),
    ForwardVector_({0,0,0}),
	IsOnGround_(true),Acceleration_(0.0f), AcceleValue_(1.0f),
	HitStopTimer_(0),IsInvincibility_(false),InvincibilityTime_(InvincibilityValue),ColliderSize_(1.5f)
{
	transform_.position_ = { 0,0,0 };
}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{

	hEnemy_ = Model::Load("Model\\chara2.fbx");
	assert(hEnemy_ >= 0);

	transform_.position_ = { 0.0,0.5 ,5.0 };

	//EnemyFrontDirection = XMVector3TransformCoord(EnemyFrontDirection, GetWorldMatrix());  //getworldmatrixで変換


	pPlayer_ = (Player*)FindObject("Player");
	//pGround_ = (Ground*)FindObject("Ground");
	SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), ColliderSize_);
	this->AddCollider(collision);

	EnemyState_ = S_IDLE;

}

void Enemy::Update()
{
	PlayerPosition_ = pPlayer_->GetWorldPosition();//プレイヤーの位置（ワールド座標）
	pPositionVec_ = XMLoadFloat3(&PlayerPosition_);//プレイヤーの位置をベクトル化し取り続ける
	EnemyPosition_ = XMLoadFloat3(&this->transform_.position_);//敵の位置をベクトル化し取り続ける
	ForwardVector_ = RotateVecFront(this->transform_.rotate_.y, EnemyFrontDirection);//自分の前方ベクトル(回転した分も含む)を更新

	switch (EnemyState_)
	{
	case Enemy::S_IDLE:
		UpdateIdle();
		break;
	case Enemy::S_ROOT:
		UpdateRoot();
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
			InvincibilityTime_ = InvincibilityValue;
			IsInvincibility_ = false;
		}
	}

	JumpSpeed_ -= Enemy_Gravity;//重力分の値を引き、プレイヤーは常に下方向に力がかかっている
	this->transform_.position_.y += JumpSpeed_;//フィールドに乗っているかは関係なく重力はかかり続ける

	if (this->transform_.position_.y <= 0.5f && IsOnGround_)//めりこみ防止に一定以下のy座標で値を固定
	{
		this->transform_.position_.y = 0.5f;
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
			EnemyState_ = S_ROOT;
	}

	/*XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, ForwardVector_);*/

	//ImGui::Text("front.x:%3f", (float)this->transform_.position_.x);
	//ImGui::Text("front.y:%3f", (float)this->transform_.position_.y);
	//ImGui::Text("front.z:%3f", (float)this->transform_.position_.z);

	//ImGui::Text("EnemyLife:%.3f", (float)CharacterLife_);
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

void Enemy::UpdateRoot()
{
	float dist = PlayerEnemyDistanceX();

	if (dist > ChaseLength)//一定距離以上離れているなら追跡
	{
		EnemyState_ = S_CHASE;
	}
	else//接近しているなら攻撃準備
	{
		EnemyState_ = S_AIM;
	}
}

void Enemy::UpdateChase()
{
	this->transform_.rotate_.x -= MoveRotateX;

	LookPlayer();
	XMVECTOR MoveVector = XMVectorScale(AttackVector_, (velocity + Acceleration_) * DeltaTime);//移動ベクトル化する
	XMVECTOR PrevPos = EnemyPosition_;
	XMVECTOR NewPos = PrevPos + MoveVector;
	
	XMStoreFloat3(&this->transform_.position_, NewPos);
	//this->transform_.position_.y = 0.5f;

	float dist = PlayerEnemyDistanceX();
	if (dist < ChaseLength)
	{
		EnemyState_ = S_AIM;
		this->transform_.rotate_.x = 0.0f;
	}
}

void Enemy::UpdateHitStop()
{
	//ヒットストップ活用時のみ使用
	if (++HitStopTimer_ > HitStop)
	{
		HitStopTimer_ = 0;
		EnemyState_ = S_HIT;
	}
}

void Enemy::UpdateHit()
{
	if (KnockBack_Velocity_.x <= 0.5f || KnockBack_Velocity_.z <= 0.5f)
	{
		transform_.rotate_.x = 0.0f;
		EnemyState_ = S_ROOT;
	}
	Blown();
}

void Enemy::UpdateWallHit()
{
	Blown();
	if (KnockBack_Velocity_.x <= 0.1f || KnockBack_Velocity_.z <= 0.1f)
	{
		transform_.rotate_.x = 0.0f;
		EnemyState_ = S_ROOT;
		IsInvincibility_ = true;

		SceneManager* pSM = (SceneManager*)FindObject("SceneManager");
		if (pSM->IsBattleScene())
		{
			BattleScene* pBattleScene = (BattleScene*)FindObject("BattleScene");
			//pBattleScene->SetPlayerHp(CharacterLife_);
			pBattleScene->PlusPlayerScore();
		}
	}

	/*if (--deadTimer_ < 0)
	{
		CharacterLife_--;
		deadTimer_ = deadTimerValue;
		EnemyState_ = S_ROOT;
		IsInvincibility_ = true;
		this->transform_.position_ = { 0,0,0 };
	}*/
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

void Enemy::Blown()
{
	this->transform_.rotate_.x -= MoveRotateX;

	//毎フレーム速度を減少
	KnockBack_Velocity_.x *= 0.9;
	KnockBack_Velocity_.z *= 0.9;

	//毎フレームpositionに方向を加算
	//位置 = 位置 + 方向 * 速度
	XMFLOAT3 TmpPos = this->transform_.position_;
	TmpPos.x += KnockBack_Direction_.x * KnockBack_Velocity_.x;
	TmpPos.z += KnockBack_Direction_.z * KnockBack_Velocity_.z;

	XMVECTOR NewPos = XMLoadFloat3(&TmpPos);

	//場外でなければ位置更新 
	XMFLOAT3 f;
	XMStoreFloat3(&f, NewPos);
	if (!(f.x > 60.0f || f.x < -60.0f || f.z > 60.0f || f.z < -60.0f))
	{
		XMStoreFloat3(&this->transform_.position_, NewPos);
	}
}

void Enemy::UpdateAttack()
{
	//移動ベクトルを計算(方向 * 速度(初速 + 加速))
	XMVECTOR MoveVector = XMVectorScale(AttackVector_,(velocity + Acceleration_) * DeltaTime);

	//敵の位置に移動ベクトルを加算
	XMVECTOR PrevPos = EnemyPosition_;
	XMVECTOR NewPos = PrevPos + MoveVector;

	//場外でなければ位置更新 
	XMFLOAT3 f;
	XMStoreFloat3(&f, NewPos);
	if (!(f.x > 60.0f || f.x < -60.0f || f.z > 60.0f || f.z < -60.0f))
	{
		XMStoreFloat3(&this->transform_.position_, NewPos);
	}

	//速度を毎フレーム減少
	Acceleration_ -= AcceleValue_;

	//ノックバック時回転
	this->transform_.rotate_.x -= FastRotateX;

	if (Acceleration_ <= 0.0f)
	{
		transform_.rotate_.x = 0.0f;
		EnemyState_ = S_ROOT;
	}
}

void Enemy::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Fence")
	{
		if (!IsInvincibility_ && !(EnemyState_ == S_WALLHIT))
		{
		//	KnockBack_Velocity_ = { 0,0,0 };
			Acceleration_ = 0.0f;
			XMFLOAT3 inverse;
			XMStoreFloat3(&inverse, ForwardVector_);
			KnockBack_Direction_ = { inverse.x * -1, inverse.y * -1, inverse.z * -1 };
			KnockBack_Velocity_.x = KnockBackPower;
			KnockBack_Velocity_.z = KnockBackPower;

			EnemyState_ = S_WALLHIT;
		}
	}

	if (pTarget->GetObjectName() == "Player")
	{
		Acceleration_ = 0;
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
	//XMVECTOR DistVec = XMVectorSubtract(EnemyPosition_, pPositionVec_);
	//float Pointdist = XMVectorGetX(XMVector3Length(DistVec));

	//敵と自機の距離（座標）
	XMFLOAT3 LookPoint = PlayerEnemyDistanceFloat3();//プレイヤーの位置-敵の位置で距離をとる

	//敵と自機の回転処理
	XMVECTOR front = EnemyFrontDirection;//計算用の前向きベクトル（初期値が入る）
	XMMATRIX mvec = transform_.matRotate_;//現在の回転している方向（自分の回転行列）
	front = XMVector3Transform(front, mvec);//正面からどれだけ回転しているか

	XMVECTOR PlayerDist = XMLoadFloat3(&LookPoint);//ベクトルにする
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

XMFLOAT3 Enemy::PlayerEnemyDistanceFloat3()
{
	XMFLOAT3 dist = PlayerPosition_ - this->transform_.position_;
	return dist;
}

float Enemy::PlayerEnemyDistanceX()
{
	XMVECTOR DistVec = XMVectorSubtract(EnemyPosition_, pPositionVec_);
	float tmp = XMVectorGetX(XMVector3Length(DistVec));
	return tmp;
}
