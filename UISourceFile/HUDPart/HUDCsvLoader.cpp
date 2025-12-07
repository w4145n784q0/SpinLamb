#include "HUDCsvLoader.h"

namespace
{
	//csv読み込み時のインデックス(イージング処理用の変数)
	enum EasingIndex
	{
		i_GoMinScale = 0,
		i_GoMaxScale,
		i_GoEndTime_,
		i_TimeMinScale,
		i_TimeMaxScale,
		i_TimeDuration,
		i_Max
	};
}

HUDCsvLoader::HUDCsvLoader(GameObject* parent)
	:GameObject(parent, "HUDCsvLoader"), hudParam_(nullptr)
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
	InitCSVTransformArray(csvTransform, ParamNames, hudParam_->ImageArray);

	//csvファイルを読み込む
	CsvReader csvEasing;
	csvEasing.Load("CSVdata\\HUDData\\HUDSomeData.csv");

	//csvファイル(HUDSomeData.csv)の0列目の文字列を取得
	std::string EasingLogoName = "Easing";

	//0列目の文字列を渡し、その行のパラメータを取得
	std::vector<float> EasingData = GetCSVReadData(csvEasing, EasingLogoName);

	//初期化の順番はcsvの各行の順番に合わせる
	//vの添え字はnamespaceで宣言した列挙型を使用
	hudParam_->GoMinScale_   = EasingData[i_GoMinScale];
	hudParam_->GoMaxScale_   = EasingData[i_GoMaxScale];
	hudParam_->GoEndTime_    = EasingData[i_GoEndTime_];
	hudParam_->TimeMinScale_ = EasingData[i_TimeMinScale];
	hudParam_->TimeMaxScale_ = EasingData[i_TimeMaxScale];
	hudParam_->TimeDuration_ = EasingData[i_TimeDuration];
}