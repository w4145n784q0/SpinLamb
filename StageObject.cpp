#include "StageObject.h"
#include"Engine/Model.h"
//#include"Engine/CsvReader.h"
#include"Engine/SphereCollider.h"

StageObject::StageObject(GameObject* parent)
	:GameObject(parent,"StageObject"), hModel_Wall_(-1)
{
}

StageObject::~StageObject()
{
}

void StageObject::Initialize()
{
	hModel_Wall_ = Model::Load("wall.fbx");
	SphereCollider* col = new SphereCollider(XMFLOAT3(0, 0, 0), 0.5f);
	this->AddCollider(col);

	trans_wall.position_ = { 0,3,10 };
}

void StageObject::Update()
{
}

void StageObject::Draw()
{
	Model::SetTransform(hModel_Wall_, trans_wall);
	Model::Draw(hModel_Wall_);
}

void StageObject::Release()
{
}

void StageObject::OnCollision(GameObject* pTarget)
{

}
