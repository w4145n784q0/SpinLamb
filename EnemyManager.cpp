#include "EnemyManager.h"
#include"Enemy.h"

EnemyManager::EnemyManager(GameObject* parent)
	:GameObject(parent,"EnemyManager"),EnemyNum_(1)
{
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::Initialize()
{
	
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

void EnemyManager::EnemyInitialize()
{
	for (int i = 0; i < EnemyNum_; i++)
	{
		Instantiate<Enemy>(this);
	}
}
