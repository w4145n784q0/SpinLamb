#pragma once
#include "Engine/GameObject.h"

class BaseScene :
    public GameObject
{
protected:

	enum Decide
	{
		Selected,
		Decided,
		MaxSelect
	};
	Decide ModeDecide_;
public:
	BaseScene(GameObject* parent);
	BaseScene(GameObject* parent, const std::string& name);
	virtual ~BaseScene();

	virtual void Update() override;

	virtual void Select();
	virtual void Decide();
};

