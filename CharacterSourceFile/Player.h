#pragma once
#include "../Engine/GameObject.h"
#include"Character.h"

//�v���C���[�L�����N�^�[�̏������s���N���X
class Player :
    public Character
{
private:
	//----------���f���n���h��----------

	//�v���C���[���f��
	int hPlayer_;

	//----------��ԑJ��----------
	
	//�v���C���[�̏�ԑJ��
	//�����̒l�ɉ����ĊeUpdate�֐����Ăяo��
	enum State
	{
		S_Idle,		//�ʏ�
		S_Charge,	//�`���[�W��
		S_Attack,	//�U��
		S_Hit,		//�e�����
		S_FenceHit,	//��Ƀq�b�g
		S_Stop,		//�v���C���[���~�߂���
		S_MaxState,
	};
	State PlayerState_;

	//�J�����̏��
	//�����̒l�ɉ����ăJ�����̈ʒu�E��]�ʂ�ω�������
	enum CameraState
	{
		S_NormalCamera,		//�ʏ�J����
		S_DebugCamera,		//�f�o�b�O�p�J����
		S_MaxCamera,
	};
	CameraState CameraState_;
	
	//----------������----------

	//�R���g���[���[�g�p����ID(�L�����N�^�[�N���X��ID�Ƃ͕�)
	int ControllerID_;

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

	//���I�ɌĂяo���X�V����
	void PlayerRun();

	//----------PlayerState_�ɉ����ē��e���ς��Update�֐�----------

	//�ʏ���
	void UpdateIdle();

	//�`���[�W���
	void UpdateCharge();

	//�U�����
	void UpdateAttack();

	//�e���ꂽ���
	void UpdateHit();

	//��ɐڐG�������
	void UpdateFenceHit();

	//�v���C���[���~�߂���
	void UpdateStop();

	//----------Player�����֐�----------

	//ImGui�̕`��
	void DrawImGui();

	//�v���C���[�Ɉړ�������
	void PlayerStart() { PlayerState_ = S_Idle; }

	//�v���C���[���~�߂�
	void PlayerStop() { PlayerState_ = S_Stop; }
	
	/// <summary>
	/// �v���C���[�̏�����(�ʊ֐�����Ăԗp)
	/// </summary>
	/// <param name="_CSVpath">�ǂݍ���csv�t�@�C���̃p�X</param>
	/// <param name="_Modelpath">�ǂݍ���fbx�t�@�C���̃p�X</param>
	void PlayerInit(std::string _CSVpath, std::string _Modelpath);

	//�J�����̑���i�ꕔ�̃X�e�[�g�̂݁j
	void CameraControl();

	//�J�����̐ݒ�(���t���[���Ăяo��)
	void CameraUpdate();

	//�L�[�{�[�h�ړ�����
	void KeyBoardMove();

	/// <summary>
	/// �R���g���[���ړ�����
	/// </summary>
	/// <param name="_PadID">�g�p���Ă���Q�[���p�b�h��ID(�ڑ���)</param>
	void ControllerMove(int _PadID);

	/// <summary>
	/// �v���C���[��p�ړ�����
	/// </summary>
	/// <param name="_move">�ړ��������</param>
	void PlayerMove(XMVECTOR _move);

	/// <summary>
	/// �󂯎�������O�ɉ����ėv�f���󂯎��A���ˏ���������
	/// </summary>
	/// <param name="_name">�ڐG�����I�u�W�F�N�g��</param>
	void CollisionCharacter(std::string _name);

	/// <summary>
	/// Player����̃f�[�^��CSV�ǂݍ���
	/// </summary>
	/// <param name="_path">�ǂݍ���csv�t�@�C���̃p�X</param>
	void SetCSVPlayer(std::string _path);

	//----------�Z�b�^�[�E�Q�b�^�[�֐�----------
	void SetCameraPosition(XMFLOAT3 _pos) { CameraPosition_ = _pos; }
	XMFLOAT3 GetCameraPosition() const { return CameraPosition_; }

	void SetCameraTarget(XMFLOAT3 _tar) { CameraTarget_ = _tar; }
	XMFLOAT3 GetCameraTarget() const { return CameraTarget_; }

	void SetControllerID(int _controllerID) { ControllerID_ = _controllerID; }
	int GetControllerID() const { return ControllerID_; }
};

