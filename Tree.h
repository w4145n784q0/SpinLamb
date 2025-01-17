#pragma once
#include "Engine/GameObject.h"
#include"Ground.h"

class Tree :
    public GameObject
{
	int hTree_;
	Ground* pGround_;
public:
	Tree(GameObject* parent);
	~Tree();
	
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void OnCollision(GameObject* pTarget) override;
};

