#include "MiniMap.h"
#include"Engine/Image.h"

namespace
{
	enum MiniMapParam 
	{
		i_reductionXParam = 0,
		i_reductionZParam,
		i_MapPositonXParam,
		i_MapPositonYParam,
		i_MapPositonZParam,
		i_CorrectionValueXParam,
		i_CorrectionValueYParam,
		i_MapAlphaParam,
	};


	float reductionX = 0.0;//キャラクターのX座標を縮小する値
	float reductionY = 0.00;//キャラクターのZ座標を縮小する値
	XMFLOAT3 MapPositon = { 0.0f,0.0f,0.0f };//マップの位置
	float CorrectionValueX = 0.0f;//マップのX座標を補正する値
	float CorrectionValueY = 0.0f;//マップのY座標を補正する値
	int MapAlpha = 0;//マップの透明度
}

MiniMap::MiniMap(GameObject* parent)
	:GameObject(parent,"MiniMap"),hMap_(-1),hPlayerIcon_(-1),hEnemyIcon_(-1),pPlayer_(nullptr), pEnemy_(nullptr)
{
}

MiniMap::~MiniMap()
{
}

void MiniMap::Initialize()
{
	SetCSV();

	hMap_ = Image::Load("Image\\MiniMap\\minimap.png");
	assert(hMap_ >= 0);

	hPlayerIcon_ = Image::Load("Image\\MiniMap\\blue_circle.png");
	assert(hPlayerIcon_ >= 0);

	hEnemyIcon_ = Image::Load("Image\\MiniMap\\red_circle.png");
	assert(hEnemyIcon_ >= 0);

	pPlayer_ = (Player*)FindObject("Player");
	pEnemy_ = (Enemy*)FindObject("Enemy");

	Trans_Map.position_ = MapPositon;
}

void MiniMap::Update()
{
	Trans_Player.position_.x = (pPlayer_->GetPosition().x * reductionX) + CorrectionValueX;
	Trans_Player.position_.y = (pPlayer_->GetPosition().z * reductionY) - CorrectionValueY;

	Trans_Enemy.position_.x = (pEnemy_->GetPosition().x * reductionX) + CorrectionValueX;
	Trans_Enemy.position_.y = (pEnemy_->GetPosition().z * reductionY) - CorrectionValueY;
}

void MiniMap::Draw()
{
#ifdef _DEBUG
	//ImGui::Begin("config 2");
	ImGui::SliderFloat("mapPos x", &Trans_Map.position_.x, 0.0, 1.0);
	ImGui::SliderFloat("mapPos y", &Trans_Map.position_.y, -1.0, 0.0);
#endif

	Image::SetTransform(hMap_, Trans_Map);
	Image::Draw(hMap_);
	Image::SetAlpha(hMap_, MapAlpha);

	Image::SetTransform(hPlayerIcon_, Trans_Player);
	Image::Draw(hPlayerIcon_);

	Image::SetTransform(hEnemyIcon_, Trans_Enemy);
	Image::Draw(hEnemyIcon_);


}

void MiniMap::Release()
{
}

void MiniMap::SetCSV()
{
	CsvReader csv;
	csv.Load("CSVdata\\MiniMapData.csv");

	std::string init = "MIniMapInit";
	if (csv.IsGetParamName(init))
	{
		std::vector<float> v = csv.GetParam(init);
		reductionX = v[i_reductionXParam];
		reductionY = v[i_reductionZParam];
		MapPositon.x = v[i_MapPositonXParam];
		MapPositon.y = v[i_MapPositonYParam];
		MapPositon.z = v[i_MapPositonZParam];
		CorrectionValueX = v[i_CorrectionValueXParam];
		CorrectionValueY = v[i_CorrectionValueYParam];
		MapAlpha = (int)v[i_MapAlphaParam];
		MapAlpha = (int)v[i_MapAlphaParam];
	}
}
