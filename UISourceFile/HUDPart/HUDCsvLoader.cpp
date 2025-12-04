#include "HUDCsvLoader.h"

namespace
{

	//描画モード（状況に応じて表示/非表示を切り替えたいもの）を指定
	//他クラスからSetDrawModeを通じて指示するため、namespaceに宣言(このクラスからは変更しない)
	enum DrawMode
	{
		Mode_BeforeStart = 0,	//スタート前
		Mode_JustBefore,		//"Ready?","Go!"を出す、ゲーム開始直前
		Mode_Playing,			//バトルシーンのゲーム中
		Mode_PlayPause,			//バトルシーンのポーズ中
		Mode_Finish,			//"Finish"を出す ゲーム終了
		Mode_Practice,			//フリープレイのゲーム中
		Mode_PracticePause,		//フリープレイのポーズ中
		Mode_None,
	};

}

HUDCsvLoader::HUDCsvLoader(GameObject* parent)
	:GameObject(parent, "HUDCsvLoader")
{
}

void HUDCsvLoader::SetHUDCSV()

{
	//csvファイルを読み込む
	CsvReader csvTransform;
	csvTransform.Load("CSVdata\\HUDData\\HUDTransformData.csv");

	//csvファイル(HUDTransformData.csv)の各0列目の文字列の配列を取得
	std::vector<std::string> ParamNames = {
		"Practice","Explanation","Start","Finish","Split",
		"TenTime","OneTime","MiniMap","FirstIcon", "SecondIcon",
		"FirstScoreTen","FirstScoreOne","SecondScoreTen","SecondScoreOne",
		"PauseMenu","PauseIcon"
	};

	//まとめて初期化
	InitCSVTransformArray(csvTransform, ParamNames, ImageArray);

	//csvファイルを読み込む
	CsvReader csvEasing;
	csvEasing.Load("CSVdata\\HUDData\\HUDSomeData.csv");

	//csvファイル(HUDSomeData.csv)の0列目の文字列を取得
	std::string EasingLogoName = "Easing";

	//0列目の文字列を渡し、その行のパラメータを取得
	std::vector<float> EasingData = GetCSVReadData(csvEasing, EasingLogoName);

	//初期化の順番はcsvの各行の順番に合わせる
	//vの添え字はnamespaceで宣言した列挙型を使用
	GoMinScale = EasingData[i_GoMinScale];
	GoMaxScale = EasingData[i_GoMaxScale];
	TimeMinScale = EasingData[i_TimeMinScale];
	TimeMaxScale = EasingData[i_TimeMaxScale];
	TimeDuration = EasingData[i_TimeDuration];
}