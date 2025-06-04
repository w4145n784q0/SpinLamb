#pragma once
#include "Engine/GameObject.h"
#include"Player.h"
#include"Enemy.h"

class MiniMap :
    public GameObject
{
private:
	//----------�摜�n���h��----------

	//�}�b�v�S��
	int hMap_;

	//�v���C���[�̃A�C�R��
	int hPlayerIcon_;

	//�G�̃A�C�R��
	int hEnemyIcon_;

	//----------�e�摜�̃g�����X�t�H�[��----------

	//�}�b�v�̃g�����X�t�H�[��
	Transform Trans_Map;

	//�v���C���[�A�C�R���̃g�����X�t�H�[��
	Transform Trans_Player;

	//�G�A�C�R���̃g�����X�t�H�[��
	Transform Trans_Enemy;

	//----------�C���X�^���X----------
	Player* pPlayer_;
	Enemy* pEnemy_;

public:
	MiniMap(GameObject* parent);
	~MiniMap();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void SetCSV();
};

