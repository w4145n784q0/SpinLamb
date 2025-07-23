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
	Transform PlayerScoreTen;
	Transform PlayerScoreOne;
	Transform EnemyScoreTen;
	Transform EnemyScoreOne;

	//�摜�p�g�����X�t�H�[����z��ɓ����
	//�������̍ۂɎg�p����
	std::vector<std::reference_wrapper<Transform>> ImageArray = {
	logo_backtitle,logo_practice,logo_explanation,logo_start,
	logo_Finish ,TenTime ,OneTime, MapIcon,PlayerIcon,EnemyIcon,
	PlayerScoreTen,PlayerScoreOne,EnemyScoreTen, EnemyScoreOne
	};

	//�i���o�[�n���h���̔z��
	std::array<int, MaxNumberIndex> ArrayHandle;

	//���ԕ\�L�̃i���o�[�n���h���̓Y����(10�̈�)
	int TimeIndexTen = 0;

	//���ԕ\�L�̃i���o�[�n���h���̓Y����(1�̈�)
	int TimeIndexOne = 0; 
	
	//�i���o�[�n���h���̓Y���� Player,Enemy���ꂼ��̏\�̈ʁA��̈�
	int pScoreIndexTen = 0;
	int pScoreIndexOne = 0;
	int eScoreIndexTen = 0;
	int eScoreIndexOne = 0;

	//���S�ύX�܂ł̃J�E���^�[
	float LogoChangeCount = 0;

	//startlogo�ύX�̍ہA�ǂ̃^�C�~���O�Ő؂�ւ��邩(�萔)
	float LogoChange = 0.0f;

	//Go! �̃��S�̍ő�g�嗦(�萔)
	float MaxScale = 0.0f;
}

HUD::HUD(GameObject* parent)
	:GameObject(parent, "HUD"), hBackTitleLogo_(-1), hPracticeNow_(-1), hGameExplanation_(-1),
	hStart_(-1),hReady_(-1),hGo_(-1),
	hNumber0_(-1), hNumber1_(-1), hNumber2_(-1), hNumber3_(-1), hNumber4_(-1),
	hNumber5_(-1), hNumber6_(-1), hNumber7_(-1), hNumber8_(-1), hNumber9_(-1),
	hFinish_(-1), hMap_(-1), hFirstIcon_(-1), hSecondIcon_(-1),
	GameModeHUD_(Max), pGameTimer_(nullptr), pMiniMap_(nullptr), DrawMode_(S_None),
	FirstScore_(0),SecondScore_(0),ReadyTimer_(0),DrawStart_(start_max)

{
}

HUD::~HUD()
{
}

void HUD::Initialize()
{
	DrawStart_ = start_ready;

	//csv����p�����[�^�ǂݍ���
	SetHUDCSV();
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

	hFirstIcon_ = Image::Load("Image\\MiniMap\\blue_circle.png");
	assert(hFirstIcon_ >= 0);

	hSecondIcon_ = Image::Load("Image\\MiniMap\\red_circle.png");
	assert(hSecondIcon_ >= 0);


	//�����摜�n���h���z���������
	ArrayHandle = { hNumber0_,hNumber1_,hNumber2_,hNumber3_,hNumber4_,
	hNumber5_,hNumber6_,hNumber7_,hNumber8_,hNumber9_ };
}

void HUD::Update()
{
}

void HUD::Draw()
{
	
}

void HUD::Release()
{
}

void HUD::DrawImGui()
{
	//�V�[���N���X����̎w���ɂ���ČĂԕ`��֐���ς���
	switch (DrawMode_)
	{
	case S_BeforeStart:
	{
		DrawImGuiExplanation();
	}
	break;
	case S_Ready:
	{
		DrawImGuiScore();
		DrawImGuiStartLogo();
	}
	break;
	case S_Playing:
	{
		DrawImGuiScore();
		DrawImGuiTimer();
	}
	break;
	case S_Finish:
	{
		DrawImGuiTimer();
		DrawImGuiFinishLogo();
		DrawImGuiScore();
	}
	break;
	case S_Practice:
		DrawImGuiPracticeLogo();
		break;
	default:
		break;
	}

	//��ɕ\��������̂�switch���̊O�ŋL�q
	DrawImGuiMiniMap();
}

