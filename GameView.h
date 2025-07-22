#pragma once
#include "Engine/GameObject.h"
#include"Player.h"
#include"HUD.h"
#include"Engine/Direct3D.h"

#include"Engine/Camera.h"
//-----------------------------------------------------------
//画面分割・描画
//-----------------------------------------------------------
namespace GameView
{
	//画面描画モード
	enum GameViewMode
	{
		S_NormalScreen = 0,//タイトル,ゲームモードなどの全体画面
		S_Single,//一人プレイ中の描画
		S_Dual,//二人プレイ中の描画
		MaxScreen
	};

	//ゲーム画面の映し方の初期化
	void Initialize();

	//画面全体描画(メニューなど使用)
	void ViewNormal();

	void Release();

	//一人プレイ用描画(プレイヤーの背中にカメラを置く)
	void ViewPvE();

	//二人プレイ用描画(左画面分割)
	void ViewPlayer1();

	//二人プレイ用描画(右画面分割)
	void ViewPlayer2();

	//HUD用描画(画面全体)
	void ViewHUDNormal();

	///画面描画モードが一人プレイ中かどうか返す
	bool IsSingle();

	///画面描画モードが二人プレイ中かどうか返す
	bool IsDual();

	//描画モードを設定
	void SetGameViewMode(GameViewMode mode);

	//プレイヤー1のインスタンスを設定(Battle,Practiceで呼ぶ)
	void SetPlayer1(Player* _pPlayer);

	//プレイヤー2のインスタンスを設定(Battle,Practiceで呼ぶ)
	void SetPlayer2(Player* _pPlayer);

	void SetPlayers(std::vector<Player*> _players);

	//HUDのインスタンスを設定
	void SetHUD(HUD* _HUD);
};

