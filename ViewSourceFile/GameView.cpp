#include "GameView.h"
#include"../Engine/Direct3D.h"
#include"../Engine/Camera.h"
#include"../Engine/Input.h"
#include"../EffectSourceFile/TransitionEffect.h"

namespace GameView
{
	//参加できるプレイヤー数のインデックス
	enum pPlayerIndex
	{
		i_player1 = 0,
		i_player2,
		MaxPlayersNum	//参加できるプレイヤーの最大値
	};

	//描画モードの状態を保持
	GameView::GameViewMode GameViewMode_;

	//今のシーン(基底クラスを受け取る)
	BaseScene* pBaseScene_;

	//プレイヤー1,2のポインタ
	Player* pPlayer1_;
	Player* pPlayer2_;

	//Enemyのポインタ
	Enemy* pEnemy_;

	//HUDのポインタ
	HUD* pHUD_;

	//地面クラスのポインタ
	Ground* pGround_;

	//柵クラスのポインタ
	Fence* pFence_;

	//ステージ外オブジェクトクラスのポインタ
	OutStageThing* pOutStageThing_;

	//シーン遷移演出クラスのポインタ
	TransitionEffect* pTransitionEffect_;

	//ゲーム画面の映し方の初期化
	void GameView::Initialize()
	{
		GameViewMode_		= S_NormalScreen;
		pPlayer1_			= nullptr;
		pPlayer2_			= nullptr;
		pHUD_				= nullptr;
		pEnemy_				= nullptr;
		pGround_			= nullptr;
		pFence_				= nullptr;
		pOutStageThing_		= nullptr;
		pTransitionEffect_	= nullptr;
	}

	//解放
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
		if (pTransitionEffect_ != nullptr)
		{
			pTransitionEffect_ = nullptr;
		}
	}

	//画面全体描画(メニューなどに使用)
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

	//一人プレイ用描画(プレイヤーの背中にカメラを置く)
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

	//二人プレイ用描画(左画面分割)
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

	//二人プレイ用描画(右画面分割)
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

	//二人プレイ用のImage関連のみ描画(Player1)
	void ViewImagePlayer1()
	{
		//ここからImage関係は呼ぶ(2人プレイのとき限定)
		//Imageは単独のクラスなので、通常のImage::Drawを呼ぶと二度呼ばれてしまう
		//そのため、GameViewから別に呼ぶ

		if (pPlayer1_ == nullptr)
		{
			return;
		}

		//攻撃状態のとき、アニメーションを描画
		if (pPlayer1_->IsCharacterStateAttack())
		{
			pPlayer1_->OnDrawDashImage();
		}
	}

	//二人プレイ用のImage関連のみ描画(Player2)
	void ViewImagePlayer2()
	{
		//ここから画像関係は呼ぶ(2人プレイのとき限定)
		//Imageは単独のクラスなので、通常のImage::Drawを呼ぶと二度呼ばれてしまう
		//そのため、GameViewから別に呼ぶ

		if (pPlayer2_ == nullptr)
		{
			return;
		}

		//攻撃状態のとき、アニメーションを描画
		if (pPlayer2_->IsCharacterStateAttack())
		{
			pPlayer2_->OnDrawDashImage();
		}
	}

	//ゲーム中のImGui描画
	void GameView::ViewPlayImGui()
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

	//エンジン用(Camera,InputなどのEngine関係)のImGui描画
	void ViewEngineImGui()
	{
		//カメラ関係のImGuiを描画
		Camera::DrawImGui();

		//入力関係のImGuiを描画
		Input::DrawImGui();
	}

	//シーン用ImGui描画
	void ViewSceneImGui()
	{
		//渡されたポインタからシーン用ImGuiを描画
		//各シーン共通のものと特有のImGuiを描画する
		if (pBaseScene_ != nullptr)
		{
			pBaseScene_->DrawImGuiBaseScene();
			pBaseScene_->DrawImGuiMyScene();
		}
	}

	//HUD用描画(画面全体)
	void GameView::ViewHUDNormal()
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

	//シーン遷移演出描画
	void GameView::ViewTransitionEffect()
	{
		//渡されたポインタから遷移演出を描画
		//最終的に画面全体を覆うのでDraw()とは切り離し、mainでこの関数を呼ぶ
		//どの演出にするかは各シーンから指示

		//全体画面描画
		Direct3D::viewScreenNormal();

		if (pTransitionEffect_ == nullptr)
		{
			return;
		}

		//カメラ位置・焦点は初期値のまま
		Camera::SetPosition(Camera::GetInitPos());
		Camera::SetTarget(Camera::GetInitTar());
		Camera::Update();

		//遷移演出を描画
		pTransitionEffect_->DrawDelay();
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

	//シーンのポインタ設定
	void SetScene(BaseScene* _scene)
	{
		pBaseScene_ = _scene;
	}

	//プレイヤーのポインタを配列で受け取り設定(Battle,Practiceで呼ぶ)
	void GameView::SetPlayers(std::vector<Player*> _players)
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

	//HUDのポインタを設定
	void GameView::SetHUD(HUD* _HUD)
	{
		pHUD_ = _HUD;
	}

	//Enemyのポインタを設定
	void GameView::SetEnemy(Enemy* _enemy)
	{
		pEnemy_ = _enemy;
	}

	//Groundのポインタを設定
	void GameView::SetGround(Ground* _ground)
	{
		pGround_ = _ground;
	}

	//Fenceのポインタを設定
	void GameView::SetFence(Fence* _fence)
	{
		pFence_ = _fence;
	}

	//OutStageThingのポインタを設定
	void GameView::SetOutStageThing(OutStageThing* _stagething)
	{
		pOutStageThing_ = _stagething;
	}

	//TransitionEffectのポインタを設定
	void GameView::SetTransitionEffect(TransitionEffect* _transition)
	{
		pTransitionEffect_ = _transition;
	}
}