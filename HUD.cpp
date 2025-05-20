#include "HUD.h"
#include"Engine/Image.h"
#include"BattleScene.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"


//•`‰æ‘€ì‚Ì‚Ýˆµ‚¤ƒNƒ‰ƒX
namespace
{
	int CountdownNum = 0;

	Transform logo_backtitle;
	Transform logo_practice;
	//Transform CountDown;
	Transform logo_start;
	Transform logo_Finish;
	Transform TenTime;
	Transform OneTime;

	Transform pScoreTen;
	Transform pScoreOne;
	Transform eScoreTen;
	Transform eScoreOne;


	const XMFLOAT3 BackTitlePosition = { -0.55,-0.9,0 };
	const XMFLOAT3 PracticePosition = { -0.8,0.9,0 };
	//const XMFLOAT3 CountDownPosition = { 0.5,0.5,0 };
	const XMFLOAT3 StartPosition = { 0.0,0.5,0 };
	const XMFLOAT3 FinishPosition = { 0.0,0,0 };
	const XMFLOAT3 TenTimePosition = { -0.05,0.9,0 };
	const XMFLOAT3 OneTimePosition = { 0.05,0.9,0 };

	const XMFLOAT3 pScoreTenPosition = { -0.95,0.9,0 };
	const XMFLOAT3 pScoreOnePosition = { -0.9,0.9,0 };

	const XMFLOAT3 eScoreTenPosition = { 0.95,0.9,0 };
	const XMFLOAT3 eScoreOnePosition = { 0.9,0.9,0 };

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
	hBackTitleLogo_ = Image::Load("Image\\BackTitleLogo.png");
	assert(hBackTitleLogo_ >= 0);
	hPracticeNow_ = Image::Load("Image\\PracticeLogo.png");
	assert(hPracticeNow_ >= 0);

	/*std::string Number = "Image\\number.png";

	hPlayerScore_ = Image::Load(Number);
	assert(hPlayerScore_ >= 0);

	hEnemyScore_ = Image::Load(Number);
	assert(hEnemyScore_ >= 0);
	
	hTime_ = Image::Load(Number);
	assert(hTime_ >= 0);*/

	hStart_ = Image::Load("Image\\start_logo.png");
	assert(hStart_ >= 0);

	hFinish_ = Image::Load("Image\\finish_logo.png");
	assert(hFinish_ >= 0);
	
	/*hCountDown3_ = Image::Load("Image\\number_3.png");
	assert(hCountDown3_ >= 0);

	hCountDown2_ = Image::Load("Image\\number_2.png");
	assert(hCountDown2_ >= 0);

	hCountDown1_ = Image::Load("Image\\number_1.png");
	assert(hCountDown1_ >= 0);
	
	Array_Number_ = { hCountDown3_, hCountDown2_,hCountDown1_ };
	*/

	hNumber0_ = Image::Load("Image\\number\\number_0.png");
	assert(hNumber0_ >= 0);

	hNumber1_ = Image::Load("Image\\number\\number_1.png");
	assert(hNumber1_ >= 0);

	hNumber2_ = Image::Load("Image\\number\\number_2.png");
	assert(hNumber2_ >= 0);
	
	hNumber3_ = Image::Load("Image\\number\\number_3.png");
	assert(hNumber3_ >= 0);
	
	hNumber4_ = Image::Load("Image\\number\\number_4.png");
	assert(hNumber4_ >= 0);

	hNumber5_ = Image::Load("Image\\number\\number_5.png");
	assert(hNumber5_ >= 0);

	hNumber6_ = Image::Load("Image\\number\\number_6.png");
	assert(hNumber6_ >= 0);

	hNumber7_ = Image::Load("Image\\number\\number_7.png");
	assert(hNumber7_ >= 0);

	hNumber8_ = Image::Load("Image\\number\\number_8.png");
	assert(hNumber8_ >= 0);

	hNumber9_ = Image::Load("Image\\number\\number_9.png");
	assert(hNumber9_ >= 0);

	ArrayHandle_ = { hNumber0_,hNumber1_,hNumber2_,hNumber3_,hNumber4_,
	hNumber5_,hNumber6_,hNumber7_,hNumber8_,hNumber9_ };
	
	logo_backtitle.position_ = BackTitlePosition;
	logo_practice.position_ = PracticePosition;
	logo_start.position_ = StartPosition;
	logo_Finish.position_ = FinishPosition;
	TenTime.position_ = TenTimePosition;
	OneTime.position_ = OneTimePosition;

	pScoreTen.position_ = pScoreTenPosition;
	pScoreOne.position_ = pScoreOnePosition;

	eScoreTen.position_ = eScoreTenPosition;
	eScoreOne.position_ = eScoreOnePosition;
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
	/*static int count = 0;
	if (++count > 60)
	{
		count = 0;
		if(CountdownNum < Array_Number_.size() - 1)
		{
			CountdownNum++;
		}
	}*/


}

void HUD::UpdateBattleInProgress()
{
	Timeten_ = TimeNumber_ / 10;
	Timeone_ = TimeNumber_ % 10;

	PlayerTen_ = PlayerScore_ / 10;
	PlayerOne_ = PlayerScore_ % 10;

	EnemyTen_ =  EnemyScore_ / 10;
	EnemyOne_ = EnemyScore_ % 10;
}

void HUD::UpdateBattleEnd()
{
}

void HUD::UpdatePractice()
{
}

void HUD::DrawBattlePreStart()
{	
	Image::SetTransform(hFinish_, logo_start);
	Image::Draw(hStart_);
}

void HUD::DrawBattleInProgress()
{
	Image::SetTransform(ArrayHandle_[Timeten_], TenTime);
	Image::Draw(ArrayHandle_[Timeten_]);
	Image::SetTransform(ArrayHandle_[Timeone_], OneTime);
	Image::Draw(ArrayHandle_[Timeone_]);

	/*Image::SetTransform(ArrayHandle_[PlayerTen_],pScoreTen );
	Image::Draw(ArrayHandle_[PlayerTen_]);
	Image::SetTransform(ArrayHandle_[PlayerOne_], pScoreOne);
	Image::Draw(ArrayHandle_[PlayerOne_]);

	Image::SetTransform(ArrayHandle_[EnemyTen_], eScoreTen);
	Image::Draw(ArrayHandle_[EnemyTen_]);
	Image::SetTransform(ArrayHandle_[EnemyOne_], eScoreOne);
	Image::Draw(ArrayHandle_[EnemyOne_]);*/
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
