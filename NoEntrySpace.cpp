#include "NoEntrySpace.h"
#include"Engine/Model.h"

NoEntrySpace::NoEntrySpace(GameObject* parent)
	:GameObject(parent,"NoEntrySpace"),hWall_(-1)
{
}

NoEntrySpace::~NoEntrySpace()
{
}

void NoEntrySpace::Initialize()
{
	hWall_ = Model::Load("wall.fbx");
	assert(hWall_ >= 0);
}

void NoEntrySpace::Update()
{
}

void NoEntrySpace::Draw()
{
	Model::SetTransform(hWall_, transform_);
	Model::Draw(hWall_);
}

void NoEntrySpace::Release()
{
}
