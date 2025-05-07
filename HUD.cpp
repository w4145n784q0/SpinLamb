#include "HUD.h"
#include"Engine/Image.h"
#include"Engine/SceneManager.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

namespace
{
	Transform logo_backtitle;
	Transform logo_practice;

}

HUD::HUD(GameObject* parent)
	:GameObject(parent, "HUD"), hBackTitleLogo_(-1),hPracticeNow_(-1), GameModeHUD_(Max)
{
}

HUD::~HUD()
{
}

void HUD::Initialize()
{
	hBackTitleLogo_ = Image::Load("BackTitleLogo.png");
	hPracticeNow_ = Image::Load("PracticeLogo.png");

	logo_backtitle.position_ = { -0.55,-0.9,0 };
	logo_practice.position_ = { -0.8,0.9,0 };
}

void HUD::Update()
{
	switch (GameModeHUD_)
	{
	case HUD::Battle:
		UpdateBattle();
		break;
	case HUD::Practice:
		UpdatePractice();
		break;
	case HUD::Max:
		break;
	default:
		break;
	}
}

void HUD::Draw()
{
	switch (GameModeHUD_)
	{
	case HUD::Battle:
		DrawBattle();
		break;
	case HUD::Practice:
		DrawPractice();
		break;
	case HUD::Max:
		break;
	default:
		break;
	}



#ifdef _DEBUG
	//ImGui::Begin("config 2");
	//ImGui::SliderFloat("mapPos x", &logo_backtitle.position_.x, -1.0, 1.0);
	//ImGui::SliderFloat("mapPos y", &logo_backtitle.position_.y, -1.0, 1.0);
	//ImGui::Text("mapPos.x%.1f", logo.position_.x);
	//ImGui::Text("mapPos.y%.1f", logo.position_.y);
#endif
}

void HUD::Release()
{
}

void HUD::UpdateBattle()
{
}

void HUD::UpdatePractice()
{
}

void HUD::DrawBattle()
{
}

void HUD::DrawPractice()
{
	Image::SetTransform(hBackTitleLogo_, logo_backtitle);
	Image::Draw(hBackTitleLogo_);

	Image::SetTransform(hPracticeNow_, logo_practice);
	Image::Draw(hPracticeNow_);
}
