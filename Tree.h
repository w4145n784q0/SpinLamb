#pragma once
#include "Engine/GameObject.h"
class Tree :
    public GameObject
{
	int hTree_;
public:
	Tree(GameObject* parent);
	~Tree();
	
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void OnCollision(GameObject* pTarget) override;
};

