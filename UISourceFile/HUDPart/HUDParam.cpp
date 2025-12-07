#include "HUDParam.h"

HUDParam::HUDParam(GameObject* parent)
	:GameObject(parent, "HUDParam")
{
}

void HUDParam::InitImageArray()
{
	//”š‰æ‘œƒnƒ“ƒhƒ‹”z—ñ‚ğ‰Šú‰»
	ArrayHandle_ = { hNumber0_,hNumber1_,hNumber2_,hNumber3_,hNumber4_,
	hNumber5_,hNumber6_,hNumber7_,hNumber8_,hNumber9_ };
}