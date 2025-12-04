#include "HUDImageLoader.h"
#include"../../Engine/Image.h"

HUDImageLoader::HUDImageLoader(GameObject* parent)
	:GameObject(parent, "HUDImageLoader"),hudParam_(nullptr)
{
}

void HUDImageLoader::ImageInitialize()
{
	//同じディレクトリ内からのパスは省略
	//パスの一部を文字列にし、結合させる

	std::string Image = "Image\\";
	std::string Practice = "Practice\\";
	std::string Battle = "Battle\\";
	std::string Play = "Play\\";
	std::string Number = "number\\";
	std::string MiniMap = "MiniMap\\";


	//各画像の読み込み
	hudParam_->hPracticeNow_ = Image::Load(Image + Practice + "PracticeLogo.png");
	assert(hudParam_->hPracticeNow_ >= 0);

	hudParam_->hGameExplanation_ = Image::Load(Image + Battle + "Rule.png");
	assert(hudParam_->hGameExplanation_ >= 0);

	hudParam_->hStart_ = Image::Load(Image + Battle + "StartLogo.png");
	assert(hudParam_->hStart_ >= 0);

	hudParam_->hReady_ = Image::Load(Image + Battle + "ReadyLogo.png");
	assert(hudParam_->hReady_ >= 0);

	hudParam_->hGo_ = Image::Load(Image + Battle + "GoLogo.png");
	assert(hudParam_->hGo_ >= 0);

	hudParam_->hFinish_ = Image::Load(Image + Battle + "FinishLogo.png");
	assert(hudParam_->hFinish_ >= 0);

	hudParam_->hSplitLine_ = Image::Load(Image + Play + "ViewLine.png");
	assert(hudParam_->hSplitLine_ >= 0);

	hudParam_->hNumber0_ = Image::Load(Image + Number + "Number0.png");
	assert(hudParam_->hNumber0_ >= 0);

	hudParam_->hNumber1_ = Image::Load(Image + Number + "Number1.png");
	assert(hudParam_->hNumber1_ >= 0);

	hudParam_->hNumber2_ = Image::Load(Image + Number + "Number2.png");
	assert(hudParam_->hNumber2_ >= 0);

	hudParam_->hNumber3_ = Image::Load(Image + Number + "Number3.png");
	assert(hudParam_->hNumber3_ >= 0);

	hudParam_->hNumber4_ = Image::Load(Image + Number + "Number4.png");
	assert(hudParam_->hNumber4_ >= 0);

	hudParam_->hNumber5_ = Image::Load(Image + Number + "Number5.png");
	assert(hudParam_->hNumber5_ >= 0);

	hudParam_->hNumber6_ = Image::Load(Image + Number + "Number6.png");
	assert(hudParam_->hNumber6_ >= 0);

	hudParam_->hNumber7_ = Image::Load(Image + Number + "Number7.png");
	assert(hudParam_->hNumber7_ >= 0);

	hudParam_->hNumber8_ = Image::Load(Image + Number + "Number8.png");
	assert(hudParam_->hNumber8_ >= 0);

	hudParam_->hNumber9_ = Image::Load(Image + Number + "Number9.png");
	assert(hudParam_->hNumber9_ >= 0);

	hudParam_->hMap_ = Image::Load(Image + MiniMap + "MiniMap.png");
	assert(hudParam_->hMap_ >= 0);

	hudParam_->hFirstIcon_ = Image::Load(Image + MiniMap + "FirstCharaArrow.png");
	assert(hudParam_->hFirstIcon_ >= 0);

	hudParam_->hSecondIcon_ = Image::Load(Image + MiniMap + "SecondCharaArrow.png");
	assert(hudParam_->hSecondIcon_ >= 0);

	hudParam_->hPauseMenu_ = Image::Load(Image + "Play\\Pause.png");
	assert(hudParam_->hPauseMenu_ >= 0);

	hudParam_->hPauseIcon_ = Image::Load(Image + "GameMode\\SelectIcon.png");
	assert(hudParam_->hPauseIcon_ >= 0);
}
