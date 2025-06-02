#include "MiniMap.h"
#include"Engine/Image.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

namespace
{
	enum MiniMapParam 
	{
		reductionXParam = 0,
		reductionZParam,
		MapPositonXParam,
		MapPositonYParam,
		MapPositonZParam,
		CorrectionValueXParam,
		CorrectionValueYParam,
		MapAlphaParam,
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
	//ImGui::SliderFloat("mapPos x", &mTrans.position_.x, 0.0, 1.0);
	//ImGui::SliderFloat("mapPos y", &mTrans.position_.y, -1.0, 0.0);
	//ImGui::Text("mapPos.x%.1f", mTrans.position_.x);
	//ImGui::Text("mapPos.y%.1f", mTrans.position_.y);
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
		reductionX = v[reductionXParam];
		reductionY = v[reductionZParam];
		MapPositon.x = v[MapPositonXParam];
		MapPositon.y = v[MapPositonYParam];
		MapPositon.z = v[MapPositonZParam];
		CorrectionValueX = v[CorrectionValueXParam];
		CorrectionValueY = v[CorrectionValueYParam];
		MapAlpha = (int)v[MapAlphaParam];
		MapAlpha = (int)v[MapAlphaParam];
	}
}
