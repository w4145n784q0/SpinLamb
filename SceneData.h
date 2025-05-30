#pragma once
#include "Engine/GameObject.h"
class SceneData :
    public GameObject
{
protected:
	XMFLOAT3 ResultPosition_ = { 0,0,0 }; //結果画面のロゴの位置
	XMFLOAT3 UnderPosition_ = { 0,0,0 }; //結果画面の下のロゴの位置
	int practiceEnd_ = 0; //練習モードの終了時のカウント

public:
	SceneData(GameObject* parent);
	SceneData(GameObject* parent, const std::string& name);
	virtual ~SceneData();

	void SetCSV();
	int GetPracticeEnd() { return practiceEnd_; }
	XMFLOAT3 GetResultPosition() { return ResultPosition_; }
	XMFLOAT3 GetUnderPosition() { return UnderPosition_; }
};

