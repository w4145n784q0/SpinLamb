#include "MiniMap.h"
#include"Engine/Image.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

namespace
{
	const double reductionX = 0.00122;
	const double reductionY = 0.0022;
}

MiniMap::MiniMap(GameObject* parent)
	:GameObject(parent,"MiniMap"),hMap_(-1),hPlayerIcon_(-1),hEnemyIcon_(-1)
{
}

MiniMap::~MiniMap()
{
}

void MiniMap::Initialize()
{
	hMap_ = Image::Load("minimap.png");
	hPlayerIcon_ = Image::Load("blue_circle.png");
	hEnemyIcon_ = Image::Load("red_circle.png");

	pPlayer_ = (Player*)FindObject("Player");
	pEnemy_ = (Enemy*)FindObject("Enemy");

	mTrans.position_ = {0.891,-0.809,0};
}

void MiniMap::Update()
{
	pTrans.position_.x = (pPlayer_->GetPosition().x * reductionX) + 0.895;
	pTrans.position_.y = (pPlayer_->GetPosition().z * reductionY) - 0.81;
	//pTrans.rotate_.z = pPlayer_->GetRotate().y;

	eTrans.position_.x = (pEnemy_->GetPosition().x * reductionX) + 0.895;
	eTrans.position_.y = (pEnemy_->GetPosition().z * reductionY) - 0.81;

	//pEnemy_ = (Enemy*)FindObject("Enemy");
	//eTrans.position_ = pEnemy_->GetPosition();

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

	Image::SetTransform(hMap_,mTrans);
	Image::Draw(hMap_);
	Image::SetAlpha(hMap_, 128);

	Image::SetTransform(hPlayerIcon_, pTrans);
	Image::Draw(hPlayerIcon_);

	Image::SetTransform(hEnemyIcon_, eTrans);
	Image::Draw(hEnemyIcon_);


}

void MiniMap::Release()
{
}
