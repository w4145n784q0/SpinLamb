#include "Enemy.h"

#include"Engine/Model.h"
#include"Engine/Input.h"
#include"Engine/Camera.h"
#include"Engine/SphereCollider.h"

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
	SphereCollider* col_body = new SphereCollider(XMFLOAT3(0, 0, 0), 0.1f);
	this->AddCollider(col_body);

	SphereCollider* col_eye = new SphereCollider(XMFLOAT3(0, 0, 0), 8.0f);
	this->AddCollider(col_eye);

}

void Enemy::Update()
{
}

void Enemy::Draw()
{
	Model::SetTransform(hModel_Enemy, transform_);
	Model::Draw(hModel_Enemy);
}

void Enemy::Release()
{
}

void Enemy::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Player")
	{
		transform_.position_.y = 2.0f;
	}
}
