#include "Terrain.h"
#include"Ground.h"
#include"Engine/Model.h"

Terrain::Terrain(GameObject* parent)
	:GameObject(parent, "Terrain"),hTerrain_(-1)
{
}

Terrain::~Terrain()
{
}

void Terrain::Initialize()
{
	hTerrain_ = Model::Load("SandGrassBox.fbx");
	assert(hTerrain_ >= 0);

}

void Terrain::Update()
{
}

void Terrain::Draw()
{
	Model::SetTransform(hTerrain_, transform_);
	Model::Draw(hTerrain_);
}

void Terrain::Release()
{
}

