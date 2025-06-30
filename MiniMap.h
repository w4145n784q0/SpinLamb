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

	//Player�̈ʒu
	XMFLOAT3 playerPos_;

	//Enemy�̈ʒu
	XMFLOAT3 enemyPos_;

public:
	MiniMap(GameObject* parent);
	~MiniMap();

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

	//CSV�t�@�C������K�v�p�����[�^��ǂݍ��݂���
	void SetMiniMapCSV();

	//�Z�b�^�[�E�Q�b�^�[�֐�
	XMFLOAT3 GetPlayerPos() { return playerPos_; }
	XMFLOAT3 GetEnemyPos() { return enemyPos_; }
};

