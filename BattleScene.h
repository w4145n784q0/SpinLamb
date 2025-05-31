#pragma once
#include"Engine/Text.h"
#include "Engine/GameObject.h"
#include"Player.h"
#include"Enemy.h"

class BattleScene :
    public GameObject
{
private:

	//インスタンス
	Text* pPlayerScore_;
	Text* pEnemyScore_;

	//----------バトルシーンステート----------
	enum Battle
	{
		BEFORE,//開始前
		NOW,//バトル中
		AFTER,//終了後
		MAX
	};
	Battle BattleState;

	//----------サウンドハンドル----------

	//バトル中サウンド
	int hBattleSound_;

	//終了ホイッスルサウンド
	int hWhistle_;

	//現在の時間(1秒ごとに減少)
	int GameTime_;

	int PlayerScore_;
	int EnemyScore_;

public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	BattleScene(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	void UpdateBattleBefore();
	void UpdateBattle();
	void UpdateBattleAfter();

	void PlusPlayerScore() { PlayerScore_++; }
	void PlusEnemyScore() { EnemyScore_++; }
	int GetGameTime() { return GameTime_; }
	void SetGameTime(int time) { GameTime_ = time; }

	void SetCSVBattle();
};

