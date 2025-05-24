#pragma once
#include "Engine/GameObject.h"
#include"Ground.h"
#include"Fence.h"

class StageManager :
    public GameObject
{
private:
	Transform GroundData_;//地面クラスに渡すデータ
	Transform WireData_;//柵クラス(鉄線)に渡すデータ
	Transform PillerData_;//柵クラス(柱)に渡すデータ

	float NorthEnd_;//ステージ北端
	float SouthEnd_; //ステージ南端
	float EastEnd_;//ステージ東端
	float WestEnd_;//ステージ西端

public:
	StageManager(GameObject* parent);
	~StageManager();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void SetSCV();

	void InitGroundData();
	void InitFenceData();
	void InitEndData();

	float GetNorthEnd() { return NorthEnd_; }
	float GetSouthEnd() { return SouthEnd_; }
	float GetEastEnd() { return EastEnd_; }
	float GetWestEnd() { return WestEnd_; }
};

