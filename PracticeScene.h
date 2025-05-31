#pragma once
#include "Engine/GameObject.h"
#include"SceneData.h"

class PracticeScene :
    public SceneData
{
private:

	//----------画像ハンドル----------

	//タイトル画像
	int hBackScreen_;

	//長押しでタイトルに遷移するカウント
	int Press_;

	//----------サウンドハンドル----------
	int hSoundPractice_; 

public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	PracticeScene(GameObject* parent);

	~PracticeScene();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
};

