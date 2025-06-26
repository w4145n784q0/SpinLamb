#include "TransitionEffect.h"
#include"Engine/Image.h"
#include"Engine/CsvReader.h"

namespace
{
	//�e��ʑJ�ڂ̉��o�p�̍\����

	//�t�F�[�h�C��/�A�E�g�p
	struct FadeInOut
	{
		Transform FadeTransform;//�t�F�[�h�C��/�A�E�g�p�g�����X�t�H�[��
		int AlphaValue = 0;//�摜�̓����x
	};
	FadeInOut FadeEffect;

	//�X���C�h�C��/�A�E�g�p
	struct SlideInOut
	{
		Transform SlideTransform;//�X���C�h�C��/�A�E�g�p�g�����X�t�H�[��
	};
	SlideInOut SlideEffect;

}

TransitionEffect::TransitionEffect(GameObject* parent)
	: GameObject(parent, "TransitionEffect"),hFadeBlack_(-1),hFadeWhite_(-1),
	EffectType_(NoneEffect),TransitionTime_(0)
{
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
		Image::SetTransform(hFadeBlack_, FadeEffect.FadeTransform);
		Image::SetAlpha(hFadeBlack_, FadeEffect.AlphaValue);
		Image::Draw(hFadeBlack_);
	}
		break;
	case TransitionEffect::S_FadeOutWhite:
	case TransitionEffect::S_FadeInWhite:
	{
		Image::SetTransform(hFadeWhite_, FadeEffect.FadeTransform);
		Image::SetAlpha(hFadeWhite_, FadeEffect.AlphaValue);
		Image::Draw(hFadeWhite_);
	}
		break;
	case TransitionEffect::S_SlideInLTR:
	{
		Image::SetTransform(hFadeBlack_, SlideEffect.SlideTransform);
		Image::Draw(hFadeBlack_);
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

	std::vector<std::string> ParamNames = { "Fade" ,"Slide" };
	std::vector<std::reference_wrapper<Transform>> EffectArray = {
		FadeEffect.FadeTransform,SlideEffect.SlideTransform
	};

	InitCSVTransformArray(csv, ParamNames, EffectArray);

}

void TransitionEffect::SetTransitionAlpha()
{
	FadeEffect.AlphaValue = Image::AlphaMin;
}

void TransitionEffect::ResetTransitionAlpha()
{
	FadeEffect.AlphaValue = 0;
}
