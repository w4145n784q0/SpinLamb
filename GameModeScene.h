#pragma once
#include "Engine/GameObject.h"
#include<array>
#include<list>
class GameModeScene :
    public GameObject
{
private:
	//----------背景----------
	int hBackScreen_;//背景色
	int hBackChara_;//背景キャラ
	int hExplanation_;//ゲーム説明
	Transform BackChara_;

	//----------ボタン----------
	int hBattle_;//バトルスタートボタン
	int hPractice_;//練習モードボタン
	int hHowtoPlay_;//あそびかた説明ボタン
	int hBackTitle_;//タイトルのボタン
	int hFrameLine_;//選択枠の枠線
	std::array<Transform, 5> ModeSetTrans;//各ボタンのトランスフォーム管理配列

	//----------ロゴ・テキスト----------
	int hModeSelect_;//モードセレクト
	int hBattleText_;//画面下部のテキスト(バトル用)
	int hFreePlayText_;//画面下部のテキスト(フリープレイ用)
	int hHowtoPlayText_;//画面下部のテキスト(あそびかた用)
	int hTitleText_;//画面下部のテキスト(タイトル用)
	std::array<int, 4> TextArray_;//各テキストのハンドル配列

	//----------サウンドハンドル----------
	int hModeSound_;

	enum Mode
	{
		Battle = 0,
		Practice,
		HowToPlay,
		Title,
		Max
	};
	Mode SelectMode_;

	std::list<Mode> ModeList_;//各モードのリスト
	std::list<Mode>::iterator itr;//ModeList_のインデックスを指す値
	
	Transform Trans_Select_;//ゲームモードロゴのトランスフォーム
	Transform Trans_Text_;//テキストの固定位置

	
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

	void SetSCV();
};

