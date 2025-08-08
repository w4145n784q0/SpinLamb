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
	SetCSVTransitionEffect();

	//�����f�B���N�g��������̃p�X�͏ȗ�
	//�p�X�̈ꕔ�𕶎���ɂ��A����������
	std::string transition = "Image\\Transition\\";

	//�e�摜�̓ǂݍ���
	hFadeBlack_ = Image::Load(transition + "FadeBlack.png");
	assert(hFadeBlack_ >= 0);

	hFadeWhite_ = Image::Load(transition + "FadeWhite.png");
	assert(hFadeWhite_ >= 0);

	hZoomSheep_ = Image::Load(transition + "SheepBlack.png");
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
		//�����t�F�[�h�C��/�A�E�g�`��
		Image::SetAlphaAndDraw(hFadeBlack_, FadeEffect_.AlphaValue_, FadeEffect_.FadeTransform_);
	}
		break;
	case TransitionEffect::S_FadeOutWhite:
	case TransitionEffect::S_FadeInWhite:
	{
		//�����t�F�[�h�C��/�A�E�g�`��
		Image::SetAlphaAndDraw(hFadeWhite_, FadeEffect_.AlphaValue_, FadeEffect_.FadeTransform_);
	}
		break;
	case TransitionEffect::S_SlideInLTR:
	{
		//������E�ɃX���C�h�C������`��
		Image::SetAndDraw(hFadeBlack_, SlideEffect_.SlideTransform_);
	}
		break;
	case TransitionEffect::S_ZoomIn:
	case TransitionEffect::S_ZoomOut:
	{
		//�Y�[���C��/�A�E�g����`��
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
	//���񂾂�Â��Ȃ�G�t�F�N�g
	if (FadeEffect_.AlphaValue_ >= Image::AlphaMin)
	{
		FadeEffect_.AlphaValue_ = Image::AlphaMin;
	}
	else
	{
		//�����x�� (���ߓx�̍ŏ��l / �J�ڎ���) �����Z���ē��ߓx���グ�Ă����A��ʂ��B���Ă���
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
		//�����x�� (���ߓx�̍ŏ��l / �J�ڎ���) �����Z���ē��ߓx�������Ă����A��ʂ������Ă���
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
		//�X���C�h�摜�ʒu�� (�E�[���獶�[���������l / �J�ڎ���) �����Z���Ă���
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
		//�Y�[���ʂ� (�ő�l / �J�ڎ���) �����Z���Ă��񂾂�傫������
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
		//�Y�[���ʂ� (�ő�l / �J�ڎ���) ���Z���Ă��񂾂񏬂�������
		ZoomEffect_.ZoomTransform_.scale_.x -= ZoomEffect_.MaxZoomValue_ / TransitionTime_;
		ZoomEffect_.ZoomTransform_.scale_.y -= ZoomEffect_.MaxZoomValue_ / TransitionTime_;
	}
}

void TransitionEffect::SetCSVTransitionEffect()
{
	//csv�t�@�C����ǂݍ���
	CsvReader csvTransform;
	csvTransform.Load("CSVdata\\EffectData\\TransitionData.csv");

	//csv�t�@�C��(TransitionData.csv)�̊e0��ڂ̕�����̔z��
	std::vector<std::string> ParamNames = { "Fade" ,"Slide","Zoom" };

	//�e�g�����X�t�H�[����z��ɓ����
	std::vector<std::reference_wrapper<Transform>> EffectArray = {
		FadeEffect_.FadeTransform_, SlideEffect_.SlideTransform_, ZoomEffect_.ZoomTransform_
	};

	//�܂Ƃ߂ď�����
	InitCSVTransformArray(csvTransform, ParamNames, EffectArray);

	//csv�t�@�C����ǂݍ���
	CsvReader csvParam;
	csvParam.Load("CSVdata\\EffectData\\TransitionSomeData.csv");

	//csv�t�@�C��(TransitionSomeData)��0��ڂ̕�������擾
	std::string SomeParams = "ZoomParam";

	//0��ڂ̕������n���A���̍s�̃p�����[�^���擾
	std::vector<float> transitionData = GetCSVReadData(csvParam, SomeParams);

	//�������̏��Ԃ�csv�̊e�s�̏��Ԃɍ��킹��
	//v�̓Y������namespace�Ő錾�����񋓌^���g�p
	ZoomEffect_.MaxZoomValue_ = transitionData[i_maxzoomvalue];
	
}