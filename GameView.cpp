#include "GameView.h"
#include"Engine/Direct3D.h"
#include"Engine/Camera.h"

namespace GameView
{
	//参加できるプレイヤー数
	enum pplayerIndex
	{
		i_player1 = 0,
		i_player2,
		MaxPlayersNum//参加できるプレイヤーの最大値
	};

	//描画モードの状態を保持
	GameView::GameViewMode GameViewMode_;

	//プレイヤー1,2のインスタンス
	Player* pPlayer1_;
	Player* pPlayer2_;

	//Enemyのインスタンス
	Enemy* pEnemy_;

	//HUDのインスタンス
	HUD* pHUD_;

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

		//カメラ更新
		Camera::Update();
	}

	void GameView::Release()
	{
		//実体の解放はSceneManager(シーン切り替え時)
		//またはmain(プログラム終了時)で行われるので
		//実体は消さず、アドレス値を無効化するのみ(ダングリングポインタ防止)

		if (pPlayer1_ != nullptr)
		{
			pPlayer1_ = nullptr;
		}
		if (pPlayer2_ != nullptr)
		{
			pPlayer2_ = nullptr;
		}
		if (pEnemy_ != nullptr)
		{
			pEnemy_ = nullptr;
		}
		if (pHUD_ != nullptr)
		{
			pHUD_ = nullptr;
		}
		if (pGround_ != nullptr)
		{
			pGround_ = nullptr;
		}
		if (pFence_ != nullptr)
		{
			pFence_ = nullptr;
		}
		if (pOutStageThing_ != nullptr)
		{
			pOutStageThing_ = nullptr;
		}
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

		//カメラ更新
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

		//カメラ更新
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

		//カメラ更新
		Camera::Update();
	}

	void ViewImGui()
	{
		//渡されたポインタからImGuiを描画
		//ImGuiの描画は一度だけでいいので、各Draw関数ではなく
		//GameViewで処理し、mainで一度だけ呼ぶ
		//BattleScene,PracticeSceneのみこの処理を行う
		//(現状、それ以外のシーンでは画面分割しないため)

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
		//渡されたポインタからHUDを描画
		//複数描画する必要が無い(画面に一つだけ描画する)ものを描画
		//タイマーやテキスト画像など

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

		//二画面状態なら画面分割用の枠線を描画
		//通常のHUDよりも前に描画する(他のオブジェクトと被るのを防ぐため)
		if (IsDual())
		{
			pHUD_->DrawSplitLine();
		}

		//HUDを描画
		pHUD_->DrawFullScreen();
	}

	//一人プレイ状態かどうか
	bool GameView::IsSingle()
	{
		if (GameViewMode_ == S_Single)
			return true;
		else
			return false;
	}

	//二人プレイ状態かどうか
	bool GameView::IsDual()
	{
		if (GameViewMode_ == S_Dual)
			return true;
		else
			return false;
	}

	//描画モード設定
	void GameView::SetGameViewMode(GameViewMode mode)
	{
		GameViewMode_ = mode;
	}

	void SetPlayers(std::vector<Player*> _players)
	{
		//プレイヤー配列が空か最大プレイヤー数を超えたら処理しない
		if (_players.empty() || _players.size() > MaxPlayersNum)
		{
			return;
		}

		//プレイヤー数に応じて実体を代入する
		//配列数が足りないならnullptrを代入
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

	//HUDのインスタンスを設定
	void SetHUD(HUD* _HUD)
	{
		pHUD_ = _HUD;
	}

	//Enemyのインスタンスを設定
	void SetEnemy(Enemy* _enemy)
	{
		pEnemy_ = _enemy;
	}

	//Groundのインスタンスを設定
	void SetGround(Ground* _ground)
	{
		pGround_ = _ground;
	}

	//Fenceのインスタンスを設定
	void SetFence(Fence* _fence)
	{
		pFence_ = _fence;
	}

	//OutStageThingのインスタンスを設定
	void SetOutStageThing(OutStageThing* _stagething)
	{
		pOutStageThing_ = _stagething;
	}
}