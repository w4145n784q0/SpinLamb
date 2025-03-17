#include "Logo.h"
#include"Engine/Image.h"

Logo::Logo(GameObject* parent)
	:GameObject(parent,"Logo"),hFinish_(-1),LogoState(S_FINISH)
{
}

void Logo::Initialize()
{
	hFinish_ = Image::Load("finish.png");
	assert(hFinish_ >= 0);
	trans_finish_.position_ = { 0,0,0 };
}

void Logo::Update()
{
}

void Logo::Draw()
{
	switch (LogoState)
	{
	case Logo::S_FINISH:
	{
		Image::SetTransform(hFinish_, trans_finish_);
		Image::Draw(hFinish_);
	}
		break;
	case Logo::S_MAX:
		break;
	default:
		break;
	}

}

void Logo::Release()
{
}
