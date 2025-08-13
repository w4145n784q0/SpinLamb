#pragma once
#include "Engine/GameObject.h"
#include"GameTimer.h"
#include"MiniMap.h"

//時間表示・ロゴ・モード表示などのUIを描画するクラス
//座標などを初期化時に読みこみ、指示されたら表示する

namespace
{

	//描画モード（状況に応じて表示/非表示を切り替えたいもの）を指定
	//他クラスからSetDrawModeを通じて指示するため、namespaceに宣言(このクラスからは変更しない)
	enum DrawMode
	{
		Mode_BeforeStart = 0,//スタート前
		Mode_JustBefore,//"Ready?","Go!"を出す、ゲーム開始直前
		Mode_Playing,//ゲーム中
		Mode_Finish,//"Finish"を出す
		Mode_Practice,//フリープレイ時
		Mode_None,
	};

}

class HUD :
	public GameObject
{
private:
	//----------画像ハンドル----------

	//タイトルに戻る(練習シーン用)
	int hBackTitleLogo_;

	//練習中(練習シーン用)
	int hPracticeNow_;

	//ゲーム開始前の簡易説明
	int hGameExplanation_;

	//ゲーム開始"start!"ロゴ(バトルシーン用)
	int hStart_;

	//ゲーム開始"ready?"ロゴ(バトルシーン用)
	int hReady_;

	//ゲーム開始"go!"ロゴ(バトルシーン用)
	int hGo_;

	//フィニッシュのロゴ(バトルシーン用)
	int hFinish_;

	//画面分割の枠
	int hSplitLine_;

	//ナンバー
	int hNumber0_;
	int hNumber1_;
	int hNumber2_;
	int hNumber3_;
	int hNumber4_;
	int hNumber5_;
	int hNumber6_;
	int hNumber7_;
	int hNumber8_;
	int hNumber9_;

	//マップ全体
	int hMap_;

	//プレイヤー1のアイコン
	int hFirstIcon_;

	//敵(CPU)・プレイヤー2のアイコン
	int hSecondIcon_;

	//----------スコア----------

	//プレイヤー1のスコア
	int FirstScore_;

	//CPU・プレイヤー2のスコア
	int SecondScore_;

	//----------インスタンス----------
	//hud側から操作する場合のタイマークラスポインタ
	GameTimer* pGameTimer_;

	//hud側から操作する場合のミニマップクラスポインタ
	MiniMap* pMiniMap_;

	//----------状態遷移----------

	//描画モードを格納する変数
	//直接代入はせず、SetDrawModeから変更される
	DrawMode DrawMode_;

	enum DrawStartMode
	{
		S_StartReady = 0,//"Ready?"を表示している状態
		S_StartGo,//"Go!"を表示している状態
		S_MaxStartMode,
	};
	DrawStartMode DrawStart_;//DrawStartで呼ぶ状態遷移

	//----------イージング----------

	//DrawReady()からDrawGo()に遷る際のタイマー
	//このクラス内では定義せず他クラスから代入される
	int ReadyTimer_;

	//イージング使用時のカウンター
	float EasingCount_;

public:
	HUD(GameObject* parent);
	~HUD();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	//ImGuiの描画
	void DrawImGui();

	//全画面描画
	void DrawFullScreen();

	//CSVファイルから必要パラメータを読み込みする
	void SetHUDCSV();

	//描画するスコア設定
	void SetFirstScore(int score) { FirstScore_ = score; }
	void SetSecondScore(int score) { SecondScore_ = score; }

	//各ポインタを設定
	void SetTimerPointer(GameTimer* _gametimer) { pGameTimer_ = _gametimer; }
	void SetMiniMapPointer(MiniMap* _minimap) { pMiniMap_ = _minimap; }

	//描画モードを変更 ここで指定した_drawmodeがDraw()にて呼ばれる
	void SetDrawMode(DrawMode _drawmode) { DrawMode_ = _drawmode; }

	//DrawReady()からDrawGo()に遷る時間設定
	//バラバラに持つことを防ぐため,BattleSceneから指示して設定
	void SetReadyTimer(int _timer) { ReadyTimer_ = _timer; }

	//----------描画関数----------
	//これらは直接呼ばず、SetDrawModeを介して,HUDクラスのDrawから描画(常に表示するものは例外)

	//練習モード中
	void DrawPracticeLogo();

	//タイマー
	void DrawTimer();

	//タイマーのイージング処理
	void DrawTimerEasing();

	//ゲームの簡易説明
	void DrawExplanation();

	//開始時(Ready)のロゴ
	void DrawStartLogo();

	//終了ロゴ
	void DrawFinishLogo();

	//画面分割の枠線
	void DrawSplitLine();

	//ミニマップ
	void DrawMiniMap();

	//スコア表示
	void DrawScore();

	//----------ImGui描画関数(Imguiを外部から呼ぶ)----------
	void DrawImGuiExplanation();
	void DrawImGuiStartLogo();
	void DrawImGuiFinishLogo();
	void DrawImGuiPracticeLogo();

	void DrawImGuiScore();
	void DrawImGuiTimer();
	void DrawImGuiMiniMap();

private:
	//以下の描画関数は他クラスから呼ばず、上記の描画関数から呼ぶ
	//Draw -> DrawStart -> DrawReady DrawGo

	void DrawReady();
	void DrawGo();
};

