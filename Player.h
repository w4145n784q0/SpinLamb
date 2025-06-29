#pragma once
#include "Engine/GameObject.h"
#include"Character.h"

//�v���C���[�L�����N�^�[�̏������s���N���X
class Player :
    public Character
{
private:
	//----------���f���n���h��----------

	//�v���C���[���f��
	int hPlayer_;

	//�i�s�������f��
	int hArrow_;

	//----------�v���C���[�X�e�[�g----------
	//�����̒l�ɉ����ĊeUpdate�֐����Ăяo��
	enum State
	{
		S_IDLE,//�ʏ�
		S_CHARGE,//�`���[�W��
		S_ATTACK,//�U��
		S_HIT,//�e�����
		S_WALLHIT,//�ǂɃq�b�g
		S_STOP,//�v���C���[���~�߂���
		S_MAX
	};
	State PlayerState_;

	//----------�J�����X�e�[�g----------
	//�����̒l�ɉ����ăJ�����̈ʒu�E��]�ʂ�ω�������
	enum CameraState
	{
		S_NORMALCAMERA,//�ʏ�J����
		S_DEBUGCAMERA,//�f�o�b�O�p�J����
		S_MAXCAMERA,
	};
	CameraState CameraState_;
	
	//----------�ړ��֌W----------

	//�L�[�{�[�h�g�p���A�v���C���[�̑������(xz�ǂ��ɐi�ނ�)
	XMFLOAT3 Direction_;
	
	//----------�J�����֌W----------

	//�J�����ʒu
	XMFLOAT3 CameraPosition_;

	//�J���������_
	XMFLOAT3 CameraTarget_;

	//�J������Transform ��]�����g��
	Transform cameraTransform_;

	//�v���C���[�̌��ɒu���J�����̈ʒu
	XMVECTOR BackCamera_;

public:
	Player(GameObject* parent);
	~Player();

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

	//�����蔻�菈��
	void OnCollision(GameObject* pTarget) override;

	//----------PlayerState_�ɉ����ē��e���ς��Update�֐�----------

	//�ʏ���
	void UpdateIdle();

	//�`���[�W���
	void UpdateCharge();

	//�U�����
	void UpdateAttack();

	//�e���ꂽ���
	void UpdateHit();

	//�ǂɐڐG���
	void UpdateWallHit();

	//�v���C���[���~�߂���
	void UpdateStop();



	//�v���C���[�Ɉړ�������
	void PlayerStart() { PlayerState_ = S_IDLE; }

	//�v���C���[���~�߂�
	void PlayerStop() { PlayerState_ = S_STOP; }
	
	//�W�����v�J�n
	void SetJump();

	//�J�����̑���i�ꕔ�̃X�e�[�g�̂݁j
	void CameraControl();

	//�J�����̐ݒ�(���t���[���Ăяo��)
	void CameraUpdate();

	/// �L�[�{�[�h�ړ�����
	void KeyBoradMove();

	/// Player����̃f�[�^��CSV�ǂݍ���
	void SetCSVPlayer();
};

