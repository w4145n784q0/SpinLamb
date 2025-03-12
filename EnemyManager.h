#pragma once
#include "Engine/GameObject.h"
class EnemyManager :
    public GameObject
{
public:
	EnemyManager(GameObject* parent);
	~EnemyManager();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

