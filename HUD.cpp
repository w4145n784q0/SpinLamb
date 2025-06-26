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

	//csv�ǂݍ��ݎ��̃C���f�b�N�X
	enum EasingIndex
	{
		i_logochange = 0,
		i_maxscale,
		i_max
	};

	//���̏�Ԃ͑��N���X���瑀��ł��Ȃ��悤��cpp�t�@�C���ɋL�q
	enum DrawStartMode
	{
		start_ready = 0,
		start_go,
		start_max
	};
	DrawStartMode DrawStart;

	//----------�摜�`��p�g�����X�t�H�[��----------
	//"�^�C�g���ɖ߂�܂�"
	Transform logo_backtitle;

	//"���K���[�h"
	Transform logo_practice;

	//�Q�[���ȈՐ���
	Transform logo_explanation;

	//Ready,Go!���S
	Transform logo_start;

	//Finish!���S
	Transform logo_Finish;

	//���ԕ\�L�\�̈�
	Transform TenTime;

	//���ԕ\�L��̈�
	Transform OneTime;

	//�}�b�v�̃g�����X�t�H�[��
	Transform MapIcon;

	//�v���C���[�A�C�R���̃g�����X�t�H�[��
	Transform PlayerIcon;

	//�G�A�C�R���̃g�����X�t�H�[��
	Transform EnemyIcon;

	//�X�R�A�\���ʒu�̃g�����X�t�H�[��
	Transform pScore_ten;
	Transform pScore_one;
	Transform eScore_ten;
	Transform eScore_one;

	//�摜�p�g�����X�t�H�[����z��ɓ����
	//�������̍ۂɎg�p����
	std::vector<std::reference_wrapper<Transform>> ImageArray = {
	logo_backtitle,logo_practice,logo_explanation,logo_start,
	logo_Finish ,TenTime ,OneTime, MapIcon,PlayerIcon,EnemyIcon,
	pScore_ten,pScore_one,eScore_ten, eScore_one 
	};

	//�i���o�[�n���h���̔z��
	std::array<int, MaxNumberIndex> ArrayHandle;

	int TimeIndexTen = 0;//���ԕ\�L�̃i���o�[�n���h���̓Y����(10�̈�)
	int TimeIndexOne = 0; //���ԕ\�L�̃i���o�[�n���h���̓Y����(1�̈�)
	
	int pScoreIndexTen = 0;
	int pScoreIndexOne = 0;
	int eScoreIndexTen = 0;
	int eScoreIndexOne = 0;

	float EasingCount = 0;//�C�[�W���O�g�p���̃J�E���^�[
	float LogoChangeCount = 0;//���S�ύX�܂ł̃J�E���^�[

	float LogoChange = 0.0f;//startlogo�ύX�̍ہA�ǂ̃^�C�~���O�Ő؂�ւ��邩
	float MaxScale = 0.0f;//Go! �̃��S�̍ő�g�嗦
}

HUD::HUD(GameObject* parent)
	:GameObject(parent, "HUD"), hBackTitleLogo_(-1), hPracticeNow_(-1), hGameExplanation_(-1),
	hStart_(-1),hReady_(-1),hGo_(-1),
	hNumber0_(-1), hNumber1_(-1), hNumber2_(-1), hNumber3_(-1), hNumber4_(-1),
	hNumber5_(-1), hNumber6_(-1), hNumber7_(-1), hNumber8_(-1), hNumber9_(-1),
	hFinish_(-1), hMap_(-1), hPlayerIcon_(-1), hEnemyIcon_(-1),
	GameModeHUD_(Max), pGameTimer_(nullptr), pMiniMap_(nullptr), DrawMode_(S_None),
	PlayerScore_(0),EnemyScore_(0),ReadyTimer_(0)

{
}

HUD::~HUD()
{
}

void HUD::Initialize()
{
	DrawStart = start_ready;

	//csv����p�����[�^�ǂݍ���
	SetHUDCSV();

	//csv����p�����[�^�ǂݍ���
	Easing::SetSCVEasing();

	//�e�摜�̓ǂݍ���
	hBackTitleLogo_ = Image::Load("Image\\Practice\\BackTitleLogo.png");
	assert(hBackTitleLogo_ >= 0);

	hPracticeNow_ = Image::Load("Image\\Practice\\PracticeLogo.png");
	assert(hPracticeNow_ >= 0);

	hGameExplanation_ = Image::Load("Image\\Battle\\rule2.png");
	assert(hGameExplanation_ >= 0);

	hStart_ = Image::Load("Image\\Battle\\start_logo.png");
	assert(hStart_ >= 0);

	hReady_ = Image::Load("Image\\Battle\\ready_logo.png");
	assert(hReady_ >= 0);

	hGo_ = Image::Load("Image\\Battle\\go_logo.png");
	assert(hGo_ >= 0);

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
}

void HUD::Update()
{
	/*switch (DrawMode_)
	{
	case S_BeforeStart:
		break;
	case S_Ready:
		break;
	case S_Playing:
		break;
	case S_Finish:
		break;
	case S_Practice:
		break;
	case S_None:
		break;
	default:
		break;
	}*/
}

