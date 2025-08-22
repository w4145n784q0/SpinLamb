#pragma once
#include "../Engine/GameObject.h"
#include"BaseScene.h"
#include"../CharacterSourceFile/Player.h"
#include"../CharacterSourceFile/Enemy.h"
#include"../UISourceFile/GameTimer.h"
#include"../UISourceFile/MiniMap.h"
#include"../UISourceFile/HUD.h"
#include"../EffectSourceFile/TransitionEffect.h"
#include"PlayScene.h"

//"バトル"から始まるモードシーン
class BattleScene :
	public PlayScene, public IGameObserver
{
private:

	//----------サウンドハンドル----------

	//バトル中サウンド
	int hSoundBattle_;

	//開始・終了ホイッスルサウンド
	int hSoundWhistle_;

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

	//----------状態遷移----------
	 
	//バトルシーンの状態遷移
	//これらの値に応じて各Update関数を呼び出す
	//そのほか描画指示にも使用する
	enum BattleState
	{
		S_Before = 0,//開始前説明
		S_Ready,     //開始直前
		S_Now,       //バトル中
		S_Pause,     //ポーズ中
		S_After,     //終了後
		MaxBattleMode
	};
	BattleState BattleState_;

	//----------スコア----------

	//プレイヤー1のスコア
	int FirstScore_;

	//敵(CPU)・プレイヤー2のスコア
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

	//止めているときの処理
	void UpdateInActive() override;

	//シーン遷移中の処理
	void UpdateTransition() override;

	//----------PlaySceneの継承関数----------

	//ポーズ画面に移動する
	void GotoPause() override;

	//通常状態に移動する
	void GotoPlay() override;

	//タイトルに移動する
	void GotoTitle() override;

	//選択アイコンの位置をセット
	void SetPauseIconY() override;

	//----------BattleState_に応じて内容が変わるUpdate関数----------
	//バトル開始前
	void UpdateBattleBefore();

	//バトル開始直前
	void UpdateBattleReady();

	//バトル中
	void UpdateBattle();

	//バトル終了後
	void UpdateBattleAfter();


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

