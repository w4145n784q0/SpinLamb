#pragma once
#include "Engine/GameObject.h"
#include"Player.h"
#include"Enemy.h"

class MiniMap :
    public GameObject
{
private:
	int hMap_;
	int hPlayerIcon_;
	int hEnemyIcon_;

	Transform mTrans;
	Transform pTrans;
	Transform eTrans;

	Player* pPlayer_;
	Enemy* pEnemy_;
public:
	MiniMap(GameObject* parent);
	~MiniMap();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

