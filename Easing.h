#pragma once

//イージング処理を行う
//使用したサイト: https://easings.net/ja
namespace Easing
{
	//----------イージング関数(基本的な計算)----------

	//最初だけゆるやかに加速、徐々に一定の速度
	double EaseInSine(double x);

	//最初は一定の速度で、徐々にゆるやかに減速
	double EaseOutSine(double x);

	//最初はゆるやかに加速、徐々に一定の速度、最後に減速
	double EaseInOutSine(double x);

	//始めは緩やかに、徐々に加速する
	double EaseInQuad(double x);

	//徐々に加速し、最後に緩やかに減速する
	double EaseOutQuad(double x);

	//緩やかに加速→一定の速度→緩やかに減速する
	double EaseInOutQuad(double x);

	//始めは緩やかに、一定のラインで大きく加速
	double EaseInCubic(double x);

	//始めに大きく加速し、徐々に緩やかになる
	double easeOutCubic(double x);

	//緩やかに加速、一定のラインで大きくし、緩やかに減衰する
	double EaseInOutCubic(double x);

	//始めは緩やかに、一定のラインで一気に加速
	double EaseInQuart(double x);

	//始めに一気に加速し、一気に減衰する
	double EaseOutQuart(double x);

	//緩やかに加速→中間で一気に加速→一気に減衰する
	double EaseInOutQuart(double x);

	//始めはあまり動かず、後半で急加速
	double EaseInQuint(double x);

	//始めに急加速し、後半はあまり動かない
	double EaseOutQuint(double x);

	//始めはあまり動かない→中間で急加速→後半あまり動かない
	double EaseInOutQuint(double x);

	//始めはほぼ動かず後半に急加速する
	double EaseInExpo(double x);

	//始めに急加速し、後半はほぼ動かない
	double EaseOutExpo(double x);

	//始めはほぼ動かず、後半に急加速し、最後に減速する
	double EaseInOutExpo(double x);

	//始めは緩やかに上昇し、後半に急加速
	double EaseInCirc(double x);

	//始めは急加速し、後半は緩やかに上昇
	double EaseOutCirc(double x);

	//緩やかに加速→中間で急加速→後半は緩やかに上昇
	double EaseInOutCirc(double x);

	//始めは下降し、急上昇する
	double EaseInBack(double x);

	//大きく上昇してから戻る
	double easeOutBack(double x);

	//下降→急上昇→下降する
	double EaseInOutBack(double x);

	//始めは緩やかに、徐々に振動する
	double EaseInElastic(double x);

	//始めは振動し、徐々に緩やかに減衰する
	double EaseOutElastic(double x);

	//緩やかに加速→振動→徐々に減衰する
	double EaseInOutElastic(double x);

	//徐々に跳ね上がる弾性の動き
	double EaseInBounce(double x);

	//徐々に減衰する弾性の動き
	double EaseOutBounce(double x);

	//徐々に跳ね上がり、徐々に減衰する弾性の動き
	double EaseInOutBounce(double x);

	//----------イージング関数応用(必要に応じて追加)----------

	/// <summary>
	/// 拡大率計算関数
	/// </summary>
	/// <param name="_MaxScale">最終的な最大拡大値</param>
	/// <param name="_EasingCount">アニメーション進行度</param>
	/// <returns>拡大割合</returns>
	double calculateScale(float _MaxScale,float _EasingCount);

}

