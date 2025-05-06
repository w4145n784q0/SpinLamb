#include "EnemyManager.h"
#include"Enemy.h"

namespace
{
	const int EnemyNum = 1;
}

EnemyManager::EnemyManager(GameObject* parent)
	:GameObject(parent,"EnemyManager"),EnemyNum_(EnemyNum)
{
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::Initialize()
{
	EnemyInitialize();
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
