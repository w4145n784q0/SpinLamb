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
	//csv読み込み時のインデックス
	enum CameraIndex
	{
		i_shakeSpeed = 0,	//振動スピード
		i_shakeWidth,		//振動幅
		i_frame,			//1フレーム
		i_initPositionX,	//初期位置X
		i_initPositionY,	//初期位置Y
		i_initPositionZ,	//初期位置Z
		i_initTargetX,		//初期焦点X
		i_initTargetY,		//初期焦点Y
		i_initTargetZ,		//初期焦点Z
		i_shakeTimeShort,	//振動時間(短)
		i_shakeTimeMiddle,	//振動時間(中)
		i_shakeTimeLong,	//振動時間(長)
	};

	bool IsCameraShake;//カメラが振動してるか
	float ShakeTimer;//カメラの振動カウント
	float ShakeSpeed = 0.0f;//振動スピード
	float ShakeWidth = 0.0f;//振動幅
	float frame = 0.0f;//1フレーム
	XMFLOAT3 InitPosition = { 0,0,0 };//初期位置
	XMFLOAT3 InitTarget = { 0,0,0 };//初期焦点
	float ShakeTimeShort = 0.0f;//振動時間(短)
	float ShakeTimeMiddle = 0.0f;//振動時間(中)
	float ShakeTimeLong = 0.0f;//振動時間(長)
}

//初期化（プロジェクション行列作成）
void Camera::Initialize()
{
	SetCSVCamera();

	_position = InitPosition;	//カメラの位置
	_target = InitTarget;	//カメラの焦点

	//プロジェクション行列
	_proj = XMMatrixPerspectiveFovLH(XM_PIDIV4, (FLOAT)Direct3D::screenWidth_ / (FLOAT)Direct3D::screenHeight_, 0.1f, 1000.0f);
}

//更新（ビュー行列作成）
void Camera::Update()
{
	//ビュー行列
	_view = XMMatrixLookAtLH(XMVectorSet(_position.x, _position.y, _position.z, 0),
		XMVectorSet(_target.x, _target.y, _target.z, 0), XMVectorSet(0, 1, 0, 0));


	//ビルボード行列
	//（常にカメラの方を向くように回転させる行列。パーティクルでしか使わない）
	//http://marupeke296.com/DXG_No11_ComeOnBillboard.html
	_billBoard = XMMatrixLookAtLH(XMVectorSet(0, 0, 0, 0), XMLoadFloat3(&_target)- XMLoadFloat3(&_position), XMVectorSet(0, 1, 0, 0));
	_billBoard = XMMatrixInverse(nullptr, _billBoard);
}

//焦点を設定
void Camera::SetTarget(XMFLOAT3 target) { _target = target; }

//位置を設定
void Camera::SetPosition(XMFLOAT3 position) { _position = position; }

//焦点を取得
XMFLOAT3 Camera::GetTarget() { return _target; }

//位置を取得
XMFLOAT3 Camera::GetPosition() { return _position; }

//ビュー行列を取得
XMMATRIX Camera::GetViewMatrix() { return _view; }

//プロジェクション行列を取得
XMMATRIX Camera::GetProjectionMatrix() { return _proj; }

//ビルボード用回転行列を取得
XMMATRIX Camera::GetBillboardMatrix(){	return _billBoard; }

//カメラ振動(Y座標のみ)
float Camera::CameraShake()
{
	//カメラの振動処理(振動中でなくともこの処理は通る)
	//カメラ振動中でないなら0を返す
	float cameraY = 0.0f;


	if (IsCameraShake)
	{
		ShakeTimer -= frame;
		if (ShakeTimer <= 0.0f)
		{
			IsCameraShake = false;
			ShakeTimer = 0.0f;
		}
		else
		{
			//始めは大きく、徐々に収まる揺れを計算
			cameraY = sinf(ShakeTimer * ShakeSpeed)* ShakeTimer * ShakeWidth;
		}
	}
	return cameraY;
}

//カメラ振動(XY座標)
XMFLOAT3 Camera::CameraShakeFloat3()
{
	//カメラの振動処理(振動中でなくともこの処理は通る)
	//カメラ振動中でないなら0,0,0を返す
	XMFLOAT3 camera = {0,0,0};

	if (IsCameraShake)
	{
		ShakeTimer -= frame;
		if (ShakeTimer <= 0.0f)
		{
			IsCameraShake = false;
			ShakeTimer = 0.0f;
		}
		else
		{
			//始めは大きく、徐々に収まる揺れを計算
			camera.x = sinf(ShakeTimer * ShakeSpeed) * ShakeTimer * ShakeWidth;
			camera.y = sinf(ShakeTimer * ShakeSpeed) * ShakeTimer * ShakeWidth;
		}
	}
	return camera;
}

//振動開始
//_shaketime 振動させる時間
void Camera::CameraShakeStart(float _shaketime)
{
	IsCameraShake = true;
	ShakeTimer = _shaketime;

}

void Camera::SetCSVCamera()
{
	//csvファイルを読み込む
	CsvReader csv;
	csv.Load("CSVdata\\EngineData\\CameraData.csv");

	//csvファイルの各0列目の文字列を取得
	std::string cameraName = "Camera";

	//0列目の文字列を渡し、その行のパラメータを取得
	std::vector<float> cameraData = GameObject::GetCSVReadData(csv, cameraName);
		
	//初期化の順番はcsvの各行の順番に合わせる
	//vの添え字はnamespaceで宣言した列挙型を使用
	ShakeSpeed = cameraData[i_shakeSpeed];
	ShakeWidth = cameraData[i_shakeWidth];
	frame = cameraData[i_frame];
	InitPosition = { cameraData[i_initPositionX],  cameraData[i_initPositionY],  cameraData[i_initPositionZ] };
	InitTarget = { cameraData[i_initTargetX],  cameraData[i_initTargetY],  cameraData[i_initTargetZ] };
	ShakeTimeShort = cameraData[i_shakeTimeShort];
	ShakeTimeMiddle = cameraData[i_shakeTimeMiddle]; 
	ShakeTimeLong = cameraData[i_shakeTimeLong];
}

void Camera::HalfScreen()
{
	//画面左右分割する際のプロジェクション行列作成
	//第二引数:(画面幅 / 2) / 画面高さ(画面を左右に分けるので、本来の横幅を2で割り、それを縦幅で割る) 
	_proj = XMMatrixPerspectiveFovLH(XM_PIDIV4, ( (FLOAT)Direct3D::screenWidth_ / 2.0f ) / (FLOAT)Direct3D::screenHeight_,  0.1f, 1000.0f);

}

void Camera::FullScreen()
{
	//全体描画のプロジェクション行列に戻す
	_proj = XMMatrixPerspectiveFovLH(XM_PIDIV4, (FLOAT)Direct3D::screenWidth_ / (FLOAT)Direct3D::screenHeight_, 0.1f, 1000.0f);
}

//カメラの振動幅をセット(必要に応じてセット)
void Camera::SetShakeWidth(float _width)
{
	ShakeWidth = _width;
}

//カメラの振動速度をセット(必要に応じてセット)
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