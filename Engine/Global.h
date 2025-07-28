#pragma once
#include "Direct3D.h"

//���S�Ƀ��������J�����邽�߂̃}�N��
#define SAFE_DELETE(p) {if ((p)!=nullptr) { delete (p); (p)=nullptr;}}
#define SAFE_DELETE_ARRAY(p) {if ((p)!=nullptr) { delete[] (p); (p)=nullptr;}}
#define SAFE_RELEASE(p) {if ((p)!=nullptr) { p->Release(); (p)=nullptr;}}

//�����_��C�ӂ̌��Ŏl�̌ܓ�
//�f�t�H���g�͏�����O��
template<typename T>
static T RoundDecimal(float _value, int _digit = 3)
{
	double factor = pow(10.0, _digit);
	return round(_value * factor) / factor;
}