void HUD::DrawFullScreen()
{
	//�e�I�u�W�F�N�g�ɔ킳�邱�Ƃ�h�����߁A���̊֐�����Ă�
	//HUD�N���X��Draw()�͍Ō�ɌĂԂ悤�ɐ݌v
	//�󋵂ɉ����Đ؂�ւ���������(�X�^�[�g/�v���C/�I�����̃��S�Ȃ�)��
	//����switch���Ő؂�ւ��

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

void HUD::SetHUDCSV()
{
	//csv�t�@�C����ǂݍ���
	CsvReader csvTransform;
	csvTransform.Load("CSVdata\\HUDData\\HUDData.csv");

	//csv�t�@�C���̊e0��ڂ̕�����̔z����擾
	std::vector<std::string> ParamNames = {
		"backtitle","practice","explanation","start","finish",
		"tentime","onetime","minimap","playericon", "enemyicon",
		"playerscoreten","playerscoreone","enemyscoreten","enemyscoreone",
	};

	//�܂Ƃ߂ď�����
	InitCSVTransformArray(csvTransform, ParamNames, ImageArray);

	//csv�t�@�C����ǂݍ���
	CsvReader csveasing;
	csveasing.Load("CSVdata\\HUDData\\HUDSomeData.csv");

	//csv�t�@�C����0��ڂ̕�������擾
	std::string easingLogoName = "Easing";

	//0��ڂ̕������n���A���̍s�̃p�����[�^���擾
	std::vector<float> easingData = GetCSVReadData(csveasing, easingLogoName);

	//�������̏��Ԃ�csv�̊e�s�̏��Ԃɍ��킹��
	//v�̓Y������namespace�Ő錾�����񋓌^���g�p
	LogoChange = easingData[i_logochange];
	MaxScale = easingData[i_maxscale];
}

void HUD::DrawPracticeLogo()
{


	//"�^�C�g���ɖ߂�܂�"���S�`��
	Image::SetAndDraw(hBackTitleLogo_, logo_backtitle);

	//"���K���[�h"���S�`��
	Image::SetAndDraw(hPracticeNow_, logo_practice);
}

void HUD::DrawTimer()
{



	if(pGameTimer_ != nullptr)
	{
		//���݂̎���(�\�̈�,��̈�)���擾
		TimeIndexTen = pGameTimer_->GetTimeTen();
		TimeIndexOne = pGameTimer_->GetTimeOne();

		//�������Ԃ̏\�̈�,��̈ʂ�`��
		Image::SetAndDraw(ArrayHandle[TimeIndexTen], TenTime);
		Image::SetAndDraw(ArrayHandle[TimeIndexOne], OneTime);
	}
}

void HUD::DrawExplanation()
{


	//�Q�[���������S�`��
	Image::SetAndDraw(hGameExplanation_, logo_explanation);
}

void HUD::DrawStartLogo()
{


	//DrawStart�̏�Ԃɂ���ĕ`�悷�郍�S��؂�ւ���
	//DrawStart_�̏�Ԃ�start_ready->start_go�̏��ɕω����邪
	//start_ready�ɖ߂鏈����BattleScene����w��
	switch (DrawStart_)
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

	//"Finish!"���S�`��
	Image::SetAndDraw(hFinish_, logo_Finish);
}

void HUD::DrawMiniMap()
{


	if (pMiniMap_ != nullptr) 
	{
		PlayerIcon.position_ = pMiniMap_->GetFirstPos();
		EnemyIcon.position_ = pMiniMap_->GetSecondPos();
	}

	//�}�b�v�摜,Player,Enemy�̃A�C�R���`��
	Image::SetAndDraw(hMap_, MapIcon);
	Image::SetAndDraw(hFirstIcon_, PlayerIcon);
	Image::SetAndDraw(hSecondIcon_, EnemyIcon);
}

void HUD::DrawScore()
{
	//���݂̃X�R�A�����ꂼ��v�Z
	//�\�̈�:���݂̃X�R�A��10�ŏ��Z
	//��̈�:���݂̃X�R�A��10�ŏ��Z�����]��
	pScoreIndexTen = FirstScore_ / TenDivision;
	pScoreIndexOne = FirstScore_ % TenDivision;
	eScoreIndexTen = SecondScore_ / TenDivision;
	eScoreIndexOne = SecondScore_ % TenDivision;

	//Player�̃X�R�A�̏\�̈�,��̈ʂ�`��
	Image::SetAndDraw(ArrayHandle[pScoreIndexTen], PlayerScoreTen);
	Image::SetAndDraw(ArrayHandle[pScoreIndexOne], PlayerScoreOne);

	//Enemy�̃X�R�A�̏\�̈�,��̈ʂ�`��
	Image::SetAndDraw(ArrayHandle[eScoreIndexTen], EnemyScoreTen);
	Image::SetAndDraw(ArrayHandle[eScoreIndexOne], EnemyScoreOne);
}

void HUD::DrawImGuiExplanation()
{
#ifdef _DEBUG
	if (ImGui::TreeNode("Explanation"))
	{
		ImGui::SliderFloat("ExplanationX", &logo_explanation.position_.x, Image::LeftEdge, Image::RightEdge);
		ImGui::SliderFloat("ExplanationY", &logo_explanation.position_.y, Image::UpEdge, Image::DownEdge);
		ImGui::TreePop();
	}
#endif
}

void HUD::DrawImGuiStartLogo()
{
#ifdef _DEBUG
	if (ImGui::TreeNode("Start"))
	{
		ImGui::SliderFloat("StartX", &logo_start.position_.x, Image::LeftEdge, Image::RightEdge);
		ImGui::SliderFloat("StartY", &logo_start.position_.y, Image::UpEdge, Image::DownEdge);
		ImGui::TreePop();
	}
#endif
}

void HUD::DrawImGuiFinishLogo()
{
#ifdef _DEBUG
	if (ImGui::TreeNode("Finish"))
	{
		ImGui::SliderFloat("FinishX", &logo_Finish.position_.x, Image::LeftEdge, Image::RightEdge);
		ImGui::SliderFloat("FinishY", &logo_Finish.position_.y, Image::UpEdge, Image::DownEdge);
		ImGui::TreePop();
	}
#endif

}

void HUD::DrawImGuiPracticeLogo()
{
#ifdef _DEBUG
	if (ImGui::TreeNode("PracticeLogo"))
	{
		ImGui::SliderFloat("backtitleX", &logo_backtitle.position_.x, Image::LeftEdge, Image::RightEdge);
		ImGui::SliderFloat("backtitleY", &logo_backtitle.position_.y, Image::UpEdge, Image::DownEdge);

		ImGui::SliderFloat("practiceX", &logo_practice.position_.x, Image::LeftEdge, Image::RightEdge);
		ImGui::SliderFloat("practiceY", &logo_practice.position_.y, Image::UpEdge, Image::DownEdge);
		ImGui::TreePop();
	}
#endif
}

void HUD::DrawImGuiScore()
{
#ifdef _DEBUG
	if (ImGui::TreeNode("Score"))
	{
		ImGui::SliderFloat("PlayerScoreTenX", &PlayerScoreTen.position_.x, Image::LeftEdge, Image::RightEdge);
		ImGui::SliderFloat("PlayerScoreTenY", &PlayerScoreTen.position_.y, Image::UpEdge, Image::DownEdge);

		ImGui::SliderFloat("PlayerScoreOneX", &PlayerScoreOne.position_.x, Image::LeftEdge, Image::RightEdge);
		ImGui::SliderFloat("PlayerScoreOneY", &PlayerScoreOne.position_.y, Image::UpEdge, Image::DownEdge);

		ImGui::SliderFloat("EnemyScoreTenX", &EnemyScoreTen.position_.x, Image::LeftEdge, Image::RightEdge);
		ImGui::SliderFloat("EnemyScoreTenY", &EnemyScoreTen.position_.y, Image::UpEdge, Image::DownEdge);

		ImGui::SliderFloat("EnemyScoreOneX", &EnemyScoreOne.position_.x, Image::LeftEdge, Image::RightEdge);
		ImGui::SliderFloat("EnemyScoreOneY", &EnemyScoreOne.position_.y, Image::UpEdge, Image::DownEdge);

		ImGui::TreePop();
	}
#endif
}

void HUD::DrawImGuiTimer()
{
#ifdef _DEBUG
	if (ImGui::TreeNode("Timer"))
	{
		ImGui::SliderFloat("TenTimeX", &TenTime.position_.x, Image::LeftEdge, Image::RightEdge);
		ImGui::SliderFloat("TenTimeY", &TenTime.position_.y, Image::UpEdge, Image::DownEdge);

		ImGui::SliderFloat("OneTimeX", &OneTime.position_.x, Image::LeftEdge, Image::RightEdge);
		ImGui::SliderFloat("OneTimeY", &OneTime.position_.y, Image::UpEdge, Image::DownEdge);
		ImGui::TreePop();
	}
#endif
}

void HUD::DrawImGuiMiniMap()
{
#ifdef _DEBUG
	if (ImGui::TreeNode("MiniMap"))
	{
		ImGui::SliderFloat("MiniMapX", &MapIcon.position_.x, Image::LeftEdge, Image::RightEdge);
		ImGui::SliderFloat("MiniMapY", &MapIcon.position_.y, Image::UpEdge, Image::DownEdge);

		ImGui::Text("playerIconX:%.3f", PlayerIcon.position_.x);
		ImGui::Text("playerIconY:%.3f", PlayerIcon.position_.y);

		ImGui::Text("EnemyIconX:%.3f", EnemyIcon.position_.x);
		ImGui::Text("EnemyIconY:%.3f", EnemyIcon.position_.y);

		ImGui::TreePop();
	}
#endif
}

void HUD::DrawReady()
{
	if (++LogoChangeCount < ReadyTimer_)
	{
		//"Ready?"�̃��S�`��
		Image::SetAndDraw(hReady_, logo_start);
	}
	else
	{
		LogoChangeCount = 0;
		DrawStart_ = start_go;
	}
}

void HUD::DrawGo()
{
	//���X�Ƀ��S���g�傷�铮��

	//�J�E���^�[�ɖ��t���[�����Z
	EasingCount_ += DeltaTime;

	//�g�嗦���C�[�W���O�����Ōv�Z
	float scale = static_cast<float>(Easing::calculateScale(MaxScale, EasingCount_));

	//�g�����X�t�H�[���̊g��ʂɑ��
	logo_start.scale_.x = scale;
	logo_start.scale_.y = scale;

	//"Go!"�̃��S�`��
	Image::SetAndDraw(hGo_, logo_start);
}
