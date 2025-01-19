#pragma once
#include "Engine/GameObject.h"

#include"Tree.h"

#include<vector>
class TreeManager :
    public GameObject
{
private:
	vector<Tree> Trees_;
	vector<XMFLOAT3> TreePos_;
public:
	TreeManager(GameObject* parent);
	~TreeManager();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void InitializeTree(XMFLOAT3 pos);
	void InitializeTreePos(XMFLOAT3 pos);
	const vector<Tree>& GetTrees() const { return Trees_; }
	const vector<XMFLOAT3>& GetTreePos() const { return TreePos_; }
};

