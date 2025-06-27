#pragma once
#include "Engine/GameObject.h"
#include"BaseScene.h"
#include"TransitionEffect.h"

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

	//----------インスタンス----------
	TransitionEffect* pTransitionEffect_;

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

	//----------BaseSceneの継承関数----------
	//通常の処理
	void UpdateActive() override;

	//シーン遷移中の処理
	void UpdateTransition() override;
};