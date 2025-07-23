#pragma once
#include "Engine/GameObject.h"
#include<vector>

//ステージ外オブジェクトを管理するクラス

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

	//藁束モデル
	int hStraw_;

	//各モデルハンドル配列
	std::vector<int> ModelHandleArray_;

	//----------モデル用トランスフォーム----------

	//小屋のトランスフォーム配列
	std::vector<Transform> CabinData_;

	//木のトランスフォーム配列
	std::vector<Transform> TreeData_;

	//丸太のトランスフォーム配列
	std::vector<Transform> LogsData_;

	//藁束のトランスフォーム配列
	std::vector<Transform> StrawData_;

	//各モデル用トランスフォーム配列
	std::vector<std::vector<Transform>*> StageTransArray_;
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

	//ImGuiの描画
	void DrawImGui();

	//----------セッター・ゲッター関数----------
	void SetCabinTransform(std::vector<Transform> _data) { CabinData_  =  _data; }

	void SetTreeTransform(std::vector<Transform> _data) { TreeData_  = _data; }

	void SetLogsTransform(std::vector<Transform> _data) { LogsData_ = _data; }

	void SetStrawTransform(std::vector<Transform> _data) { StrawData_ = _data; }
};

