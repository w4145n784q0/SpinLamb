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
	SphereCollider* col = new SphereCollider(XMFLOAT3(0, 0, 0), 1.0f);
	this->AddCollider(col);

	Ground* pGround = (Ground*)FindObject("Ground");
	transform_.position_ = pGround->GetTerrainTrans().position_;

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

void Terrain::OnCollision(GameObject* pTarget)
{
}
