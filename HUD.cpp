#include "HUD.h"
#include"Engine/Image.h"
#include"BattleScene.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"


//ï`âÊëÄçÏÇÃÇ›àµÇ§ÉNÉâÉX
namespace
{
	Transform logo_backtitle;
	Transform logo_practice;
	Transform Number_CountDown;
	Transform logo_Finish;

	const XMFLOAT3 BackTitlePosition = { -0.55,-0.9,0 };
	const XMFLOAT3 PracticePosition = { -0.8,0.9,0 };
	const XMFLOAT3 CountDownPosition = { 0.5,0.5,0 };
	const XMFLOAT3 FinishPosition = { 0.5,0.5,0 };
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

	std::string Number = "number.png";

	hPlayerScore_ = Image::Load(Number);
	hEnemyScore_ = Image::Load(Number);
	hTime_ = Image::Load(Number);
	hFinish_ = Image::Load("finish_logo.png");
	
	hCountDown3_ = Image::Load("number_3.png");
	hCountDown2_ = Image::Load("number_2.png");
	hCountDown1_ = Image::Load("number_1.png");

	logo_backtitle.position_ = BackTitlePosition;
	logo_practice.position_ = PracticePosition;
}

void HUD::Update()
{
	switch (GameModeHUD_)
	{
	case HUD::BattlePreStart:
		UpdateBattlePreStart();
		break;
	case HUD::BattleInProgress:
		UpdateBattleInProgress();
		break;
	case HUD::BattleEnd:
		UpdateBattleEnd();
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
	case HUD::BattlePreStart:
		DrawBattlePreStart();
		break;
	case HUD::BattleInProgress:
		DrawBattleInProgress();
		break;
	case HUD::BattleEnd:
		DrawBattleEnd();
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

void HUD::UpdateBattlePreStart()
{
}

void HUD::UpdateBattleInProgress()
{
}

void HUD::UpdateBattleEnd()
{
}

void HUD::UpdatePractice()
{
}

void HUD::DrawBattlePreStart()
{	
	//Image::SetRect(hCountDown_, 51.2 * countDownNumber_, 0, 51.2, 118);
	//Image::SetTransform(CountDownArray_[0], Number_CountDown);
	//Image::Draw(CountDownArray_[0]);
	
}

void HUD::DrawBattleInProgress()
{
}

void HUD::DrawBattleEnd()
{
	Image::SetTransform(hFinish_, logo_Finish);
	Image::Draw(hFinish_);
}

void HUD::DrawPractice()
{
	Image::SetTransform(hBackTitleLogo_, logo_backtitle);
	Image::Draw(hBackTitleLogo_);

	Image::SetTransform(hPracticeNow_, logo_practice);
	Image::Draw(hPracticeNow_);
}
