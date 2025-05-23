#pragma once
#include "Engine/GameObject.h"
class PracticeScene :
    public GameObject
{
private:
	//長押しでタイトルに遷移するカウント
	int Press_;
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

