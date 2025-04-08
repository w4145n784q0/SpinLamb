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

	//hWall_ = Model::Load("PillarWallfbx.fbx");
	//assert(hWall_ >= 0);

	transform_.position_ = { 0,-0.5,0 };
}

void Ground::Update()
{
	//EmitterData  data;

	//VFX::Start(data);
}

void Ground::Draw()
{
	Model::SetTransform(hGrass_,transform_);
	Model::Draw(hGrass_);
	//Model::SetTransform(hWall_, transform_);
	//Model::Draw(hWall_);
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
