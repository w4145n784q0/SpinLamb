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
	//csvからパラメータ読み込み
	SetCSVTransitionEffect();

	//同じディレクトリ内からのパスは省略
	//パスの一部を文字列にし、結合させる
	std::string transition = "Image\\Transition\\";

	//各画像の読み込み
	hFadeBlack_ = Image::Load(transition + "FadeBlack.png");
	assert(hFadeBlack_ >= 0);

	hFadeWhite_ = Image::Load(transition + "FadeWhite.png");
	assert(hFadeWhite_ >= 0);

	hZoomSheep_ = Image::Load(transition + "SheepBlack.png");
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
	case TransitionEffect::S_ZoomOut:
		UpdateZoomOut();
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
		//黒いフェードイン/アウト描画
		Image::SetAlphaAndDraw(hFadeBlack_, FadeEffect_.AlphaValue_, FadeEffect_.FadeTransform_);
	}
		break;
	case TransitionEffect::S_FadeOutWhite:
	case TransitionEffect::S_FadeInWhite:
	{
		//白いフェードイン/アウト描画
		Image::SetAlphaAndDraw(hFadeWhite_, FadeEffect_.AlphaValue_, FadeEffect_.FadeTransform_);
	}
		break;
	case TransitionEffect::S_SlideInLTR:
	{
		//左から右にスライドインする描画
		Image::SetAndDraw(hFadeBlack_, SlideEffect_.SlideTransform_);
	}
		break;
	case TransitionEffect::S_ZoomIn:
	case TransitionEffect::S_ZoomOut:
	{
		//ズームイン/アウトする描画
		Image::SetAndDraw(hZoomSheep_, ZoomEffect_.ZoomTransform_);
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
	if (FadeEffect_.AlphaValue_ >= Image::AlphaMin)
	{
		FadeEffect_.AlphaValue_ = Image::AlphaMin;
	}
	else
	{
		//透明度に (透過度の最小値 / 遷移時間) を加算して透過度を上げていき、画面を隠していく
		FadeEffect_.AlphaValue_ += Image::AlphaMin / TransitionTime_;
	}

}

void TransitionEffect::UpdateFadeIn()
{
	//だんだん明るくなるエフェクト
	if (FadeEffect_.AlphaValue_ <= 0)
	{
		FadeEffect_.AlphaValue_ = 0;
	}
	else
	{
		//透明度に (透過度の最小値 / 遷移時間) を減算して透過度を下げていき、画面を見せていく
		FadeEffect_.AlphaValue_ -= Image::AlphaMin / TransitionTime_;
	}
}

void TransitionEffect::UpdateSlideInLTR()
{
	//画像を左→右にスライド
	if (SlideEffect_.SlideTransform_.position_.x >= Image::Center)
	{
		SlideEffect_.SlideTransform_.position_.x = Image::Center;
	}
	else
	{
		//スライド画像位置に (右端から左端を引いた値 / 遷移時間) を加算していく
		SlideEffect_.SlideTransform_.position_.x += (Image::RightEdge - Image::LeftEdge) / TransitionTime_;
	}
}

void TransitionEffect::UpdateZoomIn()
{
	//だんだん大きくなるエフェクト
	if (ZoomEffect_.ZoomTransform_.scale_.x >= ZoomEffect_.MaxZoomValue_ ||
		ZoomEffect_.ZoomTransform_.scale_.y >= ZoomEffect_.MaxZoomValue_)
	{
		ZoomEffect_.ZoomTransform_.scale_.x = ZoomEffect_.MaxZoomValue_;
		ZoomEffect_.ZoomTransform_.scale_.y = ZoomEffect_.MaxZoomValue_;
	}
	else
	{
		//ズーム量に (最大値 / 遷移時間) を加算してだんだん大きくする
		ZoomEffect_.ZoomTransform_.scale_.x += ZoomEffect_.MaxZoomValue_ / TransitionTime_;
		ZoomEffect_.ZoomTransform_.scale_.y += ZoomEffect_.MaxZoomValue_ / TransitionTime_;
	}
}

void TransitionEffect::UpdateZoomOut()
{
	//だんだん小さくなるエフェクト
	if (ZoomEffect_.ZoomTransform_.scale_.x <= 0.0f || 
		ZoomEffect_.ZoomTransform_.scale_.y <= 0.0f)
	{
		ZoomEffect_.ZoomTransform_.scale_.x = 0.0f;
		ZoomEffect_.ZoomTransform_.scale_.y = 0.0f;
	}
	else
	{
		//ズーム量に (最大値 / 遷移時間) 減算してだんだん小さくする
		ZoomEffect_.ZoomTransform_.scale_.x -= ZoomEffect_.MaxZoomValue_ / TransitionTime_;
		ZoomEffect_.ZoomTransform_.scale_.y -= ZoomEffect_.MaxZoomValue_ / TransitionTime_;
	}
}

void TransitionEffect::SetCSVTransitionEffect()
{
	//csvファイルを読み込む
	CsvReader csvTransform;
	csvTransform.Load("CSVdata\\EffectData\\TransitionData.csv");

	//csvファイル(TransitionData.csv)の各0列目の文字列の配列
	std::vector<std::string> ParamNames = { "Fade" ,"Slide","Zoom" };

	//各トランスフォームを配列に入れる
	std::vector<std::reference_wrapper<Transform>> EffectArray = {
		FadeEffect_.FadeTransform_, SlideEffect_.SlideTransform_, ZoomEffect_.ZoomTransform_
	};

	//まとめて初期化
	InitCSVTransformArray(csvTransform, ParamNames, EffectArray);

	//csvファイルを読み込む
	CsvReader csvParam;
	csvParam.Load("CSVdata\\EffectData\\TransitionSomeData.csv");

	//csvファイル(TransitionSomeData)の0列目の文字列を取得
	std::string SomeParams = "ZoomParam";

	//0列目の文字列を渡し、その行のパラメータを取得
	std::vector<float> transitionData = GetCSVReadData(csvParam, SomeParams);

	//初期化の順番はcsvの各行の順番に合わせる
	//vの添え字はnamespaceで宣言した列挙型を使用
	ZoomEffect_.MaxZoomValue_ = transitionData[i_maxzoomvalue];
	
}