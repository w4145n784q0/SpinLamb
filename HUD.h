#pragma once
#include "Engine/GameObject.h"
#include"array"
#include"GameTimer.h"
#include"MiniMap.h"
#include"Engine/Text.h"

//時間表示・ロゴ・モード表示などのUIを描画するクラス
//座標などを初期化時に読みこみ、指示されたら表示する

namespace
{

	//描画モード（状況に応じて表示/非表示を切り替えたいもの）を指定
	enum DrawMode
	{
		S_StartLogo,
		S_Timer,
		S_FinishLogo,
		S_Practice,
		S_MiniMap,
		S_None,
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

	//ゲーム開始ロゴ(バトルシーン用)
	int hStart_;

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

	//フィニッシュのロゴ(バトルシーン用)
	int hFinish_;

	//マップ全体
	int hMap_;

	//プレイヤーのアイコン
	int hPlayerIcon_;

	//敵のアイコン
	int hEnemyIcon_;

	//----------ゲームシーンステート(状態により描画内容を変更)----------
	enum GameMode
	{
		BattlePreStart,//バトルシーン開始前
		BattleInProgress,//バトルシーンプレイ中
		BattleEnd,//バトルシーン終了
		Practice,//練習シーン
		Max,
	};
	GameMode GameModeHUD_;

	//----------インスタンス----------
	GameTimer* pGameTimer_;//hud側から操作する場合のタイマークラスポインタ
	MiniMap* pMiniMap_;

	///Text* pPlayerScore_;
	//Text* pEnemyScore_;

	//描画モードを格納する変数
	DrawMode DrawMode_;

	int PlayerScore_;
	int EnemyScore_;

public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
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

	void SetCSV();

	//各ポインタを設定
	void SetTimerPointer(GameTimer* _gametimer) { pGameTimer_ = _gametimer; }
	void SetMiniMapPointer(MiniMap* _minimap) { pMiniMap_ = _minimap; }

	//描画モードを変更 ここで指定したdrawmodeがDraw()にて呼ばれる
	void SetDrawMode(DrawMode _drawmode) { DrawMode_ = _drawmode; }

	//----------描画関数----------
	//これらは直接呼ばず、SetDrawModeを介して描画(常に表示するものは例外)

	//練習モード中
	void DrawPracticeLogo();

	//タイマー
	void DrawTimer();

	//開始時のロゴ
	void DrawStartLogo();

	//終了ロゴ
	void DrawFinishLogo();

	//ミニマップ
	void DrawMiniMap();

	//スコア表示(BattleSceneで使用)
	void DrawScore();

	void SetPlayerScore(int score) { PlayerScore_ = score; }
	void SetEnemyScore(int score) { EnemyScore_ = score; }
};

