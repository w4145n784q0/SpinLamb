#include "HUD.h"
#include"Engine/Image.h"
#include"BattleScene.h"
#include <unordered_map>
#include<functional>


//�`�摀��݈̂����N���X
namespace
{
	//���Ԃ̐����\�L�̃C���f�b�N�X
	enum NumberIndex
	{
		i_Number0 = 0,
		i_Number1,
		i_Number2,
		i_Number3,
		i_Number4,
		i_Number5,
		i_Number6,
		i_Number7,
		i_Number8,
		i_Number9,
		MaxNumberIndex,
	};

	//----------�摜�`��p�g�����X�t�H�[��----------
	//"�^�C�g���ɖ߂�܂�"
	Transform logo_backtitle;

	//"���K���[�h"
	Transform logo_practice;

	//Start!���S
	Transform logo_start;

	//Finish!���S
	Transform logo_Finish;

	//���ԕ\�L�\�̈�
	Transform TenTime;

	//���ԕ\�L��̈�
	Transform OneTime;

	//�}�b�v�̃g�����X�t�H�[��
	Transform Mapicon;

	//�v���C���[�A�C�R���̃g�����X�t�H�[��
	Transform Playericon;

	//�G�A�C�R���̃g�����X�t�H�[��
	Transform Enemyicon;

	//�i���o�[�n���h���̔z��
	std::array<int, MaxNumberIndex> ArrayHandle;

	int HandleIndexTen = 0;//�i���o�[�n���h���̓Y����(10�̈�)
	int HandleIndexOne = 0; //�i���o�[�n���h���̓Y����(1�̈�)

}

HUD::HUD(GameObject* parent)
	:GameObject(parent, "HUD"), hBackTitleLogo_(-1), hPracticeNow_(-1), hStart_(-1),
	hNumber0_(-1), hNumber1_(-1), hNumber2_(-1), hNumber3_(-1), hNumber4_(-1),
	hNumber5_(-1), hNumber6_(-1), hNumber7_(-1), hNumber8_(-1), hNumber9_(-1),
	hFinish_(-1), hMap_(-1), hPlayerIcon_(-1), hEnemyIcon_(-1),
	GameModeHUD_(Max), pGameTimer_(nullptr), pMiniMap_(nullptr), DrawMode_(S_None)

{
}

HUD::~HUD()
{
}

void HUD::Initialize()
{
	SetCSV();

	hBackTitleLogo_ = Image::Load("Image\\Practice\\BackTitleLogo.png");
	assert(hBackTitleLogo_ >= 0);

	hPracticeNow_ = Image::Load("Image\\Practice\\PracticeLogo.png");
	assert(hPracticeNow_ >= 0);

	hStart_ = Image::Load("Image\\Battle\\start_logo.png");
	assert(hStart_ >= 0);

	hFinish_ = Image::Load("Image\\Battle\\finish_logo.png");
	assert(hFinish_ >= 0);

	hNumber0_ = Image::Load("Image\\number\\number_0.png");
	assert(hNumber0_ >= 0);

	hNumber1_ = Image::Load("Image\\number\\number_1.png");
	assert(hNumber1_ >= 0);

	hNumber2_ = Image::Load("Image\\number\\number_2.png");
	assert(hNumber2_ >= 0);
	
	hNumber3_ = Image::Load("Image\\number\\number_3.png");
	assert(hNumber3_ >= 0);
	
	hNumber4_ = Image::Load("Image\\number\\number_4.png");
	assert(hNumber4_ >= 0);

	hNumber5_ = Image::Load("Image\\number\\number_5.png");
	assert(hNumber5_ >= 0);

	hNumber6_ = Image::Load("Image\\number\\number_6.png");
	assert(hNumber6_ >= 0);

	hNumber7_ = Image::Load("Image\\number\\number_7.png");
	assert(hNumber7_ >= 0);

	hNumber8_ = Image::Load("Image\\number\\number_8.png");
	assert(hNumber8_ >= 0);

	hNumber9_ = Image::Load("Image\\number\\number_9.png");
	assert(hNumber9_ >= 0);

	hMap_ = Image::Load("Image\\MiniMap\\minimap2.png");
	assert(hMap_ >= 0);

	hPlayerIcon_ = Image::Load("Image\\MiniMap\\blue_circle.png");
	assert(hPlayerIcon_ >= 0);

	hEnemyIcon_ = Image::Load("Image\\MiniMap\\red_circle.png");
	assert(hEnemyIcon_ >= 0);


	//�����摜�n���h���z���������
	ArrayHandle = { hNumber0_,hNumber1_,hNumber2_,hNumber3_,hNumber4_,
	hNumber5_,hNumber6_,hNumber7_,hNumber8_,hNumber9_ };

	//pMiniMap_ = (MiniMap*)FindObject("MiniMap");
}

void HUD::Update()
{
	
}

