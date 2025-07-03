#pragma once
#include "Engine/GameObject.h"
#include<vector>

class OutStageThing :
    public GameObject
{
private:
	//----------モデルハンドル----------

	//小屋モデル
	int hCabin_;

	//木モデル
	int hTree_;

	//丸太モデル
	int hLogs_;

	//----------モデル用トランスフォーム----------
	std::vector<Transform> CabinData_;

	std::vector<Transform> TreeData_;

	std::vector<Transform> LogsData_;

	std::vector<Transform> StrawData_;
public:
	OutStageThing(GameObject* parent);
	~OutStageThing();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	//----------セッター・ゲッター関数----------
	void SetCabinTransform(std::vector<Transform> _data) { CabinData_  =  _data; }

	void SetTreeTransform(std::vector<Transform> _data) { TreeData_  = _data; }

	void SetLogsTransform(std::vector<Transform> _data) { LogsData_ = _data; }

	void SetStrawTransform(std::vector<Transform> _data) { StrawData_ = _data; }
};

