#include "HUDParam.h"

HUDParam::HUDParam(GameObject* parent)
	:GameObject(parent, "HUDParam")
{
}

void HUDParam::InitImageArray()
{
	//数字画像ハンドル配列を初期化
	ArrayHandle = { hNumber0_,hNumber1_,hNumber2_,hNumber3_,hNumber4_,
	hNumber5_,hNumber6_,hNumber7_,hNumber8_,hNumber9_ };
}

Transform& HUDParam::GetLogoExplanation()
{
	return LogoExplanation;
}

Transform& HUDParam::GetLogoStart() 
{
	return LogoStart;
}

Transform& HUDParam::GetLogoFinish()
{
	return LogoFinish;
}

Transform& HUDParam::GetLogoPractice()
{
	return LogoPractice;
}

Transform& HUDParam::GetFirstScoreTen()
{
	return FirstScoreTen;//プレイヤー1の10の位
}

Transform& HUDParam::GetFirstScoreOne()
{
	return FirstScoreOne;//プレイヤー1の1の位
}

Transform& HUDParam::GetSecondScoreTen()
{
	return SecondScoreTen;//プレイヤー2・Enemyの1の位
}

Transform& HUDParam::GetSecondScoreOne()
{
	return SecondScoreOne;//プレイヤー2・Enemyの1の位
}

Transform& HUDParam::GetTenTime()
{
	return TenTime;
}

Transform& HUDParam::GetOneTime()
{
	return OneTime;
}

Transform& HUDParam::GetMapIcon()
{
	return MapIcon;
}

Transform& HUDParam::GetFirstIcon()
{
	return FirstIcon;
}

Transform& HUDParam::GetSecondIcon()
{
	return SecondIcon;
}

Transform& HUDParam::GetPauseMenu()
{
	return TransPauseMenu;
}

Transform& HUDParam::GetPauseIcon()
{
	return TransPauseIcon;
}