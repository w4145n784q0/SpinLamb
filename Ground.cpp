#include "Ground.h"
#include"Engine/Model.h"
#include"Engine/CsvReader.h"

namespace {
	int blocknum = 20;
}

Ground::Ground(GameObject* parent)
	:GameObject(parent,"Ground"),hModel_Ground(-1),hModel_Wall(-1)
{
	CsvReader csv;
	csv.Load("MapTest.csv");

	stageWidth_ = csv.GetWidth();    //１行に何個データがあるか
	stageHeight_ = csv.GetHeight();   //データが何行あるか
	/*for (int i = 0; i < blocknum; i++)
	{
		for (int j = 0; j < blocknum; j++)
		{
			stageTable[i][j].height = 1;
			stageTable[i][j].type = 0;
		}
	}

	for (int i = 0; i < blocknum; i++) {
		stageTable[i][blocknum - 1].height = 4;
		stageTable[i][blocknum - 1].type = 1;
	}*/
}

Ground::~Ground()
{
}

void Ground::Initialize()
{
	hModel_Ground = Model::Load("BaseField.fbx");
	hModel_Wall = Model::Load("wall.fbx");

	/*transform_.scale_ = { 3.0,1.0,3.0 };*/
	transform_.position_ = { 0,0,0 };



}

void Ground::Update()
{
}

void Ground::Draw()
{
	/*for (int x = 0; x < blocknum; x++)
	{
		for (int z = 0; z < blocknum; z++)
		{
 			for (int y = 0; y < stageTable[x][z].height; y++)
			{
				int type = stageTable[x][z].type;

				Transform blocktrans;
				blocktrans.position_.x = x;
				blocktrans.position_.y = y; 
				blocktrans.position_.z = z;

				Model::SetTransform(blockArray[type], blocktrans);
				Model::Draw(blockArray[type]);
			}
		}
	}*/


	Model::SetTransform(hModel_Ground, transform_);
	Model::Draw(hModel_Ground);

	Transform t;
	t.position_ = { 3,3,3 };
	Model::SetTransform(hModel_Wall, t);
	Model::Draw(hModel_Wall);
}

void Ground::Release()
{
}

bool Ground::IsMoveFront(int x, int y)
{
	if (stageTable[x][y].type == 1) {
		return false;
	}
	else {
		return true;
	}
}

bool Ground::CanMoveFront(int x, int z)
{
	return false;
}
