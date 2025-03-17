#pragma once
#include "Engine/GameObject.h"
class Logo :
    public GameObject
{
private:
	enum SetLogo
	{
		S_FINISH,
		S_MAX
	};
	SetLogo LogoState;

	int hFinish_;

	Transform trans_finish_;
public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	Logo(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	void SetFinish() { LogoState = S_FINISH; }
	void SetMax() { LogoState = S_MAX; }
};

