#pragma once
#include "Engine/GameObject.h"
#include"Player.h"
#include"Engine/Direct3D.h"

#include"Engine/Camera.h"
//-----------------------------------------------------------
//âÊñ ï™äÑÅEï`âÊ
//-----------------------------------------------------------
namespace GameView
{
	enum GameViewMode
	{
		S_OneScreen = 0,
		S_TwoScreen,
		MaxScreen
	};

	void Initialize();

	void ViewNormal();
	void ViewPlayer1();
	void ViewPlayer2();

	bool IsOneScreen();
	bool IsTwoScreen();

	void SetGameViewMode(GameViewMode mode);
	void SetPlayer1(Player* _pPlayer);
	void SetPlayer2(Player* _pPlayer);
};

