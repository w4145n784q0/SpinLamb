#pragma once
#include"Engine/Text.h"
#include "Engine/GameObject.h"
#include"Player.h"

class BattleScene :
    public GameObject
{
private:

	//インスタンス
	Player* pPlayer_;

	enum Battle
	{
		BEFORE,
		NOW,
		AFTER,
		MAX
	};
	Battle BattleState;

	//画像ハンドル

	int hWin_;
	int hLose_;
	int hFinish_;
	int hLife_;

	//音ハンドル
	int hBattleSound_;
	int hWhistle_;

	bool IsWin_;

	std::array<Transform, 2> HUD_Trans_;
	int PlayerLife_;
	int EnemyLife_;
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

	void SetPlayerHp(int _hp) { PlayerLife_ = _hp; }

};

