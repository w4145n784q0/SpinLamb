#pragma once
#include "Engine/GameObject.h"
class GameOverScene :
    public GameObject
{
private:
	//----------画像ハンドル----------

	//タイトル画像
	int hBackScreen_;

	//"cpu win"のロゴ
	int hlogoResult_;

	//"push title"のロゴ
	int hlogoTitle_;

	//----------サウンドハンドル----------

	//ゲームオーバーサウンド
	int hGameOverSound_;
public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	GameOverScene(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
};

