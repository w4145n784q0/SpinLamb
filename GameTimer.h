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
	enum TimeState {
		STOP = 0,  //待機中
		COUNTING, //カウント中
		MAXTIME 
	};
	TimeState TimeState_;

public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
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

	void UpdateTimeStop();
	void UpdateTimeCount();

	void StartTimer() { TimeState_ = COUNTING; }
	void StopTimer() { TimeState_ = STOP; }

	int GetTimeTen() { return Timeten_; }
	int GetTimeOne() { return Timeone_; }
	int GetCurrentGameTime() { return CurrentGameTime_; }
	void SetCurrentGameTime(int _time) { CurrentGameTime_ = _time; }

	/// <summary>
	/// ゲーム時間の計算
	/// </summary>
	void TimeCalclation();
};