void HUD::Draw()
{
	//�e�I�u�W�F�N�g�ɔ킳�邱�Ƃ�h�����߁A���̊֐�����Ă�
	//HUD�N���X��Draw()�͍Ō�ɌĂԂ悤�ɐ݌v
	//�󋵂ɉ����Đ؂�ւ���������(�X�^�[�g/�v���C/�I�����̃��S�Ȃ�)�͂���switch���Ő؂�ւ���
	
	//�V�[���N���X����̎w���ɂ���ČĂԕ`��֐���ς���
	switch (DrawMode_)
	{
	case S_StartLogo:
		DrawStartLogo();
		break;
	case S_Timer:
		DrawTimer();
		break;
	case S_FinishLogo:
		DrawTimer();
		DrawFinishLogo();
		break;
	case S_Practice:
		DrawPracticeLogo();
		break;
	case S_None:
		break;
	default:
		break;
	}

	DrawMiniMap();
}

void HUD::Release()
{
}

void HUD::SetCSV()
{
	CsvReader csv;
	csv.Load("CSVdata\\HUDData.csv");

	std::string title = "backtitle";
	if (csv.IsGetParamName(title))
	{
		std::vector<float> v = csv.GetParam(title);
		SetTransformPRS(logo_backtitle, v);
	}

	std::string practice = "practice";
	if (csv.IsGetParamName(practice)) {
		std::vector<float> v = csv.GetParam(practice);
		SetTransformPRS(logo_practice, v);
	}

	std::string start = "start";
	if (csv.IsGetParamName(start))
	{
		std::vector<float> v = csv.GetParam(start);
		SetTransformPRS(logo_start, v);
	}

	std::string finish = "finish";
	if (csv.IsGetParamName(finish))
	{
		std::vector<float> v = csv.GetParam(finish);
		SetTransformPRS(logo_Finish, v);
	}

	std::string ten = "tentime";
	if (csv.IsGetParamName(ten))
	{
		std::vector<float> v = csv.GetParam(ten);
		SetTransformPRS(TenTime, v);
	}

	std::string one = "onetime";
	if (csv.IsGetParamName(one))
	{
		std::vector<float> v = csv.GetParam(one);
		SetTransformPRS(OneTime, v);
	}

	std::string map = "minimap";
	if (csv.IsGetParamName(map))
	{
		std::vector<float> v = csv.GetParam(map);
		SetTransformPRS(Mapicon, v);
	}

	std::string picon = "playericon";
	if (csv.IsGetParamName(picon))
	{
		std::vector<float> v = csv.GetParam(picon);
		SetTransformPRS(Playericon, v);
	}

	std::string eicon = "enemyicon";
	if (csv.IsGetParamName(eicon))
	{
		std::vector<float> v = csv.GetParam(eicon);
		SetTransformPRS(Enemyicon, v);
	}
}

void HUD::DrawPracticeLogo()
{
#ifdef _DEBUG
	if (ImGui::TreeNode("PracticeLogo"))
	{
		ImGui::SliderFloat("positionX", &logo_backtitle.position_.x, -1.0f, 1.0f);
		ImGui::SliderFloat("positionY", &logo_backtitle.position_.y, -1.0f, 1.0f);

		ImGui::SliderFloat("positionX", &logo_practice.position_.x, -1.0f, 1.0f);
		ImGui::SliderFloat("positionY", &logo_practice.position_.y, -1.0f, 1.0f);
		ImGui::TreePop();
	}
#endif


	Image::SetTransform(hBackTitleLogo_, logo_backtitle);
	Image::Draw(hBackTitleLogo_);

	Image::SetTransform(hPracticeNow_, logo_practice);
	Image::Draw(hPracticeNow_);
}

void HUD::DrawTimer()
{
	if(pGameTimer_ != nullptr)
	{
		HandleIndexTen = pGameTimer_->GetTimeTen();
		HandleIndexOne = pGameTimer_->GetTimeOne();

		Image::SetTransform(ArrayHandle[HandleIndexTen], TenTime);
		Image::Draw(ArrayHandle[HandleIndexTen]);
		Image::SetTransform(ArrayHandle[HandleIndexOne], OneTime);
		Image::Draw(ArrayHandle[HandleIndexOne]);
	}
}

void HUD::DrawStartLogo()
{
	Image::SetTransform(hStart_, logo_start);
	Image::Draw(hStart_);
}

void HUD::DrawFinishLogo()
{
	Image::SetTransform(hFinish_, logo_Finish);
	Image::Draw(hFinish_);
}

void HUD::DrawMiniMap()
{
#ifdef _DEBUG
	if (ImGui::TreeNode("MiniMap"))
	{
		ImGui::SliderFloat("positionX", &Mapicon.position_.x, -1.0f, 1.0f);
		ImGui::SliderFloat("positionY", &Mapicon.position_.y, -1.0f, 1.0f);
		ImGui::TreePop();
	}
#endif

	if (pMiniMap_ != nullptr) 
	{
		Playericon.position_ = pMiniMap_->GetPlayerPos();
		Enemyicon.position_ = pMiniMap_->GetEnemyPos();
	}

	Image::SetTransform(hMap_, Mapicon);
	Image::Draw(hMap_);
	//Image::SetAlpha(hMap_, MapAlpha);

	Image::SetTransform(hPlayerIcon_, Playericon);
	Image::Draw(hPlayerIcon_);

	Image::SetTransform(hEnemyIcon_, Enemyicon);
	Image::Draw(hEnemyIcon_);
}
