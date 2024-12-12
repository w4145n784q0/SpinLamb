#include "Enemy.h"

#include"Engine/Model.h"
#include"Engine/Input.h"
#include"Engine/Camera.h"
#include"Engine/SphereCollider.h"

namespace
{
	float speed_ = 3.0f;
}

Enemy::Enemy(GameObject* parent)
	:GameObject(parent,"Enemy"),hModel_Enemy(-1),pPlayer(nullptr)
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{
	hModel_Enemy = Model::Load("enemy.fbx");
	transform_.position_ = { 0,0,7 };
	pPlayer = (Player*)FindObject("Player");
	

	//SphereCollider* col_eye = new SphereCollider(XMFLOAT3(0, 0, 0), 5.0f);
	//this->AddCollider(col_eye);

}

void Enemy::Update()
{
	EnemyPosition = XMLoadFloat3(&this->transform_.position_);

	switch (enemy_state)
	{
	case Enemy::S_IDLE:
		UpdateIdle();
		break;
	case Enemy::S_SUSPICIOUS:
		break;
	case Enemy::S_CHASE:
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
	XMFLOAT3 a;
	
	XMFLOAT3 pPosition = pPlayer.GetPosition();
	XMVECTOR pPositionVec = pPlayer.GetPlayerPosition();
	XMFLOAT3 x;
	XMStoreFloat3(&x, pPositionVec);

	//自分(enmey)と相手(player)の座標上の距離をはかる
	a.x = this->transform_.position_.x - pPosition.x;
	a.y = this->transform_.position_.y - pPosition.y;
	a.z = this->transform_.position_.z - pPosition.z;

	float Pointdist = sqrt(pow(a.x, 2) + pow(a.y, 2) + pow(a.z, 2));

	//XMVECTOR v = XMLoadFloat3(&a);
	//v = XMVector3Normalize(v);//距離をはかったらvector化


	XMVECTOR forward = FrontVec(this->transform_.rotate_.y);//自分の前方ベクトル
	XMVECTOR dot = XMVector3Dot(pPositionVec, forward);//相手へのベクトルと自分の前方ベクトルの内積をとる
	float cosine = XMVectorGetX(dot);
	float sin = XMVectorGetY(dot);

	if (cosine > cosf(XMConvertToRadians(60)) && Pointdist < FrontLength) //距離は60度以内かand相手との距離がFrontLengthより小さい
	{
		int i = 0;
	}
	
}

void Enemy::UpdateChase()
{

}

void Enemy::OnCollision(GameObject* pTarget)
{
	/*if (pTarget->GetObjectName() == "Player")
	{
		transform_.position_.y = 2.0f;
	}*/
}

//bool Enemy::ComparePosition(XMFLOAT3 pos)
//{
//	//if(transform_.position_.x){}
//}
