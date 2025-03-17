#pragma once
#include"Engine/Text.h"
#include "Engine/GameObject.h"
class BossBattleScene :
    public GameObject
{
private:

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

	//音ハンドル
	int hBattleSound_;
	int hWhistle_;

	int Phase_;
	int deadCount_;
	Text* pText_;
	Text* pText2_;

	bool IsWin_;
public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	BossBattleScene(GameObject* parent);

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

	void DeadCountPlus() { deadCount_--; }
	void PhasePlus() { Phase_++; }
};

