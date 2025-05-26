#pragma once
#include "Engine/GameObject.h"
#include"Player.h"
#include"Ground.h"
class Player;

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
		S_IDLE = 0,//�ҋ@
		S_ROOT,//���f�p
		S_CHASE,//�ǂ�������
		S_ATTACK,//�U��
		S_HITSTOP,//�q�b�g�X�g�b�v
		S_HIT,//�U����H�����
		S_WALLHIT,//�ǂɓ�����_���[�W
		S_AIM,//�v���C���[��_��(�U������)
		S_ONALEAT,//�l�q��

		S_MAX
	};
	State EnemyState_;

	//----------�U���֌W----------
	int AimTimer_;//�_���Ă���U���܂ł̃^�C�}�[
	//int randaim;

	//----------�ړ��֌W----------
	//float FrontLength_;//�G�̎��E�̒���
	//float Eye_;
	//bool IsHit_;
	XMVECTOR pPositionVec_;//�v���C���[�̈ʒu��ۊ�
	XMFLOAT3 PlayerPosition_;//�v���C���[�̈ʒu(���W)
	float PlayerAcceleration_;


	//�_���[�W�֌W
	int HitStopTimer_;//�q�b�g�X�g�b�v����

public:
	Enemy(GameObject* parent);
	~Enemy();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void UpdateIdle();//�ҋ@(�f�o�b�O�p)
	void UpdateRoot();
	//void UpdateThink();//�v�l��
	void UpdateChase();//�ǐ�
	void UpdateAttack();//�U��
	void UpdateHitStop();//�q�b�g�X�g�b�v
	void UpdateHit();//�U����������
	void UpdateWallHit();//�ǂɐڐG
	void UpdateAim();//�v���C���[��_���Ă���
	void UpdateOnAlert();//�l�q��
	//void UpdateRandomAim();

	void OnCollision(GameObject* pTarget) override;

	//�G�Ɉړ�������
	void EnemyStart() { EnemyState_ = S_ROOT; }

	//�G���~�߂�
	void EnemyStop() { EnemyState_ = S_MAX; }

	/// <summary>
	/// �v���C���[����͂�����鏈��
	/// </summary>
	/// <param name="_vector">�v���C���[����n���ꂽ�m�b�N�o�b�N�������(���K����)</param>
	/// <param name="_isDush">�v���C���[���_�b�V������</param>
	void PlayerReflect(XMVECTOR _vector, bool _isDush);
	
	//void SetState(State s) { EnemyState_ = s; }
	//State GetState() { return EnemyState_; }
	//bool GetStateAttack() { if (EnemyState_ == S_ATTACK) return true; else return false; }

	void SetStateStop() { EnemyState_ = S_MAX; }

	/// <summary>
	/// �v���C���[�̕����ɉ�]����
	/// </summary>
	void LookPlayer();

	//�G�ƃv���C���[�̋�����Ԃ�
	float PlayerEnemyDistanceX();

};

