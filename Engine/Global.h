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
#define MODULO_TEN(x) ((x) % 10)

//-----汎用的に使う定数-----

//60fpsにおける1フレームの時間(DeltaTime)(前フレームからの経過時間) 
//使用端末によるフレームレート依存防止
//ここでは宣言のみ
extern float Dt;

//60fpsにおける1秒
const int ONE_SECOND = 60;

//0.1の定数 主にImGui用
const float ZERO_POINT_ONE = 0.1f;

//0度の定数 主にImGui用
const float ANGLE_0_DEG = 0.0f;

//360度の定数 主にImGui用
const float ANGLE_360_DEG = 360.0f;