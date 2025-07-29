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

	//----------�C���X�^���X----------

	//�v���C���[�̃C���X�^���X(Enemy����͎�炸�A��ʃV�[������ݒ�)
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
		S_FENCEHIT,//��Ƀq�b�g
		S_STOP,//�G���~�߂�
		S_MAX
	};
	State EnemyState_;

	//----------�U���֌W----------
	
	//�_���Ă���U���܂ł̃^�C�}�[
	int AimTimer_;

	//----------�ړ��֌W----------

	//�v���C���[�̈ʒu�x�N�g����ۊ�
	XMVECTOR TargetVec_;

	//�v���C���[�̈ʒu(���W)
	XMFLOAT3 TargetPosition_;

	//�v���C���[�̑��x
	float TargetAcceleration_;

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

	//���I�ɌĂяo���X�V����
	void EnemyRun();

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
	void UpdateFenceHit();
	
	//�G���~�߂�
	void UpdateStop();

	//ImGui�̕`��
	void DrawImGui();
	
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

	//----------�Z�b�^�[�E�Q�b�^�[�֐�----------
	void SetPlayerPointer(Player* _player) { pPlayer_ = _player; }

	void SetTargetPosition(XMFLOAT3 _targetpos) { TargetPosition_ = _targetpos; }
	XMFLOAT3 GetTargetPosition() const {return TargetPosition_; }

	void SetTargetAcceleration(float _acceleration) { TargetAcceleration_ = TargetAcceleration_; }
	float GetTargetAcceleration() const { return TargetAcceleration_; }
};
