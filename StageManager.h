#pragma once
#include "Engine/GameObject.h"
#include"Ground.h"
#include"Fence.h"

//ステージ全体管理クラス
class StageManager :
    public GameObject
{
private:
	Transform GroundData_;//地面クラスに渡すデータ
	Transform WireData_;//柵クラス(鉄線)に渡すデータ
	Transform PillerData_;//柵クラス(柱)に渡すデータ

	float UpperEnd_;//ステージ北端
	float LowerEnd_; //ステージ南端
	float RightEnd_;//ステージ東端
	float LeftEnd_;//ステージ西端
	int PillerNum_;//柱の数

	XMFLOAT3 WirePosUpper_;//鉄線の当たり判定の位置（+z方向）
	XMFLOAT3 WirePosLower_;//鉄線の当たり判定の位置（-z方向）
	XMFLOAT3 WirePosRight_;//鉄線の当たり判定の位置（+x方向）
	XMFLOAT3 WirePosLeft_;//鉄線の当たり判定の位置（-x方向）

	XMFLOAT3 WireSizeUpper_;//鉄線の当たり判定の大きさ（+z方向）
	XMFLOAT3 WireSizeLower_;//鉄線の当たり判定の大きさ（-z方向）
	XMFLOAT3 WireSizeRight_;//鉄線の当たり判定の大きさ（+x方向）
	XMFLOAT3 WireSizeLeft_;//鉄線の当たり判定の大きさ（-x方向）

	XMFLOAT3 UpperNormal_;//ステージ北端の法線ベクトル
	XMFLOAT3 LowerNormal_;//ステージ南端の法線ベクトル
	XMFLOAT3 RightNormal_;//ステージ東端の法線ベクトル
	XMFLOAT3 LeftNormal_;//ステージ西端の法線ベクトル

public:
	StageManager(GameObject* parent);
	~StageManager();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void SetSCV();

	/// <summary>
	/// 地面クラスのトランスフォーム初期化
	/// </summary>
	void InitGroundData();

	/// <summary>
	/// 柵クラスのトランスフォームの初期化
	/// </summary>
	void InitFenceData();

	/// <summary>
	/// 柵クラスの柱の本数・位置・鉄線クラスの当たり判定を設定
	/// </summary>
	void InitEndData();

	float GetNorthEnd() { return UpperEnd_; }
	float GetSouthEnd() { return LowerEnd_; }
	float GetEastEnd() { return RightEnd_; }
	float GetWestEnd() { return LeftEnd_; }
};

