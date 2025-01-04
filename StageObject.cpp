#include "StageObject.h"
#include"Engine/Model.h"
//#include"Engine/CsvReader.h"
#include"Engine/SphereCollider.h"
#include"Ground.h"

StageObject::StageObject(GameObject* parent)
	:GameObject(parent,"StageObject"), hModel_Wall_(-1)
{
	transform_.position_ = { 0,0,0 };
}

StageObject::~StageObject()
{
}

void StageObject::Initialize()
{
	hModel_Wall_ = Model::Load("wall.fbx");
	SphereCollider* col = new SphereCollider(XMFLOAT3(0, 0, 0), 0.5f);
	this->AddCollider(col);

	Ground* pGround = (Ground*)FindObject("Ground");
	transform_.position_ = pGround->GetObjectTrans().position_;

}

void StageObject::Update()
{
}

void StageObject::Draw()
{
	Model::SetTransform(hModel_Wall_, transform_);
	Model::Draw(hModel_Wall_);
}

void StageObject::Release()
{
}

void StageObject::OnCollision(GameObject* pTarget)
{

}
