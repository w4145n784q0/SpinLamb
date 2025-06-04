#pragma once
#include "Engine/GameObject.h"
#include"Player.h"

//�G�L�����N�^�[�̏������s���N���X
class Enemy :
    public Character
{
private:
	//----------���f���n���h��----------

	//�G���f��
    int hEnemy_;

	//�C���X�^���X�֌W
	Player* pPlayer_;

	//�G(CPU)�X�e�[�g
	enum State {
		S_ROOT = 0,//���f�p
		S_CHASE,//�ǂ�������
		S_AIM,//�v���C���[��_��(�U������)
		S_ATTACK,//�U��
		S_HITSTOP,//�q�b�g�X�g�b�v
		S_HIT,//�U����H�����
		S_WALLHIT,//�ǂɓ�����_���[�W
		S_STOP,//�G���~�߂�
		S_MAX
	};
	State EnemyState_;

	//----------�U���֌W----------
	int AimTimer_;//�_���Ă���U���܂ł̃^�C�}�[

	//----------�ړ��֌W----------
	XMVECTOR pPositionVec_;//�v���C���[�̈ʒu��ۊ�
	XMFLOAT3 PlayerPosition_;//�v���C���[�̈ʒu(���W)
	float PlayerAcceleration_;//�v���C���[�̑��x

	//----------�U���֌W----------
	int RandomAim_;//EnemyAttackTimeArray�̓Y���� 

	//�_���[�W�֌W
	int HitStopTimer_;//�q�b�g�X�g�b�v����


public:
	Enemy(GameObject* parent);
	~Enemy();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void UpdateRoot();//�������玟��Update�Ɉڂ锻�f������
	void UpdateChase();//�ǐ�
	void UpdateAim();//�U������
	void UpdateAttack();//�U��
	void UpdateHitStop();//�q�b�g�X�g�b�v
	void UpdateHit();//�U����������
	void UpdateWallHit();//�ǂɐڐG
	void UpdateStop();//�G���~�߂�

	void OnCollision(GameObject* pTarget) override;

	//�G�Ɉړ�������
	void EnemyStart() { EnemyState_ = S_ROOT; }

	//�G���~�߂�
	void EnemyStop() { EnemyState_ = S_STOP; }
	
	//�v���C���[�̕����ɉ�]����
	void LookPlayer();

	//�G�ƃv���C���[�̋�����Ԃ�
	float PlayerEnemyDistanceX();

	/// <summary>
	/// Enemy����̃f�[�^��CSV�ǂݍ���
	/// </summary>
	void SetCSVEnemy();

};

