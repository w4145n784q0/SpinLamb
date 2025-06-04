#pragma once
#include "Engine/GameObject.h"
#include<array>

class ResultScene :
    public GameObject
{
private:
	//----------画像ハンドル----------

	//タイトル画像
	int hBackScreen_;

	//"you win"のロゴ
	int hYouWin_;

	//"cpu win"のロゴ
	int hCpuWin_;

	//"Draw"のロゴ
	int hDraw_;

	//"push title"のロゴ
	int hlogoTitle_;

	//----------サウンドハンドル----------

	//リザルト画面サウンド
	int hSoundResult_;

	//----------トランスフォーム----------

	//結果画面のロゴの位置
	Transform Result_;

	//結果画面の下のロゴの位置
	Transform PushTitle_;

	enum WinnerState {
		YOU_WIN = 0,
		CPU_WIN,
		DRAW,
		RESULTMAX
	};
	WinnerState winner_;

	std::array<int, RESULTMAX> ResultArray_;
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

	//csv読み込み
	void SetCSVScene();
};

