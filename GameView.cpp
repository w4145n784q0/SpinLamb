#include "GameView.h"

GameView::GameViewMode GameViewMode_;
Player* pPlayer1_;
Player* pPlayer2_;

void GameView::Initialize()
{
	GameViewMode_ = S_OneScreen; 
	pPlayer1_ = nullptr;
	pPlayer2_ = nullptr;
}

void GameView::ViewNormal()
{
	Direct3D::viewScreenNormal();

	Camera::SetPosition(Camera::GetInitPos());

	Camera::SetTarget(Camera::GetInitTar());
}

void GameView::ViewPlayer1()
{
	if (pPlayer1_ == nullptr)
	{
		return;
	}

	//¶‰æ–Ê•`‰æ
	Direct3D::viewScreenLeft();

	Camera::SetPosition(pPlayer1_->GetCameraPosition());

	Camera::SetTarget(pPlayer1_->GetCameraTarget());

	Camera::Update();
}

void GameView::ViewPlayer2()
{
	if (pPlayer2_ == nullptr)
	{
		return;
	}

	//‰E‰æ–Ê•`‰æ
	Direct3D::viewScreenRight();

	Camera::SetPosition(pPlayer2_->GetCameraPosition());

	Camera::SetTarget(pPlayer2_->GetCameraTarget());

	Camera::Update();
}

bool GameView::IsOneScreen()
{
	if (GameViewMode_ == S_OneScreen) 
		return true; 
	else 
		return false;
}

bool GameView::IsTwoScreen()
{
	if (GameViewMode_ == S_TwoScreen)
		return true; 
	else 
		return false; 

}

void GameView::SetGameViewMode(GameViewMode mode)
{
	GameViewMode_ = mode;
}

void GameView::SetPlayer1(Player* _pPlayer)
{
	pPlayer1_ = _pPlayer;
}

void GameView::SetPlayer2(Player* _pPlayer)
{
	pPlayer2_ = _pPlayer;
}