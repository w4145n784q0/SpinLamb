#include "GameView.h"

//描画モードのインスタンス
GameView::GameViewMode GameViewMode_;

//プレイヤー1,2のインスタンス
Player* pPlayer1_;
Player* pPlayer2_;

void GameView::Initialize()
{
	GameViewMode_ = S_NormalScreen; 
	pPlayer1_ = nullptr;
	pPlayer2_ = nullptr;
}

void GameView::ViewNormal()
{
	//全体画面描画
	Direct3D::viewScreenNormal();

	//カメラ位置・焦点は初期値のまま
	Camera::SetPosition(Camera::GetInitPos());
	Camera::SetTarget(Camera::GetInitTar());

	Camera::Update();
}

void GameView::ViewPvE()
{
	if (pPlayer1_ == nullptr)
	{
		return;
	}

	//全体画面描画
	Direct3D::viewScreenNormal();

	//カメラ位置はプレイヤー1に合わせる

	Camera::SetPosition(pPlayer1_->GetCameraPosition());

	Camera::SetTarget(pPlayer1_->GetCameraTarget());

	Camera::Update();
}

void GameView::ViewPlayer1()
{
	if (pPlayer1_ == nullptr)
	{
		return;
	}

	//左画面描画
	Direct3D::viewScreenLeft();

	//カメラ位置をプレイヤー1に合わせる
	Camera::SetPosition(pPlayer1_->GetCameraPosition());

	//カメラ焦点をプレイヤー1に合わせる
	Camera::SetTarget(pPlayer1_->GetCameraTarget());

	Camera::Update();
}

void GameView::ViewPlayer2()
{
	if (pPlayer2_ == nullptr)
	{
		return;
	}

	//右画面描画
	Direct3D::viewScreenRight();

	//カメラ位置をプレイヤー2に合わせる
	Camera::SetPosition(pPlayer2_->GetCameraPosition());

	//カメラ焦点をプレイヤー2に合わせる
	Camera::SetTarget(pPlayer2_->GetCameraTarget());

	Camera::Update();
}

bool GameView::IsSingle()
{
	if (GameViewMode_ == S_Single)
		return true;
	else
		return false;
}

bool GameView::IsDual()
{
	if (GameViewMode_ == S_Dual)
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