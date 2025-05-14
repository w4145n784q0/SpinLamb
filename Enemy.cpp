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
	}

Enemy::Enemy(GameObject* parent)
	:Character(parent,"Enemy"), hEnemy_(-1), pPlayer_(nullptr), pGround_(nullptr),
	HitStopTimer_(0)
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

	std::string path = "CSVdata\\EnemyData.csv";
	SetcsvStatus(path);

	//EnemyFrontDirection = XMVector3TransformCoord(EnemyFrontDirection, GetWorldMatrix());  //getworldmatrixで変換

	SetStartPosition();

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
	ForwardVector_ = RotateVecFront(this->transform_.rotate_.y, FrontDirection_);//自分の前方ベクトル(回転した分も含む)を更新

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
	
	CharacterGravity();
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

	//LookPlayer();
	//XMVECTOR MoveVector = XMVectorScale(AttackVector_, (Velocity_ + Acceleration_) * DeltaTime);//移動ベクトル化する
	//XMVECTOR PrevPos = XMLoadFloat3(&this->transform_.position_);
	//XMVECTOR NewPos = PrevPos + MoveVector;

	CreateMoveVector();
	
	XMStoreFloat3(&this->transform_.position_, NewPositon_);
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
		Acceleration_ = FullAccelerate_;
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
	////移動ベクトルを計算(方向 * 速度(初速 + 加速))
	//XMVECTOR MoveVector = XMVectorScale(AttackVector_,(Velocity_ + Acceleration_) * DeltaTime);

	////敵の位置に移動ベクトルを加算
	//XMVECTOR PrevPos = EnemyPosition_;
	//XMVECTOR NewPos = PrevPos + MoveVector;

	CreateMoveVector();

	//場外でなければ位置更新 
	XMFLOAT3 f;
	XMStoreFloat3(&f, NewPositon_);
	if (!(f.x > 60.0f || f.x < -60.0f || f.z > 60.0f || f.z < -60.0f))
	{
		XMStoreFloat3(&this->transform_.position_, NewPositon_);
	}

	//速度を毎フレーム減少
	Acceleration_ -= AcceleValue_;

	//キャラモデル回転
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

	
	//------------敵と自機の回転処理------------

	//XMMATRIX mvec = transform_.matRotate_;//現在の回転している方向（自分の回転行列）

	//正面ベクトルからどれだけ回転したかを計算し、前向きベクトルを計算
	XMVECTOR front = RotateVecFront(this->transform_.rotate_.y, FrontDirection_);

	//プレイヤーの位置-敵の位置で距離をとり、ベクトル化する
	XMFLOAT3 LookDir = PlayerEnemyDistanceFloat3();
	XMVECTOR PlayerDist = XMLoadFloat3(&LookDir);

	//距離を単位ベクトル化
	MoveDirection_ = XMVector3Normalize(PlayerDist);

	//攻撃方向を保存
	//AttackVector_ = MoveDirection_;


	//------------角度に応じて回転------------
	
	//二つのベクトル間のラジアン角を求める
	XMVECTOR angle = XMVector3AngleBetweenVectors(MoveDirection_, front);

	//前向きベクトルとプレイヤーのいる方向のベクトルの外積を求める
	XMVECTOR cross = XMVector3Cross(front, MoveDirection_);

	//外積のY軸（+か-で左右どちらにいるか判断）を求める
	float crossY = XMVectorGetY(cross);

	//二つのベクトル間のラジアン角を度数に戻し
	//一定以上開いているなら回転
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

void Enemy::Look()
{

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
