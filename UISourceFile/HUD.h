#pragma once
#include <memory>

#include "../Engine/GameObject.h"
#include"../UISourceFile/GameTimer.h"
#include"../UISourceFile/MiniMap.h"
#include"HUDPart/HUDDebugPanel.h"
#include"HUDPart/HUDParam.h"
#include"HUDPart/HUDCsvLoader.h"
#include"HUDPart/HUDImageLoader.h"
#include"HUDPart/HUDDrawTable.h"

//時間表示・ロゴ・モード表示などのUIを管理するクラス

class HUD :
	public GameObject
{
public:
	//----------モジュール----------
	//ImGui関連
	std::unique_ptr<HUDDebugPanel> HUDDebug_;

	//データ関連
	std::unique_ptr<HUDParam> HUDParam_;

	//csv読み込み関連
	std::unique_ptr<HUDCsvLoader> HUDCsvLoader_;

	//画像読み込み関連
	std::unique_ptr<HUDImageLoader> HUDImageLoader_;

	//描画モード関連
	std::unique_ptr<HUDDrawTable> HUDDrawTable_;

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

	//描画タスクテーブルの構築
	void BuildDrawTable();

	//----------以下は個別に指定する描画関数----------

	//-----以下は直接呼ばず、SetDrawModeを介して
	//描画する(シーンによって切り替わるものなど)-----

	//練習モード中
	void DrawPracticeLogo();

	//タイマー
	void DrawTimer();

	//ゲームの簡易説明
	void DrawExplanation();

	//開始時(Ready,Go)のロゴ
	void DrawStartLogo();

	//終了(Finish)ロゴ
	void DrawFinishLogo();

	//ミニマップ
	void DrawMiniMap();

	//スコア表示
	void DrawScore();

	//ポーズ画面
	void DrawPause();

	//-----以下はHUDのポインタから描画する-----
	
	//画面分割の枠線
	void DrawSplitLine();

	//-----以下はHUDクラス内で行う前処理-----

	//タイマーの更新
	void UpdateTimer();

	//タイマーのイージング処理
	void TimerEasing();

	//ミニマップの位置取得
	void UpdateMiniMapPosition();

	//スコアの更新
	void UpdateScoreCalculate();

private:
	//以下の描画関数は他クラスから呼ばず、上記の描画関数から呼ぶ
	//Draw → DrawStart → DrawReady DrawGo

	void DrawReady();
	void DrawGo();
};