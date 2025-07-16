#pragma once
#include "GameObject.h"

//-----------------------------------------------------------
//�J����
//-----------------------------------------------------------
namespace Camera
{
	//�������i�v���W�F�N�V�����s��쐬�j
	void Initialize();

	//�X�V�i�r���[�s��쐬�j
	void Update();

	//���_�i�J�����̈ʒu�j��ݒ�
	void SetPosition(XMFLOAT3 position);

	//�œ_�i����ʒu�j��ݒ�
	void SetTarget(XMFLOAT3 target);

	//�ʒu���擾
	XMFLOAT3 GetPosition();

	//�œ_���擾
	XMFLOAT3 GetTarget();

	//�r���[�s����擾
	XMMATRIX GetViewMatrix();

	//�v���W�F�N�V�����s����擾
	XMMATRIX GetProjectionMatrix();

	//�r���{�[�h�p��]�s����擾
	XMMATRIX GetBillboardMatrix();

	//�J�����U��(Y���W�̂�)
	float CameraShake();

	//�J�����U��(XY���W)
	XMFLOAT3 CameraShakeFloat3();

	//�U���J�n
	//_shaketime �U�������鎞��
	void CameraShakeStart(float _shaketime);

	//CSV����e�X�e�[�^�X��ǂݍ��݂���
	void SetCSVCamera();

	//void SetScreen(int split);

	void HalfScreen();

	//----------�Z�b�^�[�E�Q�b�^�[�֐�----------
	void SetShakeWidth(float _width);

	void SetShakeSpeed(float _speed);

	XMFLOAT3 GetInitPos();

	XMFLOAT3 GetInitTar();

	float GetShakeTimeShort(); 

	float GetShakeTimeMiddle();

	float GetShakeTimeLong();
};