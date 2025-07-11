#pragma once
#include "Engine/GameObject.h"

//スコアの計算処理をするクラス
class Score :
    public GameObject
{
private:
	//Player1のスコア
	int Score1_; 

	//Player2（CPU）のスコア
	int Score2_; 
public:
	Score(GameObject* parent);
	~Score();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	void ScoreMinus();
};

