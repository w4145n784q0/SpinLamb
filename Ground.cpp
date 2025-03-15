#include "Ground.h"
#include"Engine/Model.h"
#include"Engine/SphereCollider.h"

namespace {
	
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
	hGrass_ = Model::Load("GrassField_x30.fbx");
	assert(hGrass_ >= 0);

	mapTrans_.position_ = { 0,-0.5,0 };
}

void Ground::Update()
{
}

void Ground::Draw()
{
	Model::SetTransform(hGrass_, mapTrans_);
	Model::Draw(hGrass_);
}

void Ground::Release()
{
}

bool Ground::CanMoveFront(int x, int z)
{
	//csv‚Ì’l‚ð’´‚¦‚é‚©‰º‰ñ‚é‚È‚ç false‚ð•Ô‚·
	if (x < 0 || z < 0 || x >= stageWidth_ || z >= stageHeight_) 
	{
		return false;
	}
	return true;
}