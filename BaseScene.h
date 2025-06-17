#pragma once
#include "Engine/GameObject.h"

//Title,GameMode,ResultSceneの共通処理をまとめたクラス
class BaseScene :
    public GameObject
{
protected:

	//選択中か選択後か
	enum Decide
	{
		Selected,
		Decided,
		MaxSelect
	};
	Decide ModeDecide_;

	//シーン遷移までの時間を計る
	int SceneTransitionTimer_;
public:
	BaseScene(GameObject* parent);
	BaseScene(GameObject* parent, const std::string& name);
	virtual ~BaseScene();

	void Update() override;

	//以下の処理は純粋仮想関数のため、継承先で必ずオーバーライド

	//選択中の処理（プレイ中以外に使用）
	virtual void UpdateSelect() = 0;

	//選択後の処理（プレイ中以外に使用）主にシーン遷移までに時間を持たせる
	virtual void UpdateDecide() = 0;
};

