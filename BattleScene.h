#pragma once
#include "Engine/GameObject.h"
#include"BaseScene.h"

#include"Player.h"
#include"Enemy.h"
#include"GameTimer.h"
#include"MiniMap.h"
#include"HUD.h"

//"バトル"から始まるモードシーン
class BattleScene :
	public BaseScene
{
private:

	//----------インスタンス----------
	Player* pPlayer_;
	Enemy* pEnemy_;

	HUD* pHUD_;

	//HUDクラスに渡すポインタ
	GameTimer* pGameTimer_;//BattleScene側から操作する場合のタイマークラスポインタ
	MiniMap* pMiniMap_;//HUDクラスに渡すポインタ


	//----------バトルシーンステート----------
	enum Battle
	{
		S_BEFORE = 0,//開始前説明
		S_READY,//開始直前
		S_NOW,//バトル中
		S_AFTER,//終了後
		MAXMODE
	};
	Battle BattleState_;

	//タイトル画像
	int hBackScreen_;

	//----------サウンドハンドル----------

	//バトル中サウンド
	int hSoundBattle_;

	//終了ホイッスルサウンド
	int hSoundWhistle_;

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

	void UpdateActive() override;
	void UpdateTransition() override;

	void UpdateBattleBefore();
	void UpdateBattleReady();
	void UpdateBattle();
	void UpdateBattleAfter();

	void PlusPlayerScore() { PlayerScore_++; }
	void PlusEnemyScore() { EnemyScore_++; }

	void SetCSVBattle();
};

