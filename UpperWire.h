#pragma once
#include "Engine/GameObject.h"
#include"Wire.h"

//前方の柵の当たり判定を行うクラス
class UpperWire :
    public Wire
{
public:
	UpperWire(GameObject* parent);
	~UpperWire();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
};

