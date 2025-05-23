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
	//const int EnemyAttackTime = 180;//敵が攻撃するまでの時間

	const int EnemyAttackTimeArray[] = { 180,150,120,60 };
	}

Enemy::Enemy(GameObject* parent)
	:Character(parent,"Enemy"), hEnemy_(-1), pPlayer_(nullptr), pGround_(nullptr),
	HitStopTimer_(0)
{
	srand((unsigned)time(NULL));
}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{

	std::string path = "CSVdata\\EnemyData.csv";
	SetcsvStatus(path);

	hEnemy_ = Model::Load("Model\\chara2.fbx");
	assert(hEnemy_ >= 0);

	//EnemyFrontDirection = XMVector3TransformCoord(EnemyFrontDirection, GetWorldMatrix());  //getworldmatrixで変換
	ShadowInit();
	SetStartPosition();

	pPlayer_ = (Player*)FindObject("Player");
	//pGround_ = (Ground*)FindObject("Ground");
	SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), ColliderSize_);
	this->AddCollider(collision);

	EnemyState_ = S_IDLE;

	randaim = rand() % 4;
}

void Enemy::Update()
{
	PlayerPosition_ = pPlayer_->GetWorldPosition();//プレイヤーの位置（ワールド座標）
	pPositionVec_ = XMLoadFloat3(&PlayerPosition_);//プレイヤーの位置をベクトル化し取り続ける
	
	//正面ベクトルからどれだけ回転したかを計算し、前向きベクトルを計算
	ForwardVector_ = RotateVecFront(this->transform_.rotate_.y, FrontDirection_);

	ShadowSet();

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

	InvincibilityTimeCalclation();
	
	CharacterGravity();
}

void Enemy::Draw()
{
	Model::SetTransform(hEnemy_, transform_);
	Model::Draw(hEnemy_);

	ShadowDraw();

#ifdef _DEBUG
	if (ImGui::Button("EnemystateChange"))
	{
		if (!EnemyState_ == S_IDLE)
			EnemyState_ = S_IDLE;
		else
			EnemyState_ = S_ROOT;
	}

	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, MoveDirection_);

	//ImGui::Text("front.x:%3f", (float)tmp.x);
	//ImGui::Text("front.y:%3f", (float)tmp.y);
	//ImGui::Text("front.z:%3f", (float)tmp.z);

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
	CharacterMove(MoveDirection_);

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
	if (KnockBack_Velocity_.x <= KnockBackEnd_ || KnockBack_Velocity_.z <= KnockBackEnd_)
	{
		transform_.rotate_.x = 0.0f;
		EnemyState_ = S_ROOT;
	}
	KnockBack();
}

void Enemy::UpdateWallHit()
{
	KnockBack();
	if (KnockBack_Velocity_.x <= KnockBackEnd_ || KnockBack_Velocity_.z <= KnockBackEnd_)
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
	SetChargingEffect("PaticleAssets\\circle_R.png");
	this->transform_.rotate_.x -= FastRotateX;

	//	EnemyAttackTime = 180

	if (++AimTimer_ > EnemyAttackTimeArray[randaim])
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

void Enemy::UpdateAttack()
{
	SetAttackLocusEffect();
	CharacterMove(MoveDirection_);

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
			KnockBack_Velocity_.x = KnockBackPower_;
			KnockBack_Velocity_.z = KnockBackPower_;

			EnemyState_ = S_WALLHIT;
		}
	}

	if (pTarget->GetObjectName() == "Player")
	{
		Acceleration_ = 0;
		AimTimer_ = 0;
	}
}

void Enemy::PlayerReflect(XMVECTOR _vector,bool _isDush)
{
	XMFLOAT3 f;
	XMStoreFloat3(&f, _vector);
	KnockBack_Direction_ = f;

	if (_isDush)
	{
		KnockBack_Velocity_.x = KnockBackPower_ * 1.5;
		KnockBack_Velocity_.z = KnockBackPower_ * 1.5;
	}
	else
	{
		KnockBack_Velocity_.x = KnockBackPower_;
		KnockBack_Velocity_.z = KnockBackPower_;
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

	//プレイヤーの位置(ベクトル)から敵の位置(ベクトル)を引く
	XMVECTOR enemyVector = XMLoadFloat3(&this->transform_.position_);
	XMVECTOR PlayerDist = XMVectorSubtract(pPositionVec_ ,enemyVector);

	//回転する方向を設定(初期化)
	XMVECTOR RotateDirection = XMVectorZero();

	//距離を単位ベクトル化(PlayerDistが0,0,0でエラー防止)
	if(!XMVector3Equal(PlayerDist, XMVectorZero()))
	{
		RotateDirection = XMVector3Normalize(PlayerDist);
	}

	//敵への移動方向を保管
	MoveDirection_ = RotateDirection;

	//------------角度に応じて回転------------
	
	//二つのベクトル間のラジアン角を求める
	XMVECTOR angle = XMVector3AngleBetweenVectors(RotateDirection, ForwardVector_);

	//前向きベクトルとプレイヤーのいる方向のベクトルの外積を求める
	XMVECTOR cross = XMVector3Cross(ForwardVector_, RotateDirection);

	//前向きベクトルとプレイヤー方向ベクトルはXZに伸びるベクトルなので
	//外積のY軸（+か-で左右どちらにいるか判断）を求める
	float crossY = XMVectorGetY(cross);

	//二つのベクトル間のラジアン角を度数に戻し
	//一定以上開いているなら回転
	float Dig = XMConvertToDegrees(XMVectorGetX(angle));
	if (Dig > 3)
	{
		//外積Yが0以上なら左周り(半時計周り)
		if (crossY > 0.0)
		{
			transform_.rotate_.y -= 1.5f;
		}
		//外積Yが0以下なら右周り(時計周り)
		else if (crossY < 0.0)
		{
			transform_.rotate_.y += 1.5f;
		}
	}

	transform_.Calclation();
}

float Enemy::PlayerEnemyDistanceX()
{
	XMVECTOR EnemyPosition = XMLoadFloat3(&this->transform_.position_);
	XMVECTOR DistVec = XMVectorSubtract(EnemyPosition, pPositionVec_);
	float tmp = XMVectorGetX(XMVector3Length(DistVec));
	return tmp;
}
