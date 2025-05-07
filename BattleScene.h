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

	//画像ハンドル

	//"finish"のロゴ
	int hFinish_;

	//プレイヤーのHP画像
	int hPlayerLife_;

	//敵のHP画像
	int hEnemyLife_;

	//音ハンドル
	int hBattleSound_;
	int hWhistle_;

	std::array<Transform, 2> HUD_Trans_;
	//int PlayerLife_;
	//int EnemyLife_;

	//ゲーム開始カウント
	int StartCount_;

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

	//void SetPlayerHp(int _hp) { PlayerLife_ = _hp; }

	void PlusPlayerScore() { PlayerScore_++; }
	void PlusEnemyScore() { EnemyScore_++; }

};

