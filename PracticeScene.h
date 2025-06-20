#pragma once
#include "Engine/GameObject.h"
#include"BaseScene.h"
#include"HUD.h"
#include"MiniMap.h"

//"フリープレイ"から始まる練習モードシーン
class PracticeScene :
    public BaseScene
{
private:

	//----------画像ハンドル----------

	//タイトル画像
	int hBackScreen_;

	//アイリスアウト演出用画像
	int hBackIrisIn_;

	//----------サウンドハンドル----------
	int hSoundPractice_; 

	//インスタンス
	Player* pPlayer_;
	Enemy* pEnemy_;

	HUD* pHUD_;
	MiniMap* pMiniMap_;

	//長押しでタイトルに遷移するカウント
	int Press_;

public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
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

	void UpdateActive() override;
	void UpdateTransition() override;

	void SetSCVTitle();
};

