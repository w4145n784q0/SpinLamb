#include "TransitionEffect.h"
#include"Engine/Image.h"
#include"Engine/CsvReader.h"

namespace
{
	//csv�ǂݍ��ݎ��̃C���f�b�N�X
	enum ZoomIndex
	{
		i_maxzoomvalue = 0,
		i_max
	};
}

TransitionEffect::TransitionEffect(GameObject* parent)
	: GameObject(parent, "TransitionEffect"),hFadeBlack_(-1),hFadeWhite_(-1),
	hZoomSheep_(-1), EffectType_(NoneEffect),TransitionTime_(0)
{
	FadeEffect_.AlphaValue_ = 0;
	ZoomEffect_.MaxZoomValue_ = 0;
}

TransitionEffect::~TransitionEffect()
{
}

void TransitionEffect::Initialize()
{
	//csv����p�����[�^�ǂݍ���
	SetSCVTransitionEffect();

	//�e�摜�̓ǂݍ���
	hFadeBlack_ = Image::Load("Image\\Transition\\fade_black.png");
	assert(hFadeBlack_ >= 0);

	hFadeWhite_ = Image::Load("Image\\Transition\\fade_white.png");
	assert(hFadeWhite_ >= 0);

	hZoomSheep_ = Image::Load("Image\\Transition\\SheepBlack.png");
	assert(hZoomSheep_ >= 0);
}

void TransitionEffect::Update()
{
	//EffectType_����s����ʑJ�ڏ������w��
	switch (EffectType_)
	{
	case TransitionEffect::S_FadeOutBlack:
	case TransitionEffect::S_FadeOutWhite:
		UpdateFadeOut();
		break;
	case TransitionEffect::S_FadeInBlack:
	case TransitionEffect::S_FadeInWhite:
		UpdateFadeIn();
		break;
	case TransitionEffect::S_SlideInLTR:
		UpdateSlideInLTR();
		break;
	case TransitionEffect::S_ZoomIn:
		UpdateZoomIn();
		break;
	case TransitionEffect::S_ZoomOut:
		UpdateZoomOut();
		break;
	default:
		break;
	}

}

void TransitionEffect::Draw()
{
	//EffectType_����s���`����w��
	switch (EffectType_)
	{
	case TransitionEffect::S_FadeOutBlack:
	case TransitionEffect::S_FadeInBlack:
	{
		Image::SetTransform(hFadeBlack_, FadeEffect_.FadeTransform_);
		Image::SetAlpha(hFadeBlack_, FadeEffect_.AlphaValue_);
		Image::Draw(hFadeBlack_);
	}
		break;
	case TransitionEffect::S_FadeOutWhite:
	case TransitionEffect::S_FadeInWhite:
	{
		Image::SetTransform(hFadeWhite_, FadeEffect_.FadeTransform_);
		Image::SetAlpha(hFadeWhite_, FadeEffect_.AlphaValue_);
		Image::Draw(hFadeWhite_);
	}
		break;
	case TransitionEffect::S_SlideInLTR:
	{
		Image::SetTransform(hFadeBlack_, SlideEffect_.SlideTransform_);
		Image::Draw(hFadeBlack_);
	}
		break;
	case TransitionEffect::S_ZoomIn:
	case TransitionEffect::S_ZoomOut:
	{
		Image::SetTransform(hZoomSheep_, ZoomEffect_.ZoomTransform_);
		Image::Draw(hZoomSheep_);
	}
		break;
	default:
		break;
	}

}

void TransitionEffect::Release()
{
}

void TransitionEffect::UpdateFadeOut()
{
	//���񂾂�Â��Ȃ�G�t�F�N�g
	if (FadeEffect_.AlphaValue_ >= Image::AlphaMin)
	{
		FadeEffect_.AlphaValue_ = Image::AlphaMin;
	}
	else
	{
		FadeEffect_.AlphaValue_ += Image::AlphaMin / TransitionTime_;
	}

}

