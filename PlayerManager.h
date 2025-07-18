#pragma once
#include "Engine/GameObject.h"
#include"Player.h"
#include"Enemy.h"

class PlayerManager :
    public GameObject
{
private:

	//----------�C���X�^���X----------
	Player* pPlayer1_;
	Player* pPlayer2_;
	Enemy* pEnemy_;

public:
	PlayerManager(GameObject* parent);
	~PlayerManager();

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;
};

