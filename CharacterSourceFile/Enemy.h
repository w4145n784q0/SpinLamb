#pragma once
#include "../Engine/GameObject.h"
#include"Player.h"

//�G(CPU)�L�����N�^�[�̏������s���N���X
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

	//----------��ԑJ��----------

	//�G(CPU)�̏�ԑJ��
	//�����̒l�ɉ����ĊeUpdate�֐����Ăяo��
	enum EnemyState {
		S_Root = 0,		//���f�p
		S_Chase,		//�ǂ�������
		S_Aim,			//�v���C���[��_��(�U������)
		S_Attack,		//�U��
		S_HitStop,		//�q�b�g�X�g�b�v
		S_Hit,			//�e�����
		S_FenceHit,		//��Ƀq�b�g
		S_Stop,			//�G���~�߂�
		S_MaxState
	};
	EnemyState EnemyState_;

	//----------�U���֌W----------
	
	//�_���Ă���U���܂ł̌v���^�C�}�[
	int AimTimer_;

	//EnemyAttackTimeArray�̓Y����
	int RandomAim_;

	//----------�ړ��֌W----------

	//�v���C���[�̈ʒu�x�N�g����ۊ�
	XMVECTOR TargetVec_;

	//�v���C���[�̈ʒu(���W)
	XMFLOAT3 TargetPosition_;

	//�v���C���[�̑��x
	float TargetAcceleration_;

	//----------�_���[�W�֌W----------

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

	//��ɐڐG�������
	void UpdateFenceHit();
	
	//�G���~�߂�
	void UpdateStop();
	
	//----------Enemy�����֐�----------

	//ImGui�̕`��
	void DrawImGui();

	//�G�Ɉړ�������
	void EnemyStart() { EnemyState_ = S_Root; }

	//�G���~�߂�
	void EnemyStop() { EnemyState_ = S_Stop; }
	
	//�v���C���[�̕����ɉ�]����
	void LookPlayer();

	/// <summary>
	/// ���g��Y����]�ʂ��w�肳�ꂽ�����ɍ��킹�čX�V���A�O�������悤�ɂ���
	/// </summary>
	/// <param name="_direction">��]�ʂ��v�Z����ۂ̃x�N�g��</param>
	void RotateFromDirection(XMVECTOR _direction);

	//�G�ƃv���C���[�̋�����Ԃ�
	float PlayerEnemyDistanceX();

	//Enemy����̃f�[�^��CSV�ǂݍ���
	void SetCSVEnemy();

	//----------�Z�b�^�[�E�Q�b�^�[�֐�----------
	void SetPlayerPointer(Player* _player) { pPlayer_ = _player; }

	void SetTargetPosition(XMFLOAT3 _targetpos) { TargetPosition_ = _targetpos; }
	XMFLOAT3 GetTargetPosition() const {return TargetPosition_; }

	void SetTargetAcceleration(float _acceleration) { TargetAcceleration_ = _acceleration; }
	float GetTargetAcceleration() const { return TargetAcceleration_; }
};
