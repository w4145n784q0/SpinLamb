#include "Ground.h"
#include"Engine/Model.h"

Ground::Ground(GameObject* parent)
	:GameObject(parent,"Ground"),hModel_Ground(-1)
{
}

Ground::~Ground()
{
}

void Ground::Initialize()
{
	hModel_Ground = Model::Load("ground.fbx");
	transform_.position_ = { 0,-2,0 };
}

void Ground::Update()
{
}

void Ground::Draw()
{
	Model::SetTransform(hModel_Ground, transform_);
	Model::Draw(hModel_Ground);
}

void Ground::Release()
{
}
