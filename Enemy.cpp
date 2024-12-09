#include "Enemy.h"

#include"Engine/Model.h"
#include"Engine/Input.h"
#include"Engine/Camera.h"
#include"Engine/SphereCollider.h"

#include"Player.h"

Enemy::Enemy(GameObject* parent)
	:GameObject(parent,"Enemy"),hModel_Enemy(-1)
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{
	hModel_Enemy = Model::Load("enemy.fbx");
	transform_.position_ = { 0,0,7 };

	EnemyPosition = XMLoadFloat3(&this->transform_.position_);

	SphereCollider* col_eye = new SphereCollider(XMFLOAT3(0, 0, 0), 5.0f);
	this->AddCollider(col_eye);

}

void Enemy::Update()
{
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
	Player pPlayer = (Player*)FindObject("Player");
	//XMFLOAT3 PlayerPos = pPlayer.GetPosition();
	//XMVECTOR v = XMLoadFloat3(&PlayerPos);
	
	
	

}

void Enemy::UpdateChase()
{

}

void Enemy::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Player")
	{
		transform_.position_.y = 2.0f;
	}
}

bool Enemy::ComparePosition(XMFLOAT3 pos)
{
	if(transform_.position_.x)

	return false;
}
