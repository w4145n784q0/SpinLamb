#pragma once
#include "Engine/GameObject.h"
#include"BaseScene.h"
#include<array>
#include<list>

//ゲームモードを選択するメニュー画面シーン
class GameModeScene :
    public BaseScene
{
private:

	//現在選択しているモード
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
	// 
	//背景画像
	int hBackScreen_;

	//背景キャラ画像
	int hBackChara_;

	//背景のキャラクターのトランスフォーム
	Transform BackChara_;

	//----------ボタン----------
	//バトルスタートボタン画像
	int hBattle_;

	//練習モードボタン画像
	int hPractice_;

	//あそびかた説明ボタン画像
	int hHowtoPlay_;

	//タイトルボタン画像
	int hBackTitle_;

	//選択枠の枠線画像
	int hFrameLine_;

	//各ボタンのトランスフォーム管理配列
	std::array<Transform, MaxCount> ModeArray_;

	//各ボタンの画像ハンドル配列
	std::array<int, MaxCount> ButtonArray_;
	

	//----------ロゴ・テキスト----------

	//モードセレクト画像
	int hModeSelect_;

	//画面下部のゲーム説明テキスト画像(バトル用)
	int hBattleText_;

	//画面下部のゲーム説明テキスト画像(フリープレイ用)
	int hFreePlayText_;

	//画面下部のゲーム説明テキスト画像(あそびかた用)
	int hHowtoPlayText_;

	//画面下部のゲーム説明テキスト画像(タイトル用)
	int hTitleText_;

	//各テキストの画像ハンドル配列
	std::array<int, MaxCount> TextArray_;

	//----------サウンドハンドル----------

	//ゲームモードシーンのBGM
	int hSoundGameMode_;

	//モード選択時のSE
	int hSoundSelect_;

	//モード決定時のSE
	int hSoundDecide_;


	//各モードのリスト
	std::list<Mode> ModeList_;

	//ModeList_のインデックスを指す値
	std::list<Mode>::iterator itr;
	
	//選択枠のトランスフォーム
	Transform TransFrame_;

	//モードセレクト画像のトランスフォーム
	Transform TransSelect_;

	//ゲーム説明テキストの固定位置
	Transform TransText_;

	
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

	//CSVファイルから必要パラメータを読み込みする
	void SetGameModeSCV();

	//----------BaseSceneの継承関数----------
	//通常の処理
	void UpdateActive() override;

	//シーン遷移中の処理
	void UpdateTransition() override;
};

