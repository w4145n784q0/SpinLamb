#pragma once
#include "Engine/GameObject.h"
#include"Engine/CsvReader.h"
#include<array>
#include<vector>

#include"Tree.h"

using std::vector;

class Ground :
    public GameObject
{
	int hGrass_;

	enum Type {
		GRASS = 0,//通常　移動できる
		NOENTRY, //入れない
	};
	Type Type_;


	/// <summary>
	/// csvのデータを保管
	/// </summary>
	vector<vector<int>> MapData_;

	/// <summary>
	/// MapDataの高さ情報を保管
	/// </summary>
	vector<vector<int>> MapHeight_;

	/// <summary>
	/// MapDataの地面の種類を保管
	/// </summary>
	vector<vector<Type>> MapType_;

	int stageWidth_;
	int stageHeight_;
	Transform mapTrans_;

	CsvReader csv_;



public:
	Ground(GameObject* parent);
	~Ground();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void TerrainSet();
	void ObjectSet();

	/// <summary>
	/// プレイヤーが先に行けるか返す(csvオーバーを防ぐだけ)
	/// </summary>
	/// <param name="x"></param>
	/// <param name="z"></param>
	/// <param name="height"></param>
	/// <returns></returns>
	bool CanMoveFront(int x, int z);

	bool CanNoEntrySpace(int x, int z);

	/// <summary>
	///  移動元と移動先を比較しプレイヤーが先に行けるか返す
	/// </summary>
	/// <param name="prevX">移動元のx座標</param>
	/// <param name="prevZ">移動元のz座標</param>
	/// <param name="nextX">移動先のx座標</param>
	/// <param name="nextZ">移動先のz座標</param>
	/// <returns></returns>
	bool CompareHeight(int prevX, int prevZ, int nextX, int nextZ);

	/// <summary>
	/// 移動元と移動先を比較しプレイヤーが先に行けるか返す
	/// </summary>
	/// <param name="prev">移動元の高さ</param>
	/// <param name="nextX">移動先のx座標</param>
	/// <param name="nextZ">移動先のz座標</param>
	/// <returns></returns>
	bool CompareHeight(int prev, int nextX, int nextZ);

	/// <summary>
	/// ゲーム上のy座標をとる
	/// </summary>
	/// <param name="x">渡されたx座標</param>
	/// <param name="z">渡されたz座標</param>
	/// <returns>csvの値をとる</returns>
	int GetPositionData(int x, int z);



	int GetStageWidth() { return stageWidth_; }
	int GetStageHeight() { return stageHeight_; }
};