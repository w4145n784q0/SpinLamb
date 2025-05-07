#pragma once
#include "Engine/GameObject.h"
class HUD :
    public GameObject
{
private:
	//画像ハンドル
	int hBackTitleLogo_;
public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	HUD(GameObject* parent);

	~HUD();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

};

