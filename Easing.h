#pragma once

//イージング処理を行う
namespace Easing
{
	//イージング関数

	//大きく出てから戻る動き
	double easeOutBack(double x);

	double easeOutCubic(double x);

	//イージング関数応用

	//拡大率計算関数
	double calculateScale(float _MaxScale,float _EasingCount);

	void SetSCVEasing();
}

