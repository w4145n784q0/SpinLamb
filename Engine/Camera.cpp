#include "camera.h"
#include "Direct3D.h"
#include"../Engine/CsvReader.h"

#include"GameObject.h"

XMFLOAT3 _position;
XMFLOAT3 _target;
XMMATRIX _view;
XMMATRIX _proj;
XMMATRIX _billBoard;

namespace
{
	//csv�ǂݍ��ݎ��̃C���f�b�N�X(�J�����̊e�ϐ�)
	enum CameraIndex
	{
		i_NearZ = 0,		//�j�A����
		i_FarZ,				//�t�@�[����
		i_ShakeSpeed,		//�U���X�s�[�h
		i_ShakeWidth,		//�U����
		i_Frame,			//1�t���[��
		i_InitPositionX,	//�����ʒuX
		i_InitPositionY,	//�����ʒuY
		i_InitPositionZ,	//�����ʒuZ
		i_InitTargetX,		//�����œ_X
		i_InitTargetY,		//�����œ_Y
		i_InitTargetZ,		//�����œ_Z
		i_ShakeTimeShort,	//�U������(�Z)
		i_ShakeTimeMiddle,	//�U������(��)
		i_ShakeTimeLong,	//�U������(��)
	};

	float NearZ = 0.0f; //�N���b�s���O��(�j�A���� �J�����ɍł��߂�)
	float FarZ = 0.0f;//�N���b�s���O��(�t�@�[���� �J��������ł�����)
	bool IsCameraShake;//�J�������U�����Ă邩
	float ShakeTimer;//�J�����̐U���J�E���g
	float ShakeSpeed = 0.0f;//�U���X�s�[�h
	float ShakeWidth = 0.0f;//�U����
	float Frame = 0.0f;//1�t���[��
	XMFLOAT3 InitPosition = { 0,0,0 };//�����ʒu
	XMFLOAT3 InitTarget = { 0,0,0 };//�����œ_
	float ShakeTimeShort = 0.0f;//�U������(�Z)
	float ShakeTimeMiddle = 0.0f;//�U������(��)
	float ShakeTimeLong = 0.0f;//�U������(��)
}

//�������i�v���W�F�N�V�����s��쐬�j
void Camera::Initialize()
{
	//csv�t�@�C����ǂݍ���
	SetCSVCamera();

	_position = InitPosition;	//�J�����̈ʒu
	_target = InitTarget;	//�J�����̏œ_

	//�v���W�F�N�V�����s��
	_proj = XMMatrixPerspectiveFovLH(XM_PIDIV4, (FLOAT)Direct3D::screenWidth_ / (FLOAT)Direct3D::screenHeight_, NearZ, FarZ);
}

//�X�V�i�r���[�s��쐬�j
void Camera::Update()
{
	//�r���[�s��
	_view = XMMatrixLookAtLH(XMVectorSet(_position.x, _position.y, _position.z, 0),
		XMVectorSet(_target.x, _target.y, _target.z, 0), XMVectorSet(0, 1, 0, 0));


	//�r���{�[�h�s��
	//�i��ɃJ�����̕��������悤�ɉ�]������s��B�p�[�e�B�N���ł����g��Ȃ��j
	//http://marupeke296.com/DXG_No11_ComeOnBillboard.html
	_billBoard = XMMatrixLookAtLH(XMVectorSet(0, 0, 0, 0), XMLoadFloat3(&_target)- XMLoadFloat3(&_position), XMVectorSet(0, 1, 0, 0));
	_billBoard = XMMatrixInverse(nullptr, _billBoard);
}

//�œ_��ݒ�
void Camera::SetTarget(XMFLOAT3 target) { _target = target; }

//�ʒu��ݒ�
void Camera::SetPosition(XMFLOAT3 position) { _position = position; }

//�œ_���擾
XMFLOAT3 Camera::GetTarget() { return _target; }

//�ʒu���擾
XMFLOAT3 Camera::GetPosition() { return _position; }

//�r���[�s����擾
XMMATRIX Camera::GetViewMatrix() { return _view; }

//�v���W�F�N�V�����s����擾
XMMATRIX Camera::GetProjectionMatrix() { return _proj; }

//�r���{�[�h�p��]�s����擾
XMMATRIX Camera::GetBillboardMatrix(){	return _billBoard; }

