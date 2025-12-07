#include "MiniMap.h"

namespace
{
	//csv読み込み時のインデックス(マップの各補正値)
	enum MiniMapIndex
	{
		i_ReductionXParam = 0,
		i_ReductionZParam,
		i_CorrectionValueXParam,
		i_CorrectionValueYParam,
	};

	//キャラクターのX座標を縮小する値
	float ReductionX = 0.0f;

	//キャラクターのZ座標を縮小する値
	float ReductionZ = 0.0f;

	//マップのX座標を補正する値
	float CorrectionValueX = 0.0f;

	//マップのY座標を補正する値
	float CorrectionValueY = 0.0f;
}

MiniMap::MiniMap(GameObject* parent)
	:GameObject(parent,"MiniMap"),
	FirstPos_({ 0,0,0 }), SecondPos_({ 0,0,0 }),
	OriginalFirstPos_({ 0,0,0 }), OriginalSecondPos_({ 0,0,0 }),
	FirstRot_({ 0,0,0 }), SecondRot_({ 0,0,0 })
{
}

MiniMap::~MiniMap()
{
}

void MiniMap::Initialize()
{
	//csvからパラメータ読み込み
	SetMiniMapCSV();
}

void MiniMap::Update()
{
	//プレイヤー・CPUの位置をとり続ける
	//キャラクターのワールド座標を縮小,補正値を足してマップ内に表示

	FirstPos_.x = (OriginalFirstPos_.x * ReductionX) + CorrectionValueX;
	FirstPos_.y = (OriginalFirstPos_.z * ReductionZ) - CorrectionValueY;

	SecondPos_.x = (OriginalSecondPos_.x * ReductionX) + CorrectionValueX;
	SecondPos_.y = (OriginalSecondPos_.z * ReductionZ) - CorrectionValueY;
}

void MiniMap::Draw()
{

}

void MiniMap::Release()
{
}

void MiniMap::SetMiniMapCSV()
{
	//csvファイル読み込む
	CsvReader csv;
	csv.Load("CSVdata\\HUDData\\MiniMapData.csv");

	//csvファイルの各0列目の文字列を取得
	std::string Init = "MiniMapInit";

	//0列目の文字列を渡し、その行のパラメータを取得
	std::vector<float> MapData = GetCSVReadData(csv, Init);

	//初期化の順番はcsvの各行の順番に合わせる
	//vの添え字はnamespaceで宣言した列挙型を使用
	ReductionX = MapData[i_ReductionXParam];
	ReductionZ = MapData[i_ReductionZParam];
	CorrectionValueX = MapData[i_CorrectionValueXParam];
	CorrectionValueY = MapData[i_CorrectionValueYParam];
	
}
