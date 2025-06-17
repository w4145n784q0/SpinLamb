#pragma once
#include "Engine/GameObject.h"
#include"BaseScene.h"

//タイトルシーン
class TitleScene :
    public BaseScene
{
private:
	//----------画像ハンドル----------

	//タイトル画像
	int hBackScreen_;

	//----------サウンドハンドル----------

	//タイトル用サウンド
	int hSoundTitle_;

	//タイトル時の決定音
	int hSoundStart_;

public:
	TitleScene(GameObject* parent);
	~TitleScene();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	void UpdateSelect() override;
	void UpdateDecide() override;
};