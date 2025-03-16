#pragma once
#include"Engine/Text.h"
#include "Engine/GameObject.h"
class BossBattleScene :
    public GameObject
{
private:
	int Phase_;
	int deadCount_;
	Text* pText_;
	Text* pText2_;
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

	void DeadCountPlus() { deadCount_--; }
};

