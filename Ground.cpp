#include "Ground.h"
#include"Engine/Model.h"

Ground::Ground(GameObject* parent)
	:GameObject(parent,"Ground"),hModel_Ground(-1),hModel_grass(-1),hModel_hole(-1)
{
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			stageTable[i][j].height = 1;
			stageTable[i][j].type = 0;
		}
	}

	for (int i = 0; i < 20; i++) {
		stageTable[i][19].height = 4;
		stageTable[i][19].type = 1;
	}
}

Ground::~Ground()
{
}

void Ground::Initialize()
{
	hModel_Ground = Model::Load("ground.fbx");
	hModel_grass = Model::Load("GrassBox.fbx");
	hModel_hole = Model::Load("box.fbx");

	blockArray = { hModel_grass,hModel_hole };
	/*transform_.scale_ = { 3.0,1.0,3.0 };
	transform_.position_ = { 0,-2,0 };*/



}

void Ground::Update()
{
}

void Ground::Draw()
{
	for (int x = 0; x < 20; x++)
	{
		for (int z = 0; z < 20; z++)
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
	}


	/*Model::SetTransform(hModel_Ground, transform_);
	Model::Draw(hModel_Ground);*/
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
