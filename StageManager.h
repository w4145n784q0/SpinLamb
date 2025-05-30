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

	float UpperEnd_;//ステージ北端
	float LowerEnd_; //ステージ南端
	float RightEnd_;//ステージ東端
	float LeftEnd_;//ステージ西端
	int PillerNum_;//柱の数

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

	float GetNorthEnd() { return UpperEnd_; }
	float GetSouthEnd() { return LowerEnd_; }
	float GetEastEnd() { return RightEnd_; }
	float GetWestEnd() { return LeftEnd_; }
};

