#pragma once
#include "Engine/GameObject.h"
class GameModeScene :
    public GameObject
{
private:
	int hBack_;//背景
	int hBossMode_;//ボスモードのボタン
	int hRushMode_;//ラッシュモードのボタン
	int hBackTitle_;//タイトルのボタン
	int hArrow_;//選択の矢印
	int hModeSelect_;//モードセレクト

	int hModeSound_;

	enum Mode
	{
		Boss,
		Rush,
		Title,
		Max
	};
	Mode SelectMode_;

	Transform Trans_BossMode_;
	Transform Trans_RushMode_;
	Transform Trans_Title_;
	Transform Trans_Arrow_;
	Transform Trans_Mode_;
public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	GameModeScene(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
};

