#include "TreeManager.h"
#include"Tree.h"

TreeManager::TreeManager(GameObject* parent)
	:GameObject(parent,"TreeManager")
{
	Trees_ = { nullptr };
}

TreeManager::~TreeManager()
{
}

void TreeManager::Initialize()
{
}

void TreeManager::Update()
{
}

void TreeManager::Draw()
{
}

void TreeManager::Release()
{
}

void TreeManager::InitializeTree(XMFLOAT3 pos)
{
	Tree* pTree_ = Instantiate<Tree>(this);
	pTree_->SetPosition(pos);
	Trees_.push_back(pTree_);
}
