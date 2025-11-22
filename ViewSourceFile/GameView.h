#pragma once
#include "../Engine/GameObject.h"
#include"../CharacterSourceFile/Player.h"
#include"../CharacterSourceFile/Enemy.h"
#include"../UISourceFile/HUD.h"
#include"../StageSourceFile/Ground.h"
#include"../StageSourceFile/Fence.h"
#include"../StageSourceFile/OutStageThing.h"
#include"../EffectSourceFile/TransitionEffect.h"

//-----------------------------------------------------------
// 画面分割・ビューポート設定・カメラ設定等を行う名前空間
// ゲームの映し方に関わる処理をまとめる
//-----------------------------------------------------------
namespace GameView
{
	//画面描画モード
	enum GameViewMode
	{
		S_NormalScreen = 0,	//タイトル,ゲームモードなどの全体画面
		S_Single,			//一人プレイ中の描画
		S_Dual,				//二人プレイ中の描画
		MaxScreen
	};

	//ゲーム画面の映し方の初期化
	void Initialize();

	//解放
	void Release();

	//画面全体描画(メニューなどに使用)
	void ViewNormal();

	//一人プレイ用描画(プレイヤーの背中にカメラを置く)
	void ViewPvE();

	//二人プレイ用描画(左画面分割)
	void ViewPlayer1();

	//二人プレイ用描画(右画面分割)
	void ViewPlayer2();

	//二人プレイ用のImage関連のみ描画(Player1)
	void ViewImagePlayer1();

	//二人プレイ用のImage関連のみ描画(Player2)
	void ViewImagePlayer2();

	//ImGui描画
	void ViewImGui();

	//HUD用描画(画面全体)
	void ViewHUDNormal();

	//シーン遷移演出描画
	void ViewTransitionEffect();

	///画面描画モードが一人プレイ中かどうか返す
	bool IsSingle();

	///画面描画モードが二人プレイ中かどうか返す
	bool IsDual();

	//描画モードを設定
	void SetGameViewMode(GameViewMode mode);

	//----------ポインタセッター関数----------

	//プレイヤーのインスタンスを配列で受け取り設定(Battle,Practiceで呼ぶ)
	void SetPlayers(std::vector<Player*> _players);

	//HUDのインスタンスを設定
	void SetHUD(HUD* _HUD);

	//Enemyのインスタンスを設定
	void SetEnemy(Enemy* _enemy);

	//Groundのインスタンスを設定
	void SetGround(Ground* _ground);

	//Fenceのインスタンスを設定
	void SetFence(Fence* _fence);

	//OutStageThingのインスタンスを設定
	void SetOutStageThing(OutStageThing* _stagething);

	//TransitionEffectのインスタンスを設定
	void SetTransitionEffect(TransitionEffect* _transition);
};

