#include "HUD.h"
#include"Engine/Image.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

namespace
{
	Transform logo;
}

HUD::HUD(GameObject* parent)
	:GameObject(parent, "HUD"), hBackTitleLogo_(-1)
{
}

HUD::~HUD()
{
}

void HUD::Initialize()
{
	hBackTitleLogo_ = Image::Load("BackTitleLogo.png");
	logo.position_ = { -0.5,0.9,0 };
}

void HUD::Update()
{
}

void HUD::Draw()
{
	Image::SetTransform(hBackTitleLogo_, logo);
	Image::Draw(hBackTitleLogo_);

#ifdef _DEBUG
	//ImGui::Begin("config 2");
	ImGui::SliderFloat("mapPos x", &logo.position_.x, -1.0, 0.0);
	ImGui::SliderFloat("mapPos y", &logo.position_.y, -1.0, 0.0);
	ImGui::Text("mapPos.x%.1f", logo.position_.x);
	ImGui::Text("mapPos.y%.1f", logo.position_.y);
#endif
}

void HUD::Release()
{
}
