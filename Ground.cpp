#include "Ground.h"
#include"Engine/Model.h"
#include"Engine/SphereCollider.h"
#include"Terrain.h"
#include"Tree.h"

namespace {
	int blocknum = 20;
	XMFLOAT3 GoalItemPos = {40,1,40};
}

Ground::Ground(GameObject* parent)
	:GameObject(parent,"Ground"),hModel_Ground(-1),hModel_Wall(-1)
{
	//csvの値は各ブロックの高さを保管
	csv.Load("MapTest.csv");

	stageWidth_ = csv.GetWidth();    //１行に何個データがあるか
	stageHeight_ = csv.GetHeight();   //データが何行あるか

	//
	for (int i = 0; i < stageHeight_; i++)
	{
		vector<int> data(stageWidth_, 0);
		MapData.push_back(data);
		MapHeight.push_back(data);
	}


	for (int j = 0; j < stageHeight_; j++)
	{
		for (int i = 0; i < stageWidth_; i++)
		{
			MapData[j][i] = csv.GetValue(i, j);
		}
	}

	for (int j = 0; j < stageHeight_; j++)
	{
		for (int i = 0; i < stageWidth_; i++)
		{
			int data = MapData[j][i];
			data = data % 10;
			
			MapHeight[j][i] = data;
		}
	}

	//Instantiate<GoalItem>(this);
	//transform_.position =  pGround->SetPosition()
}

Ground::~Ground()
{
}

void Ground::Initialize()
{
	hModel_Grass = Model::Load("GrassBox.fbx");
	assert(hModel_Grass >= 0);

	mapTrans.position_ = { 0,0,0 };

	SphereCollider* col = new SphereCollider(XMFLOAT3(0, 0, 0), 3.0f);
	this->AddCollider(col);

	TerrainSet();
	ObjectSet();
}

void Ground::Update()
{
}

void Ground::Draw()
{
	for (int z = 0; z < stageHeight_; z++)
	{
		for (int x = 0; x < stageWidth_; x++)
		{
			//int height = MapHeight[z][x];
			
			mapTrans.position_ = { (float)x, 0 ,(float)z };
			Model::SetTransform(hModel_Grass, mapTrans);
			Model::Draw(hModel_Grass);

			/*if(height >= 1 && height <= 100)
			{
				for (int y = 0; y < height; y++)
				{
					mapTrans.position_ = { (float)x, (float)y + 1 ,(float)z };
					Model::SetTransform(hModel_Grass, mapTrans);
					Model::Draw(hModel_Grass);
				}
			}*/
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


}

void Ground::Release()
{
}

void Ground::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Player")
	{
		
	}
}

void Ground::TerrainSet()
{
	Transform trans_terrain;

	for (int z = 0; z < stageHeight_; z++) {
		for (int x = 0; x < stageWidth_; x++) {
			
			trans_terrain.position_ = { (float)x, 0 ,(float)z };
			int posY = MapData[z][x];

			switch (posY)
			{
			//通常の地形
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			{
				for (int y = 0; y < posY; y++)
				{
					TerrainTrans.position_.x = trans_terrain.position_.x;
					TerrainTrans.position_.y = (float)y + 1.0;
					TerrainTrans.position_.z = trans_terrain.position_.z;
					Instantiate<Terrain>(this);
				}
			}
			break;
			default:
				break;
			}
		}

	}
}
void Ground::ObjectSet()
{
	for (int z = 0; z < stageHeight_; z++) {
		for (int x = 0; x < stageWidth_; x++) {
			int height = MapHeight[z][x] + 1;
			int data = MapData[z][x];

			switch (data)
			{
			case 101:
			case 102:
			case 103:
			case 104:
			case 105:
			case 106:
			case 107:
			case 108:
			case 109:
			{
				TreeTrans.position_ = { (float)x, (float)height,(float)z };
				Instantiate<Tree>(this);
			}
			default:
				break;
			}
		}
	}

}
//地形クラスで代用
bool Ground::CanMoveFront(int x, int z, int height)
{
	//csvの値を超えるか下回るならfalseを返す
	if (x < 0 || z < 0 || x >= stageWidth_ || z >= stageHeight_) 
	{
		return false;
	}

	int data = csv.GetValue(x, z);

	//今の自分以上の高さならfalseを返す
	if (height < data) 
	{
		return false;
	}

	return true;
}

int Ground::GetPositionData(int x, int z)
{
	//csvの値を超えるか下回るなら0を返す
	if (x < 0 || z < 0 || x >= stageWidth_ || z >= stageHeight_)
	{
		return 0;
	}

	int data = MapData[z][x];
	if (data < 100) 
	{
		return 0;
	}

	return data;
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