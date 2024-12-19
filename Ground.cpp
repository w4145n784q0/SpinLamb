#include "Ground.h"
#include"Engine/Model.h"

namespace {
	int blocknum = 20;
}

Ground::Ground(GameObject* parent)
	:GameObject(parent,"Ground"),hModel_Ground(-1),hModel_grass(-1),hModel_hole(-1)
{
	for (int i = 0; i < blocknum; i++)
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
	for (int x = 0; x < blocknum; x++)
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
