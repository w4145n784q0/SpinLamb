#pragma once
#include "Direct3D.h"

//安全にメモリを開放するためのマクロ
#define SAFE_DELETE(p) {if ((p)!=nullptr) { delete (p); (p)=nullptr;}}
#define SAFE_DELETE_ARRAY(p) {if ((p)!=nullptr) { delete[] (p); (p)=nullptr;}}
#define SAFE_RELEASE(p) {if ((p)!=nullptr) { p->Release(); (p)=nullptr;}}

//小数点を任意の桁で四捨五入
//デフォルトは少数第三位
template<typename T>
static T RoundDecimal(float _value, int _digit = 3)
{
	double factor = pow(10.0, _digit);
	return round(_value * factor) / factor;
}
