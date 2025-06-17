#pragma once
#include "Engine/GameObject.h"

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

	//選択中の処理
	virtual void UpdateSelect() = 0;

	//選択後の処理(主にシーン遷移までに時間を持たせる)
	virtual void UpdateDecide() = 0;
};

