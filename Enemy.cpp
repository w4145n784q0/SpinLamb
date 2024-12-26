#include "Enemy.h"

#include"Engine/Model.h"
#include"Engine/Input.h"
#include"Engine/Camera.h"
#include"Engine/SphereCollider.h"

namespace
{
	float speed_ = 6.0f;
	const int EyeAngle = 60;
	const float DeltaTime = 0.016f;
}

Enemy::Enemy(GameObject* parent)
	:GameObject(parent,"Enemy"),hModel_Enemy(-1),pPlayer_(nullptr),IsHit_(false),
	Fov(XMConvertToRadians(EyeAngle)),EnemyDirection({0,0,1})
{
	//基準ベクトルをつくる　0,0,1
	EnemyDirection = XMVector3TransformCoord(EnemyDirection,GetWorldMatrix());  //getworldmatrixで変換
	//EPの確度を計算　XMVector3AngleBetweenVectors
	//正規化して
	//その分回す
}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{
	hModel_Enemy = Model::Load("enemy.fbx");
	transform_.position_ = { 10,1,10 };
	//transform_.rotate_.y = 180;
	pPlayer_ = (Player*)FindObject("Player");
	

	SphereCollider* col = new SphereCollider(XMFLOAT3(0, 0, 0), 0.1f);
	this->AddCollider(col);

}

void Enemy::Update()
{
	pPosition = pPlayer_->GetPosition();
	pPositionVec = pPlayer_->GetPlayerPosition();
	EnemyPosition = XMLoadFloat3(&this->transform_.position_);

	switch (EnemyState)
	{
	case Enemy::S_IDLE:
		UpdateIdle();
		break;
	case Enemy::S_SUSPICIOUS:
		break;
	case Enemy::S_CHASE:
		UpdateChase();
		break;
	case Enemy::S_ATTACK:
		break;
	case Enemy::S_MOVE:
		break;
	case Enemy::S_WINCE:
		break;
	case Enemy::S_MAX:
		break;
	default:
		break;
	}
}

void Enemy::Draw()
{
	Model::SetTransform(hModel_Enemy, transform_);
	Model::Draw(hModel_Enemy);
}

void Enemy::Release()
{
}

void Enemy::UpdateIdle()
{	

	XMFLOAT3 x;
	XMStoreFloat3(&x, pPositionVec);

	//自分(enmey)と相手(player)の座標上の距離をはかる(二点間の距離)
	//a.x = this->transform_.position_.x - pPosition.x;
	//a.y = this->transform_.position_.y - pPosition.y;
	//a.z = this->transform_.position_.z - pPosition.z;
	//float Pointdist = sqrt(pow(a.x, 2) + pow(a.y, 2) + pow(a.z, 2));

	//自分(enemy)と相手(player)の距離をはかる(ベクトル)
	XMVECTOR DistVec = XMVectorSubtract(EnemyPosition, pPositionVec);
	float Pointdist = XMVectorGetX(XMVector3Length(DistVec));

	XMVECTOR forward = RotateVecFront(this->transform_.rotate_.y,EnemyDirection);//自分の前方ベクトル(回転した分も含む)
	DistVec = XMVector3Normalize(DistVec);//自分と相手の距離ベクトル 内積の計算用
	XMVECTOR dot = XMVector3Dot(DistVec, forward);//相手とのベクトルと自分の前方ベクトルの内積をとる
	float cosine = XMVectorGetX(dot);

	if (cosine > cosf(Fov) && Pointdist < FrontLength) //距離は60度以内かand相手との距離がFrontLengthより小さい
	{
		IsHit_ = true;
	}
	else {
		IsHit_ = false;
	}
	
	if (IsHit_)
	{
		EnemyState = S_CHASE;
	}
}

void Enemy::UpdateChase()
{
	//変更
	XMVECTOR PlayerDist = XMVectorSubtract(pPositionVec,EnemyPosition);
	PlayerDist = XMVector3Normalize(PlayerDist);//プレイヤーとの距離を正規化
	XMVECTOR normFront = XMVector3Normalize(EnemyDirection);//正面ベクトルを正規化
	XMVECTOR cross = XMVector3Cross(normFront,PlayerDist) ;//上記2つの外積を出す
	float crossY= XMVectorGetY(cross);

	
	
	if (crossY > 0)
	{
		transform_.rotate_.y += 1.0f;
	}
	else if (crossY < 0)
	{
		transform_.rotate_.y -= 1.0f;
	}
	else if(fabs(crossY < 1e-6))
	{

	}
	

	//変更

	XMFLOAT3 pPos = pPlayer_->GetPosition();//プレイヤーの位置
	ChasePoint = pPos - this->transform_.position_;//自分（enemy）とプレイヤーの位置を引き
	XMVECTOR ChaseVec = XMLoadFloat3(&ChasePoint);//それをベクトル化

	//ChaseVec = XMVector3TransformCoord(ChaseVec, EnemyRot);//回転行列をかけて
	XMVECTOR norm = XMVector3Normalize(ChaseVec);//正規化

	XMVECTOR MoveVector = XMVectorScale(norm, speed_  * DeltaTime);//移動ベクトル化する
	XMVECTOR PrevPos = EnemyPosition;
	XMVECTOR NewPos = PrevPos + MoveVector;
	XMStoreFloat3(&this->transform_.position_, NewPos);
}

void Enemy::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Player")
	{
		pPlayer_->SetPosition({ 0,0,0 });
		EnemyState = S_IDLE;
	}
}

//bool Enemy::ComparePosition(XMFLOAT3 pos)
//{
//	//if(transform_.position_.x){}
//}
