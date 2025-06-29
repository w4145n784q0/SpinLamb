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

//�������i�f�t�H���g�j
HRESULT Text::Initialize()
{
	hPict_ = Image::Load(fileName_);
	assert(hPict_ >= 0);

	if (hPict_ < 0)
		return E_FAIL;

	return S_OK;
}

//�������i�I���W�i���̉摜�j
HRESULT Text::Initialize(const char* fileName, const unsigned int charWidth, const unsigned int charHeight, const unsigned int rowLength)
{
	strcpy_s(fileName_, fileName);
	width_ = charWidth;
	height_ = charHeight;
	rowLength_ = rowLength;
	return Initialize();
}


//�`��i������j
void Text::Draw(int x, int y, const char* str)
{
	//�\���ʒu�i����j���v�Z
	//Sprite�N���X�͒��S��(0,0)�A�E�オ(1,1)�Ƃ������W�����A�����̈����͍����(0,0)�A�h�b�g�P�ʂŎw�肵�Ă���
	float px, py;

	//�����͍��㌴�_�����A�X�v���C�g�͉�ʒ��������_�Ȃ̂ŁA��ʃT�C�Y�̔������炷
	px = (float)(x - Direct3D::screenWidth_ / (int)twoDevide_);
	py = (float)(-y + Direct3D::screenHeight_ / (int)twoDevide_);	//Y����+-���]

	//�X�v���C�g��Position��1���炷�Ɖ�ʃT�C�Y�̔��������̂ŁA�s�N�Z���P�ʂɕϊ�
	px /= (float)(Direct3D::screenWidth_ / twoDevide_);
	py /= (float)(Direct3D::screenHeight_ / twoDevide_);


	//�P�������\������
	for (int i = 0; str[i] != '\0'; i++)	//������̖����܂ŗ�����I���
	{
		//�\���������������A�摜�̉��Ԗڂɏ����Ă��邩�����߂�
		int id = str[i] - '!';		//�\�������������̃R�[�h����u!�v�̃R�[�h���������ƂŁA!��0�A"=1�A#��2����Ƃ����ԍ��ɂ���

		//�\���������������A�摜�̂ǂ��ɂ��邩�����߂�
		int x = id % rowLength_;	//�����牽�Ԗ�
		int y = id / rowLength_;	//�ォ�牽�Ԗ�

		//�\������ʒu
		Transform transform;
		transform.position_.x = px;
		transform.position_.y = py;
		Image::SetTransform(hPict_, transform);

		//�\������͈�
		Image::SetRect(hPict_, width_ * x, height_ * y, width_, height_);
		
		//�\��
		Image::Draw(hPict_);

		//���̈ʒu�ɂ��炷
		px += width_ / (float)(Direct3D::screenWidth_ / twoDevide_);
	}
}

//�`��i�����l�j
void Text::Draw(int x, int y, int value)
{
	//������ɕϊ�
	char str[256];
	sprintf_s(str, "%d", value);

	Draw(x, y, str);
}

//���
void Text::Release()
{
	Image::Release(hPict_);
}
