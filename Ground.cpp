#include "Ground.h"
#include"Engine/Model.h"
#include"Engine/SphereCollider.h"
#include"Engine/GameObject.h"

#include"Engine/VFX.h"

namespace {
	Point MovePoint[] = { {0,0},{15,15},{-15,15},{15,-15},{-15,-15} };
}

Ground::Ground(GameObject* parent)
	:GameObject(parent,"Ground"),hGrass_(-1)
{
	
}

Ground::~Ground()
{
}

void Ground::Initialize()
{
	hGrass_ = Model::Load("GrassField.fbx");
	assert(hGrass_ >= 0);
	transform_.position_ = { 0,-0.5,0 };
}

void Ground::Update()
{
}

void Ground::Draw()
{
	Model::SetTransform(hGrass_,transform_);
	Model::Draw(hGrass_);
}

void Ground::Release()
{
}

Point Ground::GetRandomMovePoint()
{
	int index = 5;
	int random = rand() % index;
	return MovePoint[random];
}
