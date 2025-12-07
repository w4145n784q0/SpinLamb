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

	std::string ImagePath = "Image\\";
	std::string Practice = "Practice\\";
	std::string Battle = "Battle\\";
	std::string Play = "Play\\";
	std::string GameMode = "GameMode\\";
	std::string Number = "number\\";
	std::string MiniMap = "MiniMap\\";


	//各画像の読み込み
	hudParam_->hPracticeNow_ = Image::Load(ImagePath + Practice + "PracticeLogo.png");
	assert(hudParam_->hPracticeNow_ >= 0);

	hudParam_->hGameExplanation_ = Image::Load(ImagePath + Battle + "Rule.png");
	assert(hudParam_->hGameExplanation_ >= 0);

	hudParam_->hStart_ = Image::Load(ImagePath + Battle + "StartLogo.png");
	assert(hudParam_->hStart_ >= 0);

	hudParam_->hReady_ = Image::Load(ImagePath + Battle + "ReadyLogo.png");
	assert(hudParam_->hReady_ >= 0);

	hudParam_->hGo_ = Image::Load(ImagePath + Battle + "GoLogo.png");
	assert(hudParam_->hGo_ >= 0);

	hudParam_->hFinish_ = Image::Load(ImagePath + Battle + "FinishLogo.png");
	assert(hudParam_->hFinish_ >= 0);

	hudParam_->hSplitLine_ = Image::Load(ImagePath + Play + "ViewLine.png");
	assert(hudParam_->hSplitLine_ >= 0);

	hudParam_->hNumber0_ = Image::Load(ImagePath + Number + "Number0.png");
	assert(hudParam_->hNumber0_ >= 0);

	hudParam_->hNumber1_ = Image::Load(ImagePath + Number + "Number1.png");
	assert(hudParam_->hNumber1_ >= 0);

	hudParam_->hNumber2_ = Image::Load(ImagePath + Number + "Number2.png");
	assert(hudParam_->hNumber2_ >= 0);

	hudParam_->hNumber3_ = Image::Load(ImagePath + Number + "Number3.png");
	assert(hudParam_->hNumber3_ >= 0);

	hudParam_->hNumber4_ = Image::Load(ImagePath + Number + "Number4.png");
	assert(hudParam_->hNumber4_ >= 0);

	hudParam_->hNumber5_ = Image::Load(ImagePath + Number + "Number5.png");
	assert(hudParam_->hNumber5_ >= 0);

	hudParam_->hNumber6_ = Image::Load(ImagePath + Number + "Number6.png");
	assert(hudParam_->hNumber6_ >= 0);

	hudParam_->hNumber7_ = Image::Load(ImagePath + Number + "Number7.png");
	assert(hudParam_->hNumber7_ >= 0);

	hudParam_->hNumber8_ = Image::Load(ImagePath + Number + "Number8.png");
	assert(hudParam_->hNumber8_ >= 0);

	hudParam_->hNumber9_ = Image::Load(ImagePath + Number + "Number9.png");
	assert(hudParam_->hNumber9_ >= 0);

	hudParam_->hMap_ = Image::Load(ImagePath + MiniMap + "MiniMap.png");
	assert(hudParam_->hMap_ >= 0);

	hudParam_->hFirstIcon_ = Image::Load(ImagePath + MiniMap + "FirstCharaArrow.png");
	assert(hudParam_->hFirstIcon_ >= 0);

	hudParam_->hSecondIcon_ = Image::Load(ImagePath + MiniMap + "SecondCharaArrow.png");
	assert(hudParam_->hSecondIcon_ >= 0);

	hudParam_->hPauseMenu_ = Image::Load(ImagePath + Play + "Pause.png");
	assert(hudParam_->hPauseMenu_ >= 0);

	hudParam_->hPauseIcon_ = Image::Load(ImagePath + GameMode + "SelectIcon.png");
	assert(hudParam_->hPauseIcon_ >= 0);
}
