#pragma once
#include "Engine/GameObject.h"
#include"BaseScene.h"
#include"TransitionEffect.h"
#include<array>

//ゲーム終了後のリザルトシーン
class ResultScene :
	public BaseScene
{
private:
	//----------画像ハンドル----------

	//タイトル画像
	int hBackScreen_;

	//"you win"のロゴ
	int hYouWin_;

	//"cpu win"のロゴ
	int hCpuWin_;

	//"player1 win"
	int hPlayer1Win_;

	//"player2 win"
	int hPlayer2Win_;

	//"Draw"のロゴ
	int hDraw_;

	//"push title"のロゴ
	int hPushTitle_;

	//----------サウンドハンドル----------

	//リザルト画面サウンド
	int hSoundResult_;

	//タイトルに戻るサウンド
	int hSoundBackTitle_;

	//----------トランスフォーム----------

	//結果画面のロゴの位置
	Transform Result_;

	//結果画面の下のロゴの位置
	Transform PushTitle_;

	//勝敗状態(状態に応じて処理を変化させる)
	enum WinnerState {
		YOU_WIN = 0,//Playerの勝利
		CPU_WIN,//Enemyの勝利
		DRAW,//引き分け
		RESULTMAX
	};
	WinnerState winner_;

	//各結果ロゴの画像ハンドル配列
	std::array<int, RESULTMAX> ResultArray_;

	//----------インスタンス----------
	TransitionEffect* pTransitionEffect_;
public:
	ResultScene(GameObject* parent);
	~ResultScene();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	//CSVから必要パラメータを読み込みする
	void SetCSVResult();

	//----------BaseSceneの継承関数----------
	//通常の処理
	void UpdateActive() override;

	//シーン遷移中の処理
	void UpdateTransition() override;
};

