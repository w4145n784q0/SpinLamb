#pragma once

//イージング処理を行う
namespace Easing
{
	//----------イージング関数----------

	//大きく出てから戻る動き
	double easeOutBack(double x);

	//始めに加速し減衰する
	double easeOutCubic(double x);

	//----------イージング関数応用----------

	/// <summary>
	/// 拡大率計算関数
	/// </summary>
	/// <param name="_MaxScale">最終的な最大拡大値</param>
	/// <param name="_EasingCount">アニメーション進行度</param>
	/// <returns>拡大割合</returns>
	double calculateScale(float _MaxScale,float _EasingCount);

	//CSVファイルから必要パラメータを読み込みする
	void SetCSVEasing();
}

