#pragma once
#include "Engine/GameObject.h"
class GameTimer :
    public GameObject
{
private:

	//----------画像ハンドル----------

	//時間の数字画像ハンドル
	int hNumber0_;
	int hNumber1_;
	int hNumber2_;
	int hNumber3_;
	int hNumber4_;
	int hNumber5_;
	int hNumber6_;
	int hNumber7_;
	int hNumber8_;
	int hNumber9_;

	//----------時間描画----------

	//時間記録
	int TimeNumber_;

	//時間カウント(10の位)
	int Timeten_;

	//時間カウント(1の位)
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
};

