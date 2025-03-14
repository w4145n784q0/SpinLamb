#include "EnemyManager.h"
#include"Enemy.h"

EnemyManager::EnemyManager(GameObject* parent)
	:GameObject(parent,"EnemyManager"),EnemyNum_(3)
{
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::Initialize()
{
	for (int i = 0; i < EnemyNum_; i++)
	{
		Instantiate<Enemy>(this);
	}
}

void EnemyManager::Update()
{
}

void EnemyManager::Draw()
{
}

void EnemyManager::Release()
{
}
