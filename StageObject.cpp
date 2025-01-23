#include "StageObject.h"
#include"Engine/Model.h"
//#include"Engine/CsvReader.h"
#include"Engine/SphereCollider.h"
#include"Ground.h"

StageObject::StageObject(GameObject* parent)
	:GameObject(parent,"StageObject"), hModel_Wall_(-1),UpVec({0,1,0})
{
}

StageObject::~StageObject()
{
}

void StageObject::Initialize()
{
	
	
}

void StageObject::Update()
{	
}

void StageObject::Draw()
{

}

void StageObject::Release()
{
}


