#pragma once
#include "Engine/GameObject.h"
#include<array>
#include<list>
class GameModeScene :
    public GameObject
{
private:
	int hBack_;//背景
	int hBattle_;//バトルスタートボタン
	int hPractice_;//練習モードボタン
	int hHowtoPlay_;//あそびかた説明ボタン
	int hBackTitle_;//タイトルのボタン
	int hFrameLine_;//枠線

	int hArrow_;//選択の矢印
	int hModeSelect_;//モードセレクト

	int hModeSound_;

	enum Mode
	{
		Battle,
		Practice,
		HowToPlay,
		Title,
		Max
	};
	Mode SelectMode_;
	std::list<Mode> ModeList_;//各モードのリスト
	std::list<Mode>::iterator itr;//ModeList_のインデックスを指す値
	

	Transform Trans_Arrow_;
	std::array<Transform, 5> ModeSetTrans;
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

