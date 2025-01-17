#pragma once
#include "Engine/GameObject.h"
#include"Engine/CsvReader.h"
#include<array>
#include<vector>

#include"Ground.h"

using std::vector;

class Ground :
    public GameObject
{
	int hModel_Ground;
	int hModel_Grass;
	int hModel_Wall;
	int hModel_Hole;

	/*struct StageData {
		int height;
		int type;
	};
	StageData stageTable[20][20];
	std::array<int, 2> blockArray;*/

	/// <summary>
	/// csvのデータを保管
	/// </summary>
	vector<vector<int>> MapData;

	/// <summary>
	/// MapDataの高さ情報を保管
	/// </summary>
	vector<vector<int>> MapHeight;
	int stageWidth_;
	int stageHeight_;
	Transform mapTrans;

	//地形の位置を保管する変数
	Transform ObjectTrans;

	CsvReader csv;

public:
	Ground(GameObject* parent);
	~Ground();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void OnCollision(GameObject* pTarget) override;

	void ObjectSet();
	/// <summary>
	/// プレイヤーが先に行けるか返す
	/// </summary>
	/// <param name="x"></param>
	/// <param name="z"></param>
	/// <param name="height"></param>
	/// <returns></returns>
	bool CanMoveFront(int x, int z,int height);

	int GetGrassHandle() { return hModel_Ground; }
	int GetWallHandle() { return hModel_Wall; }

	/// <summary>
	/// ゲーム上のy座標をとる
	/// </summary>
	/// <param name="x">渡されたx座標</param>
	/// <param name="z">渡されたz座標</param>
	/// <returns>csvの値をとる</returns>
	int GetPositionData(int x, int z);

	int GetStageWidth() { return stageWidth_; }
	int GetStageHeight() { return stageHeight_; }

	Transform GetObjectTrans() { return ObjectTrans; }
};

