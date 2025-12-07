
#pragma once
#include <functional>
#include <string>

struct RenderContext
{
    //将来必要になったら入れる
};

//描画タスク構造体
//name:デバッグ用の文字列
//predicate:描画すべきか判定する関数
//pre:描画前処理
//draw:純粋な描画処理
struct RenderTask
{
    std::string name;  //デバッグ用

    //描画すべきか trueなら描く
    std::function<bool()> predicate;

    //描画前にやる処理（イージング更新など）
    std::function<void(RenderContext&)> pre;

    //純粋な描画処理
    std::function<void()> draw;
};
