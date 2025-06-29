#include <stdlib.h>
#include "Direct3D.h"
#include "Text.h"

Text::Text() : hPict_(-1), width_(0), height_(0), fileName_("char.png"), rowLength_(0),twoDevide_(0.0f)
{
	width_ = GetPrivateProfileInt("TEXT", "Width", 0, ".\\TextInit.ini");
	height_ = GetPrivateProfileInt("TEXT", "Height", 0, ".\\TextInit.ini");
	rowLength_ = GetPrivateProfileInt("TEXT", "rowLength", 0, ".\\TextInit.ini");
	twoDevide_ = GetPrivateProfileInt("TEXT", "twoDevide", 0, ".\\TextInit.ini");
}

Text::~Text()
{
}

//初期化（デフォルト）
HRESULT Text::Initialize()
{
	hPict_ = Image::Load(fileName_);
	assert(hPict_ >= 0);

	if (hPict_ < 0)
		return E_FAIL;

	return S_OK;
}

//初期化（オリジナルの画像）
HRESULT Text::Initialize(const char* fileName, const unsigned int charWidth, const unsigned int charHeight, const unsigned int rowLength)
{
	strcpy_s(fileName_, fileName);
	width_ = charWidth;
	height_ = charHeight;
	rowLength_ = rowLength;
	return Initialize();
}


//描画（文字列）
void Text::Draw(int x, int y, const char* str)
{
	//表示位置（左上）を計算
	//Spriteクラスは中心が(0,0)、右上が(1,1)という座標だが、ここの引数は左上を(0,0)、ドット単位で指定している
	float px, py;

	//引数は左上原点だが、スプライトは画面中央が原点なので、画面サイズの半分ずらす
	px = (float)(x - Direct3D::screenWidth_ / (int)twoDevide_);
	py = (float)(-y + Direct3D::screenHeight_ / (int)twoDevide_);	//Y軸は+-反転

	//スプライトはPositionを1ずらすと画面サイズの半分ずれるので、ピクセル単位に変換
	px /= (float)(Direct3D::screenWidth_ / twoDevide_);
	py /= (float)(Direct3D::screenHeight_ / twoDevide_);


	//１文字ずつ表示する
	for (int i = 0; str[i] != '\0'; i++)	//文字列の末尾まで来たら終わり
	{
		//表示したい文字が、画像の何番目に書いてあるかを求める
		int id = str[i] - '!';		//表示したい文字のコードから「!」のコードを引くことで、!＝0、"=1、#＝2･･･という番号にする

		//表示したい文字が、画像のどこにあるかを求める
		int x = id % rowLength_;	//左から何番目
		int y = id / rowLength_;	//上から何番目

		//表示する位置
		Transform transform;
		transform.position_.x = px;
		transform.position_.y = py;
		Image::SetTransform(hPict_, transform);

		//表示する範囲
		Image::SetRect(hPict_, width_ * x, height_ * y, width_, height_);
		
		//表示
		Image::Draw(hPict_);

		//次の位置にずらす
		px += width_ / (float)(Direct3D::screenWidth_ / twoDevide_);
	}
}

//描画（整数値）
void Text::Draw(int x, int y, int value)
{
	//文字列に変換
	char str[256];
	sprintf_s(str, "%d", value);

	Draw(x, y, str);
}

//解放
void Text::Release()
{
	Image::Release(hPict_);
}
