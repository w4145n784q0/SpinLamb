#pragma once
#include "GameObject.h"

//-----------------------------------------------------------
//カメラ
//-----------------------------------------------------------
namespace Camera
{
	//初期化（プロジェクション行列作成）
	void Initialize();

	//更新（ビュー行列作成）
	void Update();

	//視点（カメラの位置）を設定
	void SetPosition(XMFLOAT3 position);

	//焦点（見る位置）を設定
	void SetTarget(XMFLOAT3 target);

	//位置を取得
	XMFLOAT3 GetPosition();

	//焦点を取得
	XMFLOAT3 GetTarget();

	//ビュー行列を取得
	XMMATRIX GetViewMatrix();

	//プロジェクション行列を取得
	XMMATRIX GetProjectionMatrix();

	//ビルボード用回転行列を取得
	XMMATRIX GetBillboardMatrix();

	//カメラ振動(Y座標のみ)
	float CameraShake();

	//カメラ振動(XY座標)
	XMFLOAT3 CameraShakeFloat3();

	//振動開始
	//_shaketime 振動させる時間
	void CameraShakeStart(float _shaketime);

	//CSVから各ステータスを読み込みする
	void SetCSVCamera();

	//void SetScreen(int split);

	void HalfScreen();

	//----------セッター・ゲッター関数----------
	void SetShakeWidth(float _width);

	void SetShakeSpeed(float _speed);

	XMFLOAT3 GetInitPos();

	XMFLOAT3 GetInitTar();

	float GetShakeTimeShort(); 

	float GetShakeTimeMiddle();

	float GetShakeTimeLong();
};