#pragma once
#include "Engine/GameObject.h"
#include"Character.h"

class Player :
    public Character
{
private:
	//----------���f���n���h��----------

	//�v���C���[���f��
	int hPlayer_;

	//�e�̃��f��
	int hShadow_;

	//----------�T�E���h�n���h��----------

	//�ڐG��
	int hCollisionSound_;

	//----------�v���C���[�X�e�[�g----------
	enum State
	{
		S_IDLE,//�ʏ�
		S_HIT,//�e�����
		S_CHARGE,//�`���[�W��
		S_ATTACK,//�U��
		S_WALLHIT,//�ǂɃq�b�g
		S_STOP,//�v���C���[���~�߂���
		S_MAX
	};
	State PlayerState_;

	//----------�J�����X�e�[�g----------
	enum CameraState
	{
		S_NORMALCAMERA,//�ʏ�J����
		S_DEBUGCAMERA,//�f�o�b�O�p�J����
		S_MAXCAMERA,
	};
	CameraState CameraState_;
	
	//----------�ړ��֌W----------
	XMFLOAT3 Direction_;//�v���C���[�̑������(xz�ǂ��ɐi�ނ�)
	Transform ArrowTransform_;//��󃂃f���̈ʒu���
	
	//----------�J�����֌W----------
	XMFLOAT3 CameraPosition_;//�J�����ʒu
	XMFLOAT3 CameraTarget_;//�J���������_
	Transform cameraTransform_;//�J������Transform ��]�����g��
	XMVECTOR BackCamera_;//�v���C���[�̌��ɒu���J�����̈ʒu


public:

	Player(GameObject* parent);
	~Player();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void OnCollision(GameObject* pTarget) override;

	void UpdateIdle();//�ʏ���
	void UpdateHit();//�q�b�g���
	void UpdateCharge();//�`���[�W���
	void UpdateAttack();//�U�����
	void UpdateWallHit();//�ǂɐڐG���
	void UpdateStop();//�v���C���[���~�߂���

	//�v���C���[�Ɉړ�������
	void PlayerStart() { PlayerState_ = S_IDLE; }

	//�v���C���[���~�߂�
	void PlayerStop() { PlayerState_ = S_MAX; }
	
	//�W�����v�J�n
	void SetJump();

	//�J�����̑���i�ꕔ�̃X�e�[�g�̂݁j
	void CameraControl();

	//�J�����̐ݒ�(���t���[���Ăяo��)
	void CameraUpdate();

	/// <summary>
	/// �L�[�{�[�h�ړ�����
	/// </summary>
	void KeyBoradMove();

	/// <summary>
	/// Player����̃f�[�^��CSV�ǂݍ���
	/// </summary>
	void SetCSVPlayer();
};

