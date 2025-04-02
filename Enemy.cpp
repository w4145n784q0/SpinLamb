#include "Enemy.h"

#include"Engine/Model.h"
#include"Engine/Input.h"
#include"Engine/Camera.h"
#include"Engine/SphereCollider.h"
#include"BossBattleScene.h"

namespace
{
	float speed_ = 6.0f;
	const int EyeAngle = 60;
	const float EyeLength = 10.0f;
	const float DeltaTime = 0.016f;
	const float Enemy_Gravity = 0.08; //0.16333f
	const float KnockBackPower = 8.0f; //ノックバックする強さ
	const float mu = 0.8; //摩擦係数

}

Enemy::Enemy(GameObject* parent)
	:GameObject(parent,"Enemy"),hEnemy_(-1),pPlayer_(nullptr),IsHit_(false), FrontLength_(EyeLength),
	Eye_(XMConvertToRadians(EyeAngle)),EnemyFrontDirection_({0,0,1}),isStop_(true),MoveTimer_(0)
{
	transform_.position_ = { 0,0,0 };
}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{

	hEnemy_ = Model::Load("Enemy.fbx");
	assert(hEnemy_ >= 0);

	transform_.position_ = { 0.0,0.5 ,5.0 };
	transform_.scale_ = { 1.5,1.5,1.5 };

	//基準ベクトルをつくる　0,0,1
	//EnemyFrontDirection = XMVector3TransformCoord(EnemyFrontDirection, GetWorldMatrix());  //getworldmatrixで変換


	pPlayer_ = (Player*)FindObject("Player");
	pGround_ = (Ground*)FindObject("Ground");
	SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), 1.5f);
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
	case Enemy::S_HIT:
		UpdateHit();
		break;
	case Enemy::S_AIM:
		UpdateAim();
		break;
	case Enemy::S_MAX:
		break;
	default:
		break;
	}

	if (transform_.position_.x > 30.0f || transform_.position_.x < -30.0f ||
		transform_.position_.z > 30.0f || transform_.position_.z < -30.0f)
	{
		IsOnGround_ = false;
	}
	else
	{
		IsOnGround_ = true;
	}
	JumpSpeed_ -= Enemy_Gravity;//重力分の値を引き、プレイヤーは常に下方向に力がかかっている
	this->transform_.position_.y += JumpSpeed_;//フィールドに乗っているかは関係なく重力はかかり続ける

	if (this->transform_.position_.y <= 0.5f && IsOnGround_)//プレイヤーめりこみ防止に一定以下のy座標で値を固定
	{
		this->transform_.position_.y = 0.5f;
	}
	if (this->transform_.position_.y < -400)
	{
		//this->transform_.position_.y = -200;//高さの最低値
		BossBattleScene* pBossBattleScene = (BossBattleScene*)FindObject("BossBattleScene");
		pBossBattleScene->PhasePlus();
		KillMe();
	}
}

void Enemy::Draw()
{
	Model::SetTransform(hEnemy_, transform_);
	Model::Draw(hEnemy_);
}

void Enemy::Release()
{
}

void Enemy::UpdateIdle()
{	
	//自分(enemy)と相手(player)の距離をはかる(ベクトル)
	XMVECTOR DistVec = XMVectorSubtract(EnemyPosition_, pPositionVec_);
	float Pointdist = XMVectorGetX(XMVector3Length(DistVec));

	XMVECTOR forward = RotateVecFront(this->transform_.rotate_.y,EnemyFrontDirection_);//自分の前方ベクトル(回転した分も含む)
	DistVec = XMVector3Normalize(DistVec);//自分と相手の距離ベクトル 内積の計算用
	XMVECTOR dot = XMVector3Dot(DistVec, forward);//相手とのベクトルと自分の前方ベクトルの内積をとる
	float cosine = XMVectorGetX(dot);

	if (cosine > cosf(Eye_) && Pointdist < FrontLength_) //距離は60度以内か相手との距離がFrontLengthより小さい
	{
		IsHit_ = true;
	}
	else {
		IsHit_ = false;
	}
	
	if (IsHit_)
	{
		EnemyState_ = S_CHASE;
	}
}

void Enemy::UpdateChase()
{
	//自分(enemy)と相手(player)の距離をはかる(ベクトル)
	XMVECTOR DistVec = XMVectorSubtract(EnemyPosition_, pPositionVec_);
	float Pointdist = XMVectorGetX(XMVector3Length(DistVec));
	if (Pointdist > FrontLength_ + 5)
	{
		EnemyState_ = S_IDLE;
	}

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

	float crossY= XMVectorGetY(cross);//外積のY軸（+か-で左右どちらにいるか判断）
	
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
	XMVECTOR MoveVector = XMVectorScale(normDist, speed_  * DeltaTime);//移動ベクトル化する
	XMVECTOR PrevPos = EnemyPosition_;
	XMVECTOR NewPos = PrevPos + MoveVector;
	
	XMStoreFloat3(&this->transform_.position_, NewPos);
	this->transform_.position_.y = 0.5f;

}

void Enemy::UpdateHit()
{
	/*this->transform_.position_.x += ReflectMove.x;
	this->transform_.position_.z += ReflectMove.z;

	ReflectMove.x * 0.6;
	ReflectMove.z * 0.6;

	if (ReflectMove.x <= 0.0f || ReflectMove.z <= 0.0f)
	{
		EnemyState_ = S_AIM;
	}*/

	//速度を下げていく
	KnockBack_Velocity_.x *= 0.5;
	KnockBack_Velocity_.z *= 0.5;

	//毎フレームpositionに速度を加算
	transform_.position_.x += KnockBack_Velocity_.x;
	transform_.position_.z += KnockBack_Velocity_.z;

	if (KnockBack_Velocity_.x <= 0.5f || KnockBack_Velocity_.z <= 0.5f)
	{
		EnemyState_ = S_AIM;
	}

}

void Enemy::UpdateAim()
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

	if (++AimTimer_ > 180)
	{
		AimTimer_ = 0;
		Acceleration_ = 40;
		EnemyState_ = S_ATTACK;
	}

}

void Enemy::UpdateAttack()
{
	//移動ベクトルを計算(方向 * 速度(初速 + 加速))
	XMVECTOR MoveVector = XMVectorScale(AttackVector_,(speed_ + Acceleration_) * DeltaTime);

	//敵の位置に移動ベクトルを加算
	XMVECTOR PrevPos = EnemyPosition_;
	XMVECTOR NewPos = PrevPos + MoveVector;

	XMStoreFloat3(&this->transform_.position_, NewPos);

	//速度を毎フレーム減少
	Acceleration_--;

	if (Acceleration_ <= 0.0f)
	{
		EnemyState_ = S_AIM;
	}
}

void Enemy::OnCollision(GameObject* pTarget)
{
	
}

void Enemy::PlayerReflect(XMVECTOR _vector,bool _isDush)
{
	XMFLOAT3 f;
	XMStoreFloat3(&f, _vector);
	KnockBack_Direction_ = f;

	/*if (_isDush)
	{
		f.x *= KnockBackPower * 1.5;
		f.z *= KnockBackPower * 1.5;
	}
	else
	{
		f.x *= KnockBackPower;
		f.z *= KnockBackPower;
	}*/
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