#pragma once
#include "Engine/GameObject.h"

//各シーンの基底クラス
//シーンごとの共通処理をまとめる
class BaseScene :
    public GameObject
{
protected:

	//ゲームシーン全般の状態
	enum SceneState
	{
		S_Active = 0,//通常
		S_Transition,//シーン遷移中
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

	//シーン遷移中の処理(シーン遷移中の演出などに使用)
	virtual void UpdateTransition() = 0;
};

