#pragma once
#include "Engine/GameObject.h"
class HUD :
    public GameObject
{
private:
	//----------画像ハンドル----------

	//タイトルに戻る(練習モード用)
	int hBackTitleLogo_;

	//練習中(練習モード用)
	int hPracticeNow_;

	

	enum GameMode
	{
		Battle,
		Practice,
		Max,
	};
	GameMode GameModeHUD_;

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

	void UpdateBattle();
	void UpdatePractice();

	void DrawBattle();
	void DrawPractice();

	void SetStateBattle() { GameModeHUD_ = Battle; }
	void SetStatePractice() { GameModeHUD_ = Practice; }

};

