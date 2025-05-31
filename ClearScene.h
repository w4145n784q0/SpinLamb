#pragma once
#include "Engine/GameObject.h"
#include"SceneData.h"

class ClearScene :
    public SceneData
{
private:
	//----------画像ハンドル----------

	//タイトル画像
	int hBackScreen_;

	//"you win"のロゴ
	int hlogoResult_;

	//"push title"のロゴ
	int hlogoTitle_;

	//----------サウンドハンドル----------

	//ゲームクリアサウンド
	int hClearSound_;
public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	ClearScene(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;
	
	//描画
	void Draw() override;

	//開放
	void Release() override;
};

