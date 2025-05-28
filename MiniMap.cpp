#include "MiniMap.h"
#include"Engine/Image.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

namespace
{
	const double reductionX = 0.00122;
	const double reductionY = 0.0022;
	const XMFLOAT3 MapPositon = { 0.891,-0.809,0 };
	const float CorrectionValueX = 0.895f;
	const float CorrectionValueY = 0.81f;
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
	Image::SetAlpha(hMap_, 128);

	Image::SetTransform(hPlayerIcon_, Trans_Player);
	Image::Draw(hPlayerIcon_);

	Image::SetTransform(hEnemyIcon_, Trans_Enemy);
	Image::Draw(hEnemyIcon_);


}

void MiniMap::Release()
{
}
