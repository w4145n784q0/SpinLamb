#pragma once
#include "Engine/GameObject.h"

#include"Tree.h"

#include<vector>
class TreeManager :
    public GameObject
{
private:
	vector<Tree> Trees_;
public:
	TreeManager(GameObject* parent);
	~TreeManager();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void InitializeTree(XMFLOAT3 pos);
	const vector<Tree>& GetTrees() const { return Trees_; }
};

