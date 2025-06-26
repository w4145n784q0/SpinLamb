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
	pPlayer_(nullptr), pEnemy_(nullptr),playerPos_({0,0,0}),enemyPos_({0,0,0})
{
}

MiniMap::~MiniMap()
{
}

void MiniMap::Initialize()
{
	//csvからパラメータ読み込み
	SetMiniMapCSV();

	//インスタンスを初期化
	pPlayer_ = (Player*)FindObject("Player");
	pEnemy_ = (Enemy*)FindObject("Enemy");

}

void MiniMap::Update()
{
	//プレイヤー・CPUの位置をとり続ける
	//キャラクターのワールド座標を縮小,補正値を足してマップ内に表示

	playerPos_.x = (pPlayer_->GetPosition().x * reductionX) + CorrectionValueX;
	playerPos_.y = (pPlayer_->GetPosition().z * reductionY) - CorrectionValueY;

	enemyPos_.x = (pEnemy_->GetPosition().x * reductionX) + CorrectionValueX;
	enemyPos_.y = (pEnemy_->GetPosition().z * reductionY) - CorrectionValueY;
}

void MiniMap::Draw()
{

}

void MiniMap::Release()
{
}

void MiniMap::SetMiniMapCSV()
{
	CsvReader csv;
	csv.Load("CSVdata\\MiniMapData.csv");

	std::string init = "MIniMapInit";
	if (csv.IsGetParamName(init))
	{
		std::vector<float> v = csv.GetParam(init);
		reductionX = v[i_reductionXParam];
		reductionY = v[i_reductionZParam];
		CorrectionValueX = v[i_CorrectionValueXParam];
		CorrectionValueY = v[i_CorrectionValueYParam];
	}
}
