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
	Text* pTime_;
	Text* pPlayerScore_;
	Text* pEnemyScore_;

	enum Battle
	{
		BEFORE,
		NOW,
		AFTER,
		MAX
	};
	Battle BattleState;

	//音ハンドル
	int hBattleSound_;
	int hWhistle_;

	//制限時間
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

	void DrawBattleBefore();
	void DrawBattle();
	void DrawBattleAfter();



	void PlusPlayerScore() { PlayerScore_++; }
	void PlusEnemyScore() { EnemyScore_++; }
	int GetGameTime() { return GameTime_; }

};