//�J�����U��(Y���W�̂�)
float Camera::CameraShake()
{
	//�J�����̐U������(�U�����łȂ��Ƃ����̏����͒ʂ�)
	//�J�����U�����łȂ��Ȃ�0��Ԃ�
	float cameraY = 0.0f;


	if (IsCameraShake)
	{
		ShakeTimer -= Frame;
		if (ShakeTimer <= 0.0f)
		{
			IsCameraShake = false;
			ShakeTimer = 0.0f;
		}
		else
		{
			//�n�߂͑傫���A���X�Ɏ��܂�h����v�Z
			cameraY = sinf(ShakeTimer * ShakeSpeed)* ShakeTimer * ShakeWidth;
		}
	}
	return cameraY;
}

//�J�����U��(XY���W)
XMFLOAT3 Camera::CameraShakeFloat3()
{
	//�J�����̐U������(�U�����łȂ��Ƃ����̏����͒ʂ�)
	//�J�����U�����łȂ��Ȃ�0,0,0��Ԃ�
	XMFLOAT3 camera = {0,0,0};

	if (IsCameraShake)
	{
		ShakeTimer -= Frame;
		if (ShakeTimer <= 0.0f)
		{
			IsCameraShake = false;
			ShakeTimer = 0.0f;
		}
		else
		{
			//�n�߂͑傫���A���X�Ɏ��܂�h����v�Z
			camera.x = sinf(ShakeTimer * ShakeSpeed) * ShakeTimer * ShakeWidth;
			camera.y = sinf(ShakeTimer * ShakeSpeed) * ShakeTimer * ShakeWidth;
		}
	}
	return camera;
}

//�U���J�n
//_shaketime �U�������鎞��
void Camera::CameraShakeStart(float _shaketime)
{
	IsCameraShake = true;
	ShakeTimer = _shaketime;

}

void Camera::SetCSVCamera()
{
	//csv�t�@�C����ǂݍ���
	CsvReader csv;
	csv.Load("CSVdata\\EngineData\\CameraData.csv");

	//csv�t�@�C���̊e0��ڂ̕�������擾
	std::string CameraName = "Camera";

	//0��ڂ̕������n���A���̍s�̃p�����[�^���擾
	std::vector<float> CameraData = GameObject::GetCSVReadData(csv, CameraName);
		
	//�������̏��Ԃ�csv�̊e�s�̏��Ԃɍ��킹��
	//v�̓Y������namespace�Ő錾�����񋓌^���g�p
	NearZ = CameraData[i_NearZ];
	FarZ = CameraData[i_FarZ];
	ShakeSpeed = CameraData[i_ShakeSpeed];
	ShakeWidth = CameraData[i_ShakeWidth];
	Frame = CameraData[i_Frame];
	InitPosition = { CameraData[i_InitPositionX],  CameraData[i_InitPositionY],  CameraData[i_InitPositionZ] };
	InitTarget = { CameraData[i_InitTargetX],  CameraData[i_InitTargetY],  CameraData[i_InitTargetZ] };
	ShakeTimeShort = CameraData[i_ShakeTimeShort];
	ShakeTimeMiddle = CameraData[i_ShakeTimeMiddle]; 
	ShakeTimeLong = CameraData[i_ShakeTimeLong];
}

void Camera::HalfScreen()
{
	//��ʍ��E��������ۂ̃v���W�F�N�V�����s��쐬
	//������:(��ʕ� / 2) / ��ʍ���(��ʂ����E�ɕ�����̂ŁA�{���̉�����2�Ŋ���A������c���Ŋ���) 
	_proj = XMMatrixPerspectiveFovLH(XM_PIDIV4, ((FLOAT)Direct3D::screenWidth_ / 2.0f) / (FLOAT)Direct3D::screenHeight_, NearZ, FarZ);

}

void Camera::FullScreen()
{
	//�S�̕`��̃v���W�F�N�V�����s��ɖ߂�
	_proj = XMMatrixPerspectiveFovLH(XM_PIDIV4, (FLOAT)Direct3D::screenWidth_ / (FLOAT)Direct3D::screenHeight_, NearZ, FarZ);
}

//�J�����̐U�������Z�b�g(�K�v�ɉ����ăZ�b�g)
void Camera::SetShakeWidth(float _width)
{
	ShakeWidth = _width;
}

//�J�����̐U�����x���Z�b�g(�K�v�ɉ����ăZ�b�g)
void Camera::SetShakeSpeed(float _speed)
{
	ShakeSpeed = _speed;
}

XMFLOAT3 Camera::GetInitPos()
{
	return InitPosition;
}

XMFLOAT3 Camera::GetInitTar()
{
	return InitTarget;
}

float Camera::GetShakeTimeShort()
{
	return ShakeTimeShort;
}

float Camera::GetShakeTimeMiddle()
{
	return ShakeTimeMiddle;
}

float Camera::GetShakeTimeLong()
{
	return ShakeTimeLong;
}