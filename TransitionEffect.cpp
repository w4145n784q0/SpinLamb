#include "TransitionEffect.h"
#include"Engine/Image.h"
#include"Engine/CsvReader.h"

namespace
{
	//�e��ʑJ�ڂ̉��o�p�̍\����

	//�t�F�[�h�C��/�A�E�g�p
	struct FadeInOut
	{
		Transform FadeTransform;
		int AlphaValue = 0;
	};
	FadeInOut FadeEffect;

	//�X���C�h�C��/�A�E�g�p
	struct SlideInOut
	{
		Transform SlideTransform;
	};
	SlideInOut SlideEffect;

}

TransitionEffect::TransitionEffect(GameObject* parent)
	: GameObject(parent, "TransitionEffect"),hFade_(-1),EffectType_(NoneEffect),
	TransitionTime_(0)
{
}

TransitionEffect::~TransitionEffect()
{
}

void TransitionEffect::Initialize()
{
	SetSCVTransitionEffect();

	hFade_ = Image::Load("Image\\Battle\\fade.png");
	assert(hFade_ >= 0);
}

void TransitionEffect::Update()
{
	switch (EffectType_)
	{
	case TransitionEffect::S_FadeOut:
		UpdateFadeOut();
		break;
	case TransitionEffect::S_FadeIn:
		UpdateFadeIn();
		break;
	case TransitionEffect::S_SlideInLTR:
		UpdateSlideInLTR();
		break;
	default:
		break;
	}

}

void TransitionEffect::Draw()
{
	switch (EffectType_)
	{
	case TransitionEffect::S_FadeOut:
	case TransitionEffect::S_FadeIn:
	{
		Image::SetTransform(hFade_, FadeEffect.FadeTransform);
		Image::SetAlpha(hFade_, FadeEffect.AlphaValue);
		Image::Draw(hFade_);
	}
		break;
	case TransitionEffect::S_SlideInLTR:
	{
		Image::SetTransform(hFade_, SlideEffect.SlideTransform);
		Image::Draw(hFade_);
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
	if (FadeEffect.AlphaValue >= Image::AlphaMin)
	{
		FadeEffect.AlphaValue = Image::AlphaMin;
	}
	else
	{
		FadeEffect.AlphaValue += Image::AlphaMin / TransitionTime_;
	}

}

void TransitionEffect::UpdateFadeIn()
{
	//���񂾂񖾂邭�Ȃ�G�t�F�N�g

	if (FadeEffect.AlphaValue <= 0)
	{
		FadeEffect.AlphaValue = 0;
	}
	else
	{
		FadeEffect.AlphaValue -= Image::AlphaMin / TransitionTime_;
	}
}

void TransitionEffect::UpdateSlideInLTR()
{
	//�摜�������E�ɃX���C�h
	if (SlideEffect.SlideTransform.position_.x >= Image::Center)
	{
		SlideEffect.SlideTransform.position_.x = Image::Center;
	}
	else
	{
		SlideEffect.SlideTransform.position_.x += (Image::RightEdge - Image::LeftEdge) / TransitionTime_;
	}
}

void TransitionEffect::SetSCVTransitionEffect()
{
	CsvReader csv;
	csv.Load("CSVdata\\TransitionData.csv");

	std::string fade = "Fade";
	if (csv.IsGetParamName(fade))
	{
		std::vector<float> v = csv.GetParam(fade);
		SetTransformPRS(FadeEffect.FadeTransform, v);
	}

	std::string slide = "Slide";
	if (csv.IsGetParamName(slide))
	{
		std::vector<float> v = csv.GetParam(slide);
		SetTransformPRS(SlideEffect.SlideTransform, v);
	}
}

void TransitionEffect::SetTransitionAlpha()
{
	FadeEffect.AlphaValue = Image::AlphaMin;
}
