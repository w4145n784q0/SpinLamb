#pragma once
#include "Engine/GameObject.h"
#include"Player.h"
#include"Enemy.h"

//��ʉE���̃~�j�}�b�v��\������N���X
class MiniMap :
    public GameObject
{
private:
	//----------�C���X�^���X----------
	Player* pPlayer_;
	Enemy* pEnemy_;

	//----------�}�b�v��̈ʒu----------
	XMFLOAT3 playerPos_;
	XMFLOAT3 enemyPos_;

public:
	MiniMap(GameObject* parent);
	~MiniMap();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void SetCSV();

	XMFLOAT3 GetPlayerPos() { return playerPos_; }
	XMFLOAT3 GetEnemyPos() { return enemyPos_; }
};

