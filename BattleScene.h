#pragma once
#include"Engine/Text.h"
#include "Engine/GameObject.h"
#include"Player.h"
#include"Enemy.h"
#include"GameTimer.h"
#include"HUD.h"

//"バトル"から始まるモードシーン
class BattleScene :
    public GameObject
{
private:

	//インスタンス
	Text* pPlayerScore_;
	Text* pEnemyScore_;

	HUD* pHUD_;
	GameTimer* pGameTimer_;//hud側から操作する場合のタイマークラスポインタ


	//----------バトルシーンステート----------
	enum Battle
	{
		BEFORE,//開始前
		NOW,//バトル中
		AFTER,//終了後
		MAX
	};
	Battle BattleState_;

	//タイトル画像
	int hBackScreen_;

	//----------サウンドハンドル----------

	//バトル中サウンド
	int hSoundBattle_;

	//終了ホイッスルサウンド
	int hSoundWhistle_;

	//現在の時間(1秒ごとに減少)
	int GameTime_;

	//プレイヤーのスコア
	int PlayerScore_;

	//敵のスコア
	int EnemyScore_;

public:
	BattleScene(GameObject* parent);
	~BattleScene();

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