void HUD::Draw()
{
	//�e�I�u�W�F�N�g�ɔ킳�邱�Ƃ�h�����߁A���̊֐�����Ă�
	//HUD�N���X��Draw()�͍Ō�ɌĂԂ悤�ɐ݌v
	//�󋵂ɉ����Đ؂�ւ���������(�X�^�[�g/�v���C/�I�����̃��S�Ȃ�)�͂���switch���Ő؂�ւ���
	
	//�V�[���N���X����̎w���ɂ���ČĂԕ`��֐���ς���
	switch (DrawMode_)
	{
	case S_BeforeStart:
	{
		DrawExplanation();
	}
		break;
	case S_Ready:
	{
		DrawScore();
		DrawStartLogo();
	}
		break;
	case S_Playing:
	{
		DrawScore();
		DrawTimer();
	}
		break;
	case S_Finish:
	{
		DrawTimer();
		DrawFinishLogo();
		DrawScore();
	}
		break;
	case S_Practice:
		DrawPracticeLogo();
		break;
	case S_None:
		break;
	default:
		break;
	}

	//��ɕ\��������̂�switch���̊O�ŋL�q
	DrawMiniMap();
}

void HUD::Release()
{
}

void HUD::SetHUDCSV()
{
	CsvReader csvTransform;
	csvTransform.Load("CSVdata\\HUDData.csv");

	std::vector<std::string> ParamNames = {
		"backtitle","practice","explanation","start","finish",
		"tentime","onetime","minimap","playericon", "enemyicon",
		"playerscoreten","playerscoreone","enemyscoreten","enemyscoreone",
	};

	InitCSVTransformArray(csvTransform, ParamNames, ImageArray);


	CsvReader csveasing;
	csveasing.Load("CSVdata\\HUDSomeData.csv");

	//InitCSVParameter(csveasing, "Easing", std::ref(LogoChange), std::ref(MaxScale) );

	std::string easing = "Easing";
	if (csveasing.IsGetParamName(easing))
	{
		std::vector<float> v = csveasing.GetParam(easing);
		LogoChange = v[i_logochange];
		MaxScale = v[i_maxscale];
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
		TimeIndexTen = pGameTimer_->GetTimeTen();
		TimeIndexOne = pGameTimer_->GetTimeOne();

		Image::SetTransform(ArrayHandle[TimeIndexTen], TenTime);
		Image::Draw(ArrayHandle[TimeIndexTen]);
		Image::SetTransform(ArrayHandle[TimeIndexOne], OneTime);
		Image::Draw(ArrayHandle[TimeIndexOne]);
	}
}

void HUD::DrawExplanation()
{
	Image::SetTransform(hGameExplanation_, logo_explanation);
	Image::Draw(hGameExplanation_);
}

void HUD::DrawStartLogo()
{
	switch (DrawStart)
	{
	case start_ready:
		DrawReady();
		break;
	case start_go:
		DrawGo();
		break;
	default:
		break;
	}

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
		ImGui::SliderFloat("positionX", &MapIcon.position_.x, -1.0f, 1.0f);
		ImGui::SliderFloat("positionY", &MapIcon.position_.y, -1.0f, 1.0f);
		ImGui::TreePop();
	}
#endif

	if (pMiniMap_ != nullptr) 
	{
		PlayerIcon.position_ = pMiniMap_->GetPlayerPos();
		EnemyIcon.position_ = pMiniMap_->GetEnemyPos();
	}

	Image::SetTransform(hMap_, MapIcon);
	Image::Draw(hMap_);

	Image::SetTransform(hPlayerIcon_, PlayerIcon);
	Image::Draw(hPlayerIcon_);

	Image::SetTransform(hEnemyIcon_, EnemyIcon);
	Image::Draw(hEnemyIcon_);
}

void HUD::DrawScore()
{
	pScoreIndexTen = PlayerScore_ / TenDivision;
	pScoreIndexOne = PlayerScore_ % TenDivision;
	eScoreIndexTen = EnemyScore_ / TenDivision;
	eScoreIndexOne = EnemyScore_ % TenDivision;

	Image::SetTransform(ArrayHandle[pScoreIndexTen], pScore_ten);
	Image::Draw(ArrayHandle[pScoreIndexTen]);
	Image::SetTransform(ArrayHandle[pScoreIndexOne], pScore_one);
	Image::Draw(ArrayHandle[pScoreIndexOne]);

	Image::SetTransform(ArrayHandle[eScoreIndexTen], eScore_ten);
	Image::Draw(ArrayHandle[eScoreIndexTen]);
	Image::SetTransform(ArrayHandle[eScoreIndexOne], eScore_one);
	Image::Draw(ArrayHandle[eScoreIndexOne]);
}

void HUD::DrawReady()
{
	if (++LogoChangeCount < ReadyTimer_)
	{
		Image::SetTransform(hReady_, logo_start);
		Image::Draw(hReady_);
	}
	else
	{
		DrawStart = start_go;
	}
}

void HUD::DrawGo()
{
	EasingCount += DeltaTime;
	float scale = static_cast<float>(Easing::calculateScale(MaxScale, EasingCount));
	logo_start.scale_.x = scale;
	logo_start.scale_.y = scale;

	Image::SetTransform(hGo_, logo_start);
	Image::Draw(hGo_);
}
