#include "MiniMap.h"
#include"Engine/Image.h"

namespace
{
	//補正値のインデックス
	enum MiniMapIndex
	{
		i_reductionXParam = 0,
		i_reductionZParam,
		i_CorrectionValueXParam,
		i_CorrectionValueYParam,
	};
	//キャラクターのX座標を縮小する値
	float reductionX = 0.0f;

	//キャラクターのZ座標を縮小する値
	float reductionY = 0.0f;

	//マップのX座標を補正する値
	float CorrectionValueX = 0.0f;

	//マップのY座標を補正する値
	float CorrectionValueY = 0.0f;
}

MiniMap::MiniMap(GameObject* parent)
	:GameObject(parent,"MiniMap"),
	FirstPos_({ 0,0,0 }), SecondPos_({ 0,0,0 }),
	OriginalFirstPos_({ 0,0,0 }), OriginalSecondPos_({ 0,0,0 })
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

	FirstPos_.x = (OriginalFirstPos_.x * reductionX) + CorrectionValueX;
	FirstPos_.y = (OriginalFirstPos_.z * reductionY) - CorrectionValueY;

	SecondPos_.x = (OriginalSecondPos_.x * reductionX) + CorrectionValueX;
	SecondPos_.y = (OriginalSecondPos_.z * reductionY) - CorrectionValueY;
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
	std::string init = "MIniMapInit";

	//0列目の文字列を渡し、その行のパラメータを取得
	std::vector<float> MapData = GetCSVReadData(csv, init);

	//初期化の順番はcsvの各行の順番に合わせる
	//vの添え字はnamespaceで宣言した列挙型を使用
	reductionX = MapData[i_reductionXParam];
	reductionY = MapData[i_reductionZParam];
	CorrectionValueX = MapData[i_CorrectionValueXParam];
	CorrectionValueY = MapData[i_CorrectionValueYParam];
	
}
