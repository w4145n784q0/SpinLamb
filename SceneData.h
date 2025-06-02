#pragma once
#include "Engine/GameObject.h"

//,gameover,clearシーンで継承する
//各シーンの共通処理をまとめたクラス

class SceneData :
    public GameObject
{
protected:

	//結果画面のロゴの位置
	Transform Result;

	//結果画面の下のロゴの位置
	Transform PushTitle;

public:
	SceneData(GameObject* parent);
	SceneData(GameObject* parent, const std::string& name);
	virtual ~SceneData();

	//csv読み込み(必要なシーン初期化時に行う)
	void SetCSVScene();

};

