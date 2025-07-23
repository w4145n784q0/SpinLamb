#include "GameView.h"
#include "Engine/Global.h"

namespace GameView
{
	enum pplayerIndex
	{
		i_player1 = 0,
		i_player2,
		MaxPlayersNum,//参加できるプレイヤーの最大値
	};

	//描画モードのインスタンス
	GameView::GameViewMode GameViewMode_;

	//プレイヤー1,2のインスタンス
	Player* pPlayer1_;
	Player* pPlayer2_;

	//HUDのインスタンス
	HUD* pHUD_;

	//Enemyのインスタンス
	Enemy* pEnemy_;

	//地面クラスのインスタンス
	Ground* pGround_;

	//柵クラスのインスタンス
	Fence* pFence_;

	//ステージ外オブジェクトクラスのインスタンス
	OutStageThing* pOutStageThing_;


	void GameView::Initialize()
	{
		GameViewMode_ = S_NormalScreen;
		pPlayer1_ = nullptr;
		pPlayer2_ = nullptr;
		pHUD_ = nullptr;
		pEnemy_ = nullptr;
		pGround_ = nullptr;
		pFence_ = nullptr;
		pOutStageThing_ = nullptr;
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

	void GameView::Release()
	{
		pPlayer1_ = nullptr;
		pPlayer2_ = nullptr;
		pHUD_ = nullptr;
	}

	void GameView::ViewPvE()
	{
		if (pPlayer1_ == nullptr)
		{
			return;
		}

		//全体画面描画
		Direct3D::viewScreenNormal();

		//カメラ位置をプレイヤー1に合わせる
		Camera::SetPosition(pPlayer1_->GetCameraPosition());

		//カメラ焦点をプレイヤー1に合わせる
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

	void ViewImGui()
	{
		if (pGround_ != nullptr)
		{
			pGround_->DrawImGui();
		}
		if (pFence_ != nullptr)
		{
			pFence_->DrawImGui();
		}
		if (pOutStageThing_ != nullptr)
		{
			pOutStageThing_->DrawImGui();
		}


		//プレイヤークラスのImgui描画
		std::vector<Player*> players = { pPlayer1_,pPlayer2_ };
		for (auto player : players)
		{
			if (player != nullptr)
			{
				player->DrawImGui();
			}
		}

		if (pEnemy_ != nullptr)
		{
			pEnemy_->DrawImGui();
		}

		if (pHUD_ != nullptr)
		{
			pHUD_->DrawImGui();
		}
	}

	void ViewHUDNormal()
	{
		//全体画面描画
		Direct3D::viewScreenNormal();

		if (pHUD_ == nullptr)
		{
			return;
		}

		//カメラ位置・焦点は初期値のまま
		Camera::SetPosition(Camera::GetInitPos());
		Camera::SetTarget(Camera::GetInitTar());
		Camera::Update();

		//HUDを描画
		pHUD_->DrawFullScreen();
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

	void SetPlayers(std::vector<Player*> _players)
	{
		if (_players.empty() || _players.size() > MaxPlayersNum)
		{
			return;
		}

		if (_players.size() > i_player1)
		{
			pPlayer1_ = _players[i_player1];
		}
		else
		{
			pPlayer1_ = nullptr;
		}

		if (_players.size() > i_player2)
		{
			pPlayer2_ = _players[i_player2];
		}
		else
		{
			pPlayer2_ = nullptr;
		}
	}

	void SetHUD(HUD* _HUD)
	{
		pHUD_ = _HUD;
	}

	void SetEnemy(Enemy* _enemy)
	{
		pEnemy_ = _enemy;
	}

	void SetGround(Ground* _ground)
	{
		pGround_ = _ground;
	}

	void SetFence(Fence* _fence)
	{
		pFence_ = _fence;
	}

	void SetOutStageThing(OutStageThing* _stagething)
	{
		pOutStageThing_ = _stagething;
	}
}