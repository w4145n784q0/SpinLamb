#pragma once
#include "Engine/GameObject.h"
#include"array"

class HUD :
    public GameObject
{
private:
	//----------画像ハンドル----------

	//タイトルに戻る(練習モード用)
	int hBackTitleLogo_;

	//練習中(練習モード用)
	int hPracticeNow_;

	//プレイヤーのスコアスプライト(バトルモード用)
	int hPlayerScore_;

	//敵のスコアスプライト(バトルモード用)
	int hEnemyScore_;

	//制限時間
	int hTime_;

	//カウントダウン
	int hCountDown3_;
	int hCountDown2_;
	int hCountDown1_;

	//カウントダウンハンドルの配列
	std::array<int, 3> Array_Number_;

	//フィニッシュのロゴ
	int hFinish_;

	//std::array<int, 3> CountDownArray_;

	enum GameMode
	{
		BattlePreStart,
		BattleInProgress,
		BattleEnd,
		Practice,
		Max,
	};
	GameMode GameModeHUD_;

	int countDownNumber_;

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

	void SetNumber(int _num) { countDownNumber_ = _num; }

};

