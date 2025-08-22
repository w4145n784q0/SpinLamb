#pragma once
#include "BaseScene.h"
#include"../UISourceFile/HUD.h"
#include<array>

//Battle,Practiceシーンの基底クラス
//これらのシーンで共通する処理を記述
class PlayScene :
    public BaseScene
{
protected:
	//----------画像ハンドル----------

	//背景
	int hBackScreen_;


	//----------トランスフォーム----------
 
	//背景のトランスフォーム
	Transform TransBackScreen_;

	//----------選択状態を管理するリスト----------

	//ポーズ画面のときに選択している状態
	enum PauseSelect
	{
		S_Continue = 0,		//続ける
		S_Exit,				//やめる
		MaxPauseSelect
	};
	PauseSelect PauseSelect_;

	//ポーズ画面の選択状態のリスト
	std::list<PauseSelect> PauseSelectList_;

	// PauseSelectList_のインデックスを指す値
	std::list<PauseSelect>::iterator Pauseitr;

	//ポーズ画面の選択アイコンの位置を仮に保管する
	float TmpIconPosY_;

	//ポーズ画面の選択アイコンの位置配列
	//PauseSelectの要素数を使い、それぞれの数値が選択中のアイコンのY位置となる
	std::array<float, MaxPauseSelect> IconPosYArray;

public:
	PlayScene(GameObject* parent);
	PlayScene(GameObject* parent, const std::string& name);
	virtual ~PlayScene();

	//初期化
	void Initialize() override;

	//CSVファイルから必要パラメータを読み込みする
	void SetPlaySceneCSV();

	//BaseSceneの継承関数だが、さらにこの関数をBattle,Practiceが継承するので
	//ここではBaseSceneの更新関数は継承しない（この時点ではまだ純粋仮想関数のまま）

	//----------継承先で必要な時に呼ばれる関数----------

	//背景描画
	void DrawBackScreen();

 	//ポーズ画面の処理
	void UpdatePauseMenu();

	//ポーズ画面に遷移を待つ処理(S_Activeに記述)
	void WaitGotoPause();

	//プレイ画面に遷移を待つ処理(S_InActiveに記述)
	void WaitGotoPlay();

	//ポーズ画面に移動する
	virtual void GotoPause() {};

	//通常状態に移動する
	virtual void GotoPlay() {};

	//シーン遷移エフェクトをセット
	//基底クラスの時点では処理を決めず、継承先のものを使用する
	virtual void SetTransitionEffect() {};

	//選択アイコンの位置をセット
	//このクラスでは方法を指定しない
	virtual void SetPauseIconY() {};
};

