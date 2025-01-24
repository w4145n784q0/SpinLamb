#pragma once
#include "Engine/GameObject.h"
class NoEntrySpace :
    public GameObject
{
private:
	int hWall_;
public:
	NoEntrySpace(GameObject* parent);
	~NoEntrySpace();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

