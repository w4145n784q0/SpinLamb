#pragma once
#include "Direct3D.h"

//安全にメモリを開放するためのマクロ
#define SAFE_DELETE(p) {if ((p)!=nullptr) { delete (p); (p)=nullptr;}}
#define SAFE_DELETE_ARRAY(p) {if ((p)!=nullptr) { delete[] (p); (p)=nullptr;}}
#define SAFE_RELEASE(p) {if ((p)!=nullptr) { p->Release(); (p)=nullptr;}}

//-----汎用的に使えるマクロ-----

//引数を10で割る(時間計算などに使う)
#define DIVISION_TEN(x) ((x) / 10)

//引数を10で割った余りを返す(時間計算などに使う)
#define MODULUS_TEN(x) ((x) % 10)

//-----汎用的に使う定数-----

//60fpsにおける1フレームの時間 使用端末によるフレームレート依存防止
const float DELTATIME = 0.016f;

//60fpsにおける1秒
const int ONE_SECOND = 60;

//0.1の定数 主にImGui用
const float ZERO_POINT_ONE = 0.1f;