#pragma once
#include "Engine/GameObject.h"

//時間の計算を行うクラス
//制限時間は別クラスから受け取る
class GameTimer :
    public GameObject
{
private:

	//----------時間描画----------

	//現在の時間
	int CurrentGameTime_;

	//10の位の値
	int Timeten_;

	//1の位の値
	int Timeone_;

	//時間の状態
	//これらの値に応じて各Update関数を呼び出す
	enum TimeState
	{
		STOP = 0,  //待機中
		COUNTING, //カウント中
		MAXTIME 
	};
	TimeState TimeState_;

public:
	GameTimer(GameObject* parent);
	~GameTimer();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	//----------TimeState_に応じて内容が変わるUpdate関数----------
	//時間計測ストップ
	void UpdateTimeStop();

	//時間計測中
	void UpdateTimeCount();

	//ゲーム時間の計算
	void TimeCalclation();

	//----------セッター・ゲッター関数----------
	int GetTimeTen() const { return Timeten_; }
	int GetTimeOne() const { return Timeone_; }
	int GetCurrentGameTime() const { return CurrentGameTime_; }
	void SetCurrentGameTime(int _time) { CurrentGameTime_ = _time; }

	//時間停止を指示
	void StartTimer() { TimeState_ = COUNTING; }

	//時間計測を指示
	void StopTimer() { TimeState_ = STOP; }
};
