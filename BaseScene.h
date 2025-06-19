#pragma once
#include "Engine/GameObject.h"

//Title,GameMode,ResultSceneの共通処理をまとめたクラス
class BaseScene :
    public GameObject
{
protected:

	//ゲームシーン全般の状態
	enum SceneState
	{
		S_Active = 0,
		S_Passive,
		S_Transition,
		MaxDecide
	};
	SceneState SceneState_;

	//シーン遷移までの時間を計る
	int SceneTransitionTimer_;
public:
	BaseScene(GameObject* parent);
	BaseScene(GameObject* parent, const std::string& name);
	virtual ~BaseScene();

	void Update() override;

	//以下の処理は純粋仮想関数のため、継承先で必ずオーバーライド

	//通常の処理
	virtual void UpdateActive() = 0;

	//停止中の処理
	virtual void UpdatePassive() = 0;

	//シーン遷移中の処理
	virtual void UpdateTransition() = 0;
};

