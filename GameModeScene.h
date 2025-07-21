#pragma once
#include "Engine/GameObject.h"
#include"BaseScene.h"
#include"TransitionEffect.h"

#include<array>
#include<list>

//ゲームモードを選択するメニュー画面シーン
class GameModeScene :
    public BaseScene
{
private:

	//現在選択しているモード
	enum SelectMode
	{
		S_Battle = 0,//"バトル"を選択中
		S_Practice,//"フリープレイ"を選択中
		S_HowToPlay,//"あそびかた"を選択中
		S_Title,//"タイトル"を選択中
		MaxMode //モードの最大値
	};
	SelectMode SelectMode_;

	//ゲームモードシーンの状態
	enum GameModeState
	{
		S_Selecting,//モードの選択中
		S_Confirmation,//決定後の確認画面(必要に応じて実装)
		MaxState
	};
	GameModeState GameModeState_;

	//選択中のプレイ人数
	enum PlayerNum
	{
		S_PvE = 0,//一人プレイ
		S_PvP,//二人プレイ
		MaxPlayerNum
	};
	PlayerNum PlayerNum_;

	//----------背景----------

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

	//選択枠のトランスフォーム
	Transform TransFrame_;

	//各ボタンのトランスフォーム管理配列
	//csv読み込み時に初期化
	std::array<Transform, MaxMode> ModeTransArray_;

	//各ボタンの画像ハンドル配列
	std::array<int, MaxMode> ButtonImageArray_;
	

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

	//プレイ人数選択画面(あそぶ人数を選んでください~の画像)
	int hPlayerNumSelect_;

	//プレイ人数選択画面で出す、選択中のアイコン(矢印または★)
	int hPlayerSelectIcon_;

	//"モードセレクト"画像のトランスフォーム
	Transform TransSelect_;

	//ゲーム説明テキストの固定位置
	Transform TransText_;

	//プレイ人数選択画面(あそぶ人数を選んでください~の画像全体)のトランスフォーム
	Transform TransPlayer_;

	//プレイ人数選択選択中のアイコンのトランスフォーム
	Transform TransSelectPlayerNum_;

	//各テキストの画像ハンドル配列
	std::array<int, MaxMode> TextArray_;

	//プレイ人数選択画面のアイコンの位置配列
	std::array<Transform, MaxPlayerNum> PlayerTransArray_;

	//----------サウンドハンドル----------

	//ゲームモードシーンのBGM
	int hSoundGameMode_;

	//モード選択時のSE
	int hSoundSelect_;

	//モード決定時のSE
	int hSoundDecide_;

	//キャンセル時のSE
	int hSoundCancel_;


	//各モードのリスト
	std::list<SelectMode> ModeList_;

	//ModeList_のインデックスを指す値
	std::list<SelectMode>::iterator Modeitr;
	
	//選択中のプレイ人数のリスト
	std::list<PlayerNum> PlayerNumList_;

	//StateList_のインデックスを指す値
	std::list<PlayerNum>::iterator PlayerNumitr;

	//----------インスタンス----------
	TransitionEffect* pTransitionEffect_;
	
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

	//----------GameModeState_に応じて内容が変わるUpdate関数----------
	void UpdateSelecting();

	void UpdateConfirmation();

	//----------BaseSceneの継承関数----------
	//通常の処理
	void UpdateActive() override;

	//シーン遷移中の処理
	void UpdateTransition() override;
};

