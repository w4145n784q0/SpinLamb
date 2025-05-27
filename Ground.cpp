#include "Ground.h"
#include"Engine/Model.h"

Ground::Ground(GameObject* parent)
	:GameObject(parent,"Ground"),hGrass_(-1)
{
	
}

Ground::~Ground()
{
}

void Ground::Initialize()
{
	hGrass_ = Model::Load("Model\\GrassField.fbx");
	assert(hGrass_ >= 0);
}

void Ground::Update()
{
}

void Ground::Draw()
{
	Model::SetTransform(hGrass_,this->transform_);
	Model::Draw(hGrass_);
}

void Ground::Release()
{
}
