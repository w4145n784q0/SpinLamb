#include "Ground.h"
#include"Engine/Model.h"
#include"Engine/SphereCollider.h"
#include"Terrain.h"
#include"Tree.h"
#include"TreeManager.h"
#include"GoalItem.h"
#include"NoEntrySpace.h"

namespace {
	XMFLOAT3 GoalItemPos = {40,1,40};
}

Ground::Ground(GameObject* parent)
	:GameObject(parent,"Ground"),hGrass_(-1)
{
	//csvの値は各ブロックの高さを保管
	csv_.Load("Stage1.csv");

	stageWidth_ = csv_.GetWidth();    //１行に何個データがあるか
	stageHeight_ = csv_.GetHeight();   //データが何行あるか

	//
	for (int i = 0; i < stageHeight_; i++)
	{
		vector<int> data(stageWidth_, 0);
		MapData_.push_back(data);
		MapHeight_.push_back(data);
	}


	for (int j = 0; j < stageHeight_; j++)
	{
		for (int i = 0; i < stageWidth_; i++)
		{
			MapData_[j][i] = csv_.GetValue(i, j);
		}
	}

	for (int j = 0; j < stageHeight_; j++)
	{
		for (int i = 0; i < stageWidth_; i++)
		{
			int data = MapData_[j][i];
			data = data % 10;
			
			MapHeight_[j][i] = data;
		}
	}

	

	//Instantiate<TreeManager>(this);
}

Ground::~Ground()
{
}

void Ground::Initialize()
{
	hGrass_ = Model::Load("GrassBox.fbx");
	assert(hGrass_ >= 0);

	mapTrans_.position_ = { 0,0,0 };

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
			mapTrans_.position_ = { (float)x, -1 ,(float)z };
			Model::SetTransform(hGrass_, mapTrans_);
			Model::Draw(hGrass_);
		}
	}
}

void Ground::Release()
{
}

void Ground::TerrainSet()
{
	for (int z = 0; z < stageHeight_; z++) {
		for (int x = 0; x < stageWidth_; x++) {

			int data = MapData_[z][x];
			if (data == 1)
			{
				XMFLOAT3 pos = { (float)x ,(float)0 ,(float)z };
				Terrain* pTerrain =  Instantiate<Terrain>(this);
				pTerrain->SetPosition(pos);
			}

		}
	}
}
void Ground::ObjectSet()
{
	TreeManager* pTreeManager = (TreeManager*)FindObject("TreeManager");

	for (int z = 0; z < stageHeight_; z++) {
		for (int x = 0; x < stageWidth_; x++) {
			int height = MapHeight_[z][x];
			int data = MapData_[z][x];

			switch (data)
			{
			/*case -1:
			{
				NoEntrySpace * pNoEntrySpace = Instantiate<NoEntrySpace>(this);
				pNoEntrySpace->SetPosition({ (float)x, (float)0.0f,(float)z });
			}
			break;*/

			case 10:
			case 11:
			case 12:
			case 13:
			case 14:
			{
				GoalItem* pGoalItem = Instantiate<GoalItem>(this);
				pGoalItem->SetPosition({ (float)x, (float)height,(float)z });
			}
			break;
			case 100:
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
				//TreeTrans.position_ = { (float)x, (float)height,(float)z };
				//ローカルでインスタンス生成,位置のセット

				//Tree* pTree_ = Instantiate<Tree>(this);
				//pTree_->SetPosition({ (float)x, (float)height + 1,(float)z });
				//pTreeManager->AddTree(pTree_);

				//pTreeManager->InitializeTree({ (float)x, (float)height,(float)z });

				/*Tree* pTree_ = Instantiate<Tree>(this);
				pTree_->SetPosition({ (float)x, (float)height,(float)z });*/

			}
			break;
			default:
				break;
			}
		}
	}

}

bool Ground::CanMoveFront(int x, int z)
{
	//csvの値を超えるか下回るなら falseを返す
	if (x < 0 || z < 0 || x >= stageWidth_ || z >= stageHeight_) 
	{
		return false;
	}
	return true;
}

bool Ground::CanNoEntrySpace(int x, int z)
{
	int data = MapData_[z][x];

	//進行不可エリアならfalseを返す
	if (data == -1)
	{
		return false;
	}
	return true;
}

bool Ground::CompareHeight(int prev, int nextX, int nextZ)
{
	int Player = prev;
	int Next = MapHeight_[nextZ][nextX];

	if (Player >= Next)
		return true;
	else if (Player == -1)
		return true;
	else
		return false;
}


int Ground::GetPositionData(int x, int z)
{
	//csvの値を超えるか下回るなら0を返す
	if (x < 0 || z < 0 || x >= stageWidth_ || z >= stageHeight_)
	{
		return 0;
	}

	int data = MapHeight_[z][x];

	return data;
}