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

	//�i�s�������f��
	int hArrow_;

	//�C���X�^���X�֌W
	Player* pPlayer_;

	//�G(CPU)�X�e�[�g
	//�����̒l�ɉ����ĊeUpdate�֐����Ăяo��
	enum State {
		S_ROOT = 0,//���f�p
		S_CHASE,//�ǂ�������
		S_AIM,//�v���C���[��_��(�U������)
		S_ATTACK,//�U��
		S_HITSTOP,//�q�b�g�X�g�b�v
		S_HIT,//�e�����
		S_WALLHIT,//�ǂɃq�b�g
		S_STOP,//�G���~�߂�
		S_MAX
	};
	State EnemyState_;

	//----------�U���֌W----------
	
	//�_���Ă���U���܂ł̃^�C�}�[
	int AimTimer_;

	//----------�ړ��֌W----------

	//�v���C���[�̈ʒu��ۊ�
	XMVECTOR pPositionVec_;

	//�v���C���[�̈ʒu(���W)
	XMFLOAT3 PlayerPosition_;

	//�v���C���[�̑��x
	float PlayerAcceleration_;

	//----------�U���֌W----------

	//EnemyAttackTimeArray�̓Y����
	int RandomAim_; 

	///----------�_���[�W�֌W/----------

	//�q�b�g�X�g�b�v����
	int HitStopTimer_;


public:
	Enemy(GameObject* parent);
	~Enemy();

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//���
	void Release() override;

	//�����蔻�菈��
	void OnCollision(GameObject* pTarget) override;

	//----------EnemyState_�ɉ����ē��e���ς��Update�֐�----------

	//�������玟��Update�Ɉڂ锻�f������
	void UpdateRoot();

	//�ǐ�
	void UpdateChase();

	//�U������
	void UpdateAim();

	//�U��
	void UpdateAttack();

	//�q�b�g�X�g�b�v
	void UpdateHitStop();

	//�e���ꂽ���
	void UpdateHit();

	//�ǂɐڐG���
	void UpdateWallHit();
	
	//�G���~�߂�
	void UpdateStop();


	
	//�G�Ɉړ�������
	void EnemyStart() { EnemyState_ = S_ROOT; }

	//�G���~�߂�
	void EnemyStop() { EnemyState_ = S_STOP; }
	
	//�v���C���[�̕����ɉ�]����
	void LookPlayer();

	//�G�ƃv���C���[�̋�����Ԃ�
	float PlayerEnemyDistanceX();

	//Enemy����̃f�[�^��CSV�ǂݍ���
	void SetCSVEnemy();

};

