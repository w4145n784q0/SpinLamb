#pragma once
#include "../Engine/GameObject.h"
#include"BaseScene.h"
#include"../CharacterSourceFile/Player.h"
#include"../CharacterSourceFile/Enemy.h"
#include"../UISourceFile/MiniMap.h"
#include"../UISourceFile/HUD.h"
#include"../EffectSourceFile/TransitionEffect.h"
#include"PlayBaseScene.h"

//"フリープレイ"から始まる練習モードシーン
class PracticeScene :
    public PlayBaseScene
{
private:

	//----------サウンドハンドル----------
	int hSoundPractice_; 

	//----------ポインタ----------
	Player* pPlayer1_;
	Player* pPlayer2_;
	Enemy* pEnemy_;
	HUD* pHUD_;
	TransitionEffect* pTransitionEffect_;

	//HUDクラスに渡すポインタ
	MiniMap* pMiniMap_;

	//実際に動くプレイヤー(CPUではない)のリスト
	std::vector<Player*> ActivePlayers_;

	//生成された敵(CPU)を登録する配列
	std::vector<Enemy*> ActiveEnemys_;

	//----------状態遷移----------

	//練習シーンの状態遷移
	//これらの値に応じて描画指示
	//PracticeSceneではこのシーン特有のUpdate関数はない
	enum PracticeState
	{
		S_Now = 0,		//練習中
		S_Pause,		//ポーズ中
		MaxPracticeMode
	};
	PracticeState PracticeState_;

public:
	PracticeScene(GameObject* parent);
	~PracticeScene();

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

	//----------PlayBaseSceneの継承関数----------

	//ポーズ画面に移動する
	void GotoPause() override;

	//通常状態に移動する
	void GotoPlay() override;

	//タイトルに移動する
	void GotoTitle() override;

	//選択アイコンの位置をセット
	void SetPauseIconY() override;

	//CSVから必要パラメータを読み込みする
	void SetCSVPractice();
};