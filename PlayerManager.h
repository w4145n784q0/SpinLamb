#pragma once
#include "Engine/GameObject.h"
#include"Player.h"
#include"Enemy.h"

class PlayerManager :
    public GameObject
{
private:

	//----------インスタンス----------
	Player* pPlayer1_;
	Player* pPlayer2_;
	Enemy* pEnemy_;

public:
	PlayerManager(GameObject* parent);
	~PlayerManager();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
};

