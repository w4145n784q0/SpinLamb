#include "Ground.h"
#include"Engine/Model.h"
#include"Engine/CsvReader.h"
#include"Engine/SphereCollider.h"
#include"StageObject.h"

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

	for (int i = 0; i < stageHeight_; i++)
	{
		vector<int> data(stageWidth_, 0);
		MapData.push_back(data);
	}


	for (int j = 0; j < stageHeight_; j++)
	{
		for (int i = 0; i < stageWidth_; i++)
		{
			MapData[j][i] = csv.GetValue(i, j);
		}
	}

}

Ground::~Ground()
{
}

void Ground::Initialize()
{
	hModel_Ground = Model::Load("BaseField.fbx");
	hModel_Grass = Model::Load("GrassBox.fbx");
	hModel_Hole = Model::Load("box.fbx");
	hModel_Wall = Model::Load("wall.fbx");
	assert(hModel_Wall > 0);

	mapTrans.position_ = { 0,0,0 };

	SphereCollider* col = new SphereCollider(XMFLOAT3(0, 0, 0), 0.1f);
	this->AddCollider(col);

	ObjectSet();
}

void Ground::Update()
{
}

void Ground::Draw()
{
	for (int z = 0; z < stageHeight_; z++) {
		for (int x = 0; x < stageWidth_; x++) {
			mapTrans.position_ = { (float)x, 0 ,(float) z };
			Model::SetTransform(hModel_Grass, mapTrans);
			Model::Draw(hModel_Grass);
		}

	}

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


	//Model::SetTransform(hModel_Ground, transform_);
	//Model::Draw(hModel_Ground);


}

void Ground::Release()
{
}

void Ground::OnCollision(GameObject* pTarget)
{
}

void Ground::ObjectSet()
{
	//StageObject* pStageObject = (StageObject*)FindObject("StageObject");
	Transform ObjectPos;

	for (int z = 0; z < stageHeight_; z++) {
		for (int x = 0; x < stageWidth_; x++) {
			
			ObjectPos.position_ = { (float)x, 0 ,(float)z };

			if (MapData[z][x] == 1) 
			{
				ObjectTrans.position_ = ObjectPos.position_;
				Instantiate<StageObject>(this);
			}
			
		}

	}
}

/*bool Ground::IsMoveFront(int x, int y)
{
	if (stageTable[x][y].type == 1) {
		return false;
	}
	else {
		return true;
	}
}*/