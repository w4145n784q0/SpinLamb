#include "TransitionEffect.h"
#include"Engine/Image.h"
#include"Engine/CsvReader.h"

namespace
{
	//csv読み込み時のインデックス
	enum ZoomIndex
	{
		i_maxzoomvalue = 0,
		i_max
	};

	//各画面遷移の演出用の構造体

	//フェードイン/アウト用
	struct FadeInOut
	{
		Transform FadeTransform;//フェードイン/アウト用トランスフォーム
		int AlphaValue = 0;//画像の透明度
	};
	FadeInOut FadeEffect;

	//スライドイン/アウト用
	struct SlideInOut
	{
		Transform SlideTransform;//スライドイン/アウト用トランスフォーム
	};
	SlideInOut SlideEffect;

	//ズームイン/アウト処理
	struct ZoomInOut
	{
		Transform ZoomTransform;//ズームイン/アウト用トランスフォーム
		float MaxZoomValue = 0.0f;//ズーム最大値
		float ZoomValue = 0.0f;//ズーム量
	};
	ZoomInOut ZoomEffect;

}

TransitionEffect::TransitionEffect(GameObject* parent)
	: GameObject(parent, "TransitionEffect"),hFadeBlack_(-1),hFadeWhite_(-1),
	hZoomSheep_(-1), EffectType_(NoneEffect),TransitionTime_(0)
{
}

TransitionEffect::~TransitionEffect()
{
}

void TransitionEffect::Initialize()
{
	//csvからパラメータ読み込み
	SetSCVTransitionEffect();

	//各画像の読み込み
	hFadeBlack_ = Image::Load("Image\\Transition\\fade_black.png");
	assert(hFadeBlack_ >= 0);

	hFadeWhite_ = Image::Load("Image\\Transition\\fade_white.png");
	assert(hFadeWhite_ >= 0);

	hZoomSheep_ = Image::Load("Image\\Transition\\SheepBlack.png");
	assert(hZoomSheep_ >= 0);
}

void TransitionEffect::Update()
{
	//EffectType_から行う画面遷移処理を指示
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
	default:
		break;
	}

}

void TransitionEffect::Draw()
{
	//EffectType_から行う描画を指示
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
	case TransitionEffect::S_ZoomIn:
	{
		Image::SetTransform(hZoomSheep_, ZoomEffect.ZoomTransform);
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
	//だんだん暗くなるエフェクト
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
	//だんだん明るくなるエフェクト
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
	//画像を左→右にスライド
	if (SlideEffect.SlideTransform.position_.x >= Image::Center)
	{
		SlideEffect.SlideTransform.position_.x = Image::Center;
	}
	else
	{
		SlideEffect.SlideTransform.position_.x += (Image::RightEdge - Image::LeftEdge) / TransitionTime_;
	}
}

void TransitionEffect::UpdateZoomIn()
{
	//だんだん大きくなるエフェクト
	if (ZoomEffect.ZoomValue >= ZoomEffect.MaxZoomValue)
	{
		ZoomEffect.ZoomTransform.scale_.x = ZoomEffect.MaxZoomValue;
		ZoomEffect.ZoomTransform.scale_.y = ZoomEffect.MaxZoomValue;
	}
	else
	{
		ZoomEffect.ZoomValue += ZoomEffect.MaxZoomValue / TransitionTime_;
		ZoomEffect.ZoomTransform.scale_.x += ZoomEffect.MaxZoomValue / TransitionTime_;
		ZoomEffect.ZoomTransform.scale_.y += ZoomEffect.MaxZoomValue / TransitionTime_;
	}
}

void TransitionEffect::UpdateZoomOut()
{
}

void TransitionEffect::SetSCVTransitionEffect()
{
	CsvReader csvTransform;
	csvTransform.Load("CSVdata\\TransitionData.csv");

	std::vector<std::string> ParamNames = { "Fade" ,"Slide" };
	std::vector<std::reference_wrapper<Transform>> EffectArray = {
		FadeEffect.FadeTransform,SlideEffect.SlideTransform
	};

	InitCSVTransformArray(csvTransform, ParamNames, EffectArray);

	CsvReader csvParam;
	csvParam.Load("CSVdata\\TransitionSomeData.csv");

	std::string Params = "ZoomParam";
	if (csvParam.IsGetParamName(Params))
	{
		std::vector<float> v = csvParam.GetParam(Params);
		ZoomEffect.MaxZoomValue = v[i_maxzoomvalue];
	}


}

void TransitionEffect::SetTransitionAlpha()
{
	FadeEffect.AlphaValue = Image::AlphaMin;
}

void TransitionEffect::ResetTransitionAlpha()
{
	FadeEffect.AlphaValue = 0;
}

void TransitionEffect::ResetTransitionZoom()
{
	//値が変わるものを破棄
	ZoomEffect.ZoomValue = 0.0f;
}
