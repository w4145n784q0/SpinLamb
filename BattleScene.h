#pragma once
#include "Engine/GameObject.h"
#include"BaseScene.h"

#include"Player.h"
#include"Enemy.h"
#include"GameTimer.h"
#include"MiniMap.h"
#include"HUD.h"

#include"TransitionEffect.h"

//"バトル"から始まるモードシーン
class BattleScene :
	public BaseScene,public IGameObserver
{
private:

	//----------インスタンス----------
	Player* pPlayer1_;
	Player* pPlayer2_;
	Enemy* pEnemy_;
	HUD* pHUD_;
	TransitionEffect* pTransitionEffect_;

	//HUDクラスに渡すポインタ
	GameTimer* pGameTimer_;//BattleScene側から操作する場合のタイマークラスポインタ
	MiniMap* pMiniMap_;//HUDクラスに渡すポインタ

	//実際に操作するプレイヤーを登録する配列
	std::vector<Player*> ActivePlayers_;

	//生成された敵(CPU)を登録する配列
	std::vector<Enemy*> ActiveEnemys_;

	//----------バトルシーンステート----------
	//これらの値に応じて各Update関数を呼び出す
	enum Battle
	{
		S_BEFORE = 0,//開始前説明
		S_READY,//開始直前
		S_NOW,//バトル中
		S_AFTER,//終了後
		MAXMODE
	};
	Battle BattleState_;

	//----------画像ハンドル----------

	//タイトル画像
	int hBackScreen_;

	//----------サウンドハンドル----------

	//バトル中サウンド
	int hSoundBattle_;

	//開始・終了ホイッスルサウンド
	int hSoundWhistle_;

	//プレイヤーのスコア
	int FirstScore_;

	//敵のスコア
	int SecondScore_;

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

	//----------BaseSceneの継承関数----------
	//通常の処理
	void UpdateActive() override;

	//シーン遷移中の処理
	void UpdateTransition() override;

	//----------BattleState_に応じて内容が変わるUpdate関数----------
	//バトル開始前
	void UpdateBattleBefore();

	//バトル開始直前
	void UpdateBattleReady();

	//バトル中
	void UpdateBattle();

	//バトル終了後
	void UpdateBattleAfter();

	//------------------------------

	//CSVから必要パラメータを読み込みする
	void SetCSVBattle();

	//プレイヤー1のスコア上昇
	void PlusFirstScore() { FirstScore_++; }

	//CPU・プレイヤー2のスコア上昇
	void PlusSecondScore() { SecondScore_++; }

	/// <summary>
	/// 柵にヒットした際の処理
	/// </summary>
	/// <param name="HitCharaID">ヒットしたキャラクターのID</param>
	void OnCharacterFenceHit(int _HitCharaID) override;

};

