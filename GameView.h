#pragma once
#include "Engine/GameObject.h"
#include"Player.h"
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

	void Initialize();

	void ViewNormal();
	void ViewPvE();
	void ViewPlayer1();
	void ViewPlayer2();

	bool IsSingle();
	bool IsDual();

	//描画モードを設定
	void SetGameViewMode(GameViewMode mode);

	//プレイヤー1のインスタンスを設定(Battle,Practiceで呼ぶ)
	void SetPlayer1(Player* _pPlayer);

	//プレイヤー2のインスタンスを設定(Battle,Practiceで呼ぶ)
	void SetPlayer2(Player* _pPlayer);
};

