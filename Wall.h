#pragma once
#include "Engine/GameObject.h"
class Wall :
    public GameObject
{
private:
	
public:
	Wall(GameObject* parent);
	~Wall();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void OnCollision(GameObject* pTarget) override;
};

