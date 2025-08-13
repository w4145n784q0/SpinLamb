#pragma once
#include "Engine/GameObject.h"
#include"BaseScene.h"
#include"Player.h"
#include"Enemy.h"
#include"HUD.h"
#include"MiniMap.h"
#include"TransitionEffect.h"

//"フリープレイ"から始まる練習モードシーン
class PracticeScene :
    public BaseScene
{
private:

	//----------画像ハンドル----------

	//タイトル画像
	int hBackScreen_;

	//----------サウンドハンドル----------
	int hSoundPractice_; 

	//----------インスタンス----------
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

	//長押しでタイトルに遷移するカウント
	int Press_;

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

	//シーン遷移中の処理
	void UpdateTransition() override;

	//CSVから必要パラメータを読み込みする
	void SetCSVPractice();
};

