#pragma once
#include "Engine/GameObject.h"
class ClearScene :
    public GameObject
{
private:
	int hImage_;
	int hlogoResult_;
	int hlogoTitle_;
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