void TransitionEffect::UpdateFadeIn()
{
	//���񂾂񖾂邭�Ȃ�G�t�F�N�g
	if (FadeEffect_.AlphaValue_ <= 0)
	{
		FadeEffect_.AlphaValue_ = 0;
	}
	else
	{
		FadeEffect_.AlphaValue_ -= Image::AlphaMin / TransitionTime_;
	}
}

void TransitionEffect::UpdateSlideInLTR()
{
	//�摜�������E�ɃX���C�h
	if (SlideEffect_.SlideTransform_.position_.x >= Image::Center)
	{
		SlideEffect_.SlideTransform_.position_.x = Image::Center;
	}
	else
	{
		SlideEffect_.SlideTransform_.position_.x += (Image::RightEdge - Image::LeftEdge) / TransitionTime_;
	}
}

void TransitionEffect::UpdateZoomIn()
{
	//���񂾂�傫���Ȃ�G�t�F�N�g
	if (ZoomEffect_.ZoomTransform_.scale_.x >= ZoomEffect_.MaxZoomValue_ ||
		ZoomEffect_.ZoomTransform_.scale_.y >= ZoomEffect_.MaxZoomValue_)
	{
		ZoomEffect_.ZoomTransform_.scale_.x = ZoomEffect_.MaxZoomValue_;
		ZoomEffect_.ZoomTransform_.scale_.y = ZoomEffect_.MaxZoomValue_;
	}
	else
	{
		ZoomEffect_.ZoomTransform_.scale_.x += ZoomEffect_.MaxZoomValue_ / TransitionTime_;
		ZoomEffect_.ZoomTransform_.scale_.y += ZoomEffect_.MaxZoomValue_ / TransitionTime_;
	}
}

void TransitionEffect::UpdateZoomOut()
{
	//���񂾂񏬂����Ȃ�G�t�F�N�g
	if (ZoomEffect_.ZoomTransform_.scale_.x <= 0.0f || 
		ZoomEffect_.ZoomTransform_.scale_.y <= 0.0f)
	{
		ZoomEffect_.ZoomTransform_.scale_.x = 0.0f;
		ZoomEffect_.ZoomTransform_.scale_.y = 0.0f;
	}
	else
	{
		ZoomEffect_.ZoomTransform_.scale_.x -= ZoomEffect_.MaxZoomValue_ / TransitionTime_;
		ZoomEffect_.ZoomTransform_.scale_.y -= ZoomEffect_.MaxZoomValue_ / TransitionTime_;
	}
}

void TransitionEffect::SetSCVTransitionEffect()
{
	CsvReader csvTransform;
	csvTransform.Load("CSVdata\\TransitionData.csv");

	std::vector<std::string> ParamNames = { "Fade" ,"Slide","Zoom"};
	std::vector<std::reference_wrapper<Transform>> EffectArray = {
		FadeEffect_.FadeTransform_, SlideEffect_.SlideTransform_, ZoomEffect_.ZoomTransform_
	};

	InitCSVTransformArray(csvTransform, ParamNames, EffectArray);

	CsvReader csvParam;
	csvParam.Load("CSVdata\\TransitionSomeData.csv");

	std::string Params = "ZoomParam";
	if (csvParam.IsGetParamName(Params))
	{
		std::vector<float> v = csvParam.GetParam(Params);
		ZoomEffect_.MaxZoomValue_ = v[i_maxzoomvalue];
	}
}

void TransitionEffect::SetTransitionAlpha()
{
	FadeEffect_.AlphaValue_ = Image::AlphaMin;
}

void TransitionEffect::SetTransitionZoom()
{
	ZoomEffect_.ZoomTransform_.scale_.x = ZoomEffect_.MaxZoomValue_;
	ZoomEffect_.ZoomTransform_.scale_.y = ZoomEffect_.MaxZoomValue_;
}
