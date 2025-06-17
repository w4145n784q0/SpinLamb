#pragma once
#include "Engine/GameObject.h"
#include"BaseScene.h"
#include<array>
#include<list>

//ゲームモードを選択するシーン
class GameModeScene :
    public BaseScene
{
private:

	enum Mode
	{
		Battle = 0,
		Practice,
		HowToPlay,
		Title,
		MaxCount //モードの最大値
	};
	Mode SelectMode_;

	//----------背景----------
	int hBackScreen_;//背景色
	int hBackChara_;//背景キャラ
	Transform BackChara_;

	//----------ボタン----------
	int hBattle_;//バトルスタートボタン
	int hPractice_;//練習モードボタン
	int hHowtoPlay_;//あそびかた説明ボタン
	int hBackTitle_;//タイトルのボタン
	int hFrameLine_;//選択枠の枠線
	std::array<Transform, MaxCount> ModeArray_;//各ボタンのトランスフォーム管理配列
	std::array<int, MaxCount> ButtonArray_;//各ボタンのハンドル配列
	

	//----------ロゴ・テキスト----------
	int hModeSelect_;//モードセレクト
	int hBattleText_;//画面下部のテキスト(バトル用)
	int hFreePlayText_;//画面下部のテキスト(フリープレイ用)
	int hHowtoPlayText_;//画面下部のテキスト(あそびかた用)
	int hTitleText_;//画面下部のテキスト(タイトル用)
	std::array<int, MaxCount> TextArray_;//各テキストのハンドル配列

	//----------サウンドハンドル----------
	int hSoundGameMode_;//ゲームモードシーンのBGM
	int hSoundSelect_;//モード選択時のSE
	int hSoundDecide_;//モード決定時のSE



	std::list<Mode> ModeList_;//各モードのリスト
	std::list<Mode>::iterator itr;//ModeList_のインデックスを指す値
	
	Transform Trans_Frame_;//選択枠のトランスフォーム
	Transform Trans_Select_;//ゲームモードロゴのトランスフォーム
	Transform Trans_Text_;//テキストの固定位置

	
public:
	GameModeScene(GameObject* parent);
	~GameModeScene();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	//csvファイルの読み込み
	void SetGameModeSCV();

	void UpdateSelect() override;
	void UpdateDecide() override;
};

