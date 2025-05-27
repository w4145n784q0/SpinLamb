#pragma once
#include "Engine/GameObject.h"
#include"array"

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

	//制限時間(バトルシーン用)
	int hTime_;

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

	//ナンバーハンドルの配列
	std::array<int, 10> ArrayHandle_;

	//フィニッシュのロゴ(バトルシーン用)
	int hFinish_;

	//std::array<int, 3> CountDownArray_;

	//----------ゲームシーン(状態により描画内容を変更)----------
	enum GameMode
	{
		BattlePreStart,//バトルシーン開始前
		BattleInProgress,//バトルシーンプレイ中
		BattleEnd,//バトルシーン終了
		Practice,//練習シーン
		Max,
	};
	GameMode GameModeHUD_;

	//----------時間描画----------

	//時間記録
	int TimeNumber_;

	//時間カウント(10の位)
	int Timeten_;

	//時間カウント(10の位)
	int Timeone_;

	//画像描画用トランスフォーム
	Transform logo_backtitle_;
	Transform logo_practice_;
	Transform logo_start_;
	Transform logo_Finish_;
	Transform TenTime_;
	Transform OneTime_;


	//score描画
	int PlayerScore_;
	int EnemyScore_;

	int PlayerOne_;
	int PlayerTen_;
	int EnemyOne_;
	int EnemyTen_;

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


	void UpdateBattlePreStart();
	void UpdateBattleInProgress();
	void UpdateBattleEnd();
	void UpdatePractice();

	void DrawBattlePreStart();
	void DrawBattleInProgress();
	void DrawBattleEnd();
	void DrawPractice();

	void SetStateBattle() { GameModeHUD_ = BattlePreStart; }
	void SetStateBattleInProgress() { GameModeHUD_ = BattleInProgress; }
	void SetStateBattleEnd(){ GameModeHUD_ = BattleEnd; }
	void SetStatePractice() { GameModeHUD_ = Practice; }

	//void SetNumber(int _num) { countDownNumber_ = _num; }
	
	void SetTime(int _time) { TimeNumber_ = _time; }
	void SetPlayerScore(int _score) { PlayerScore_ = _score; }
	void SetEnemyScore(int _score) {EnemyScore_ = _score; }

	void SetCSV();
};

