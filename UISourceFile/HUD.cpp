#include "HUD.h"
#include"../Engine/Image.h"
#include"../EffectSourceFile/Easing.h"


//�`�摀��݈̂����N���X
namespace
{
	//csv�ǂݍ��ݎ��̃C���f�b�N�X(�C�[�W���O�����p�̕ϐ�)
	enum EasingIndex
	{
		i_GoMinScale = 0,
		i_GoMaxScale,
		i_TimeMinScale,
		i_TimeMaxScale,
		i_TimeDuration,
		i_Max
	};

	//----------�摜�`��p�g�����X�t�H�[��----------
	//"�^�C�g���ɖ߂�܂�"
	Transform LogoBackTitle;

	//"���K���[�h"
	Transform LogoPractice;

	//�Q�[���ȈՐ���
	Transform LogoExplanation;

	//Ready,Go!���S
	Transform LogoStart;

	//Finish!���S
	Transform LogoFinish;

	//��ʕ����̘g
	Transform SplitLine;

	//���ԕ\�L�\�̈�
	Transform TenTime;

	//���ԕ\�L��̈�
	Transform OneTime;

	//�}�b�v�̃g�����X�t�H�[��
	Transform MapIcon;

	//�v���C���[�A�C�R���̃g�����X�t�H�[��
	Transform FirstIcon;

	//�G�A�C�R���̃g�����X�t�H�[��
	Transform SecondIcon;


	//�X�R�A�\���ʒu�̃g�����X�t�H�[��
	Transform FirstScoreTen;//�v���C���[1��10�̈�
	Transform FirstScoreOne;//�v���C���[1��1�̈�
	Transform SecondScoreTen;//�v���C���[2�EEnemy��1�̈�
	Transform SecondScoreOne;//�v���C���[2�EEnemy��1�̈�

	//�|�[�Y��ʂ̃g�����X�t�H�[��
	Transform TransPause;

	//�|�[�Y��ʂ̑I���A�C�R���̃g�����X�t�H�[��
	Transform TransPauseIcon;

	//�摜�p�g�����X�t�H�[����z��ɓ����
	//�������̍ۂɎg�p����
	std::vector<std::reference_wrapper<Transform>> ImageArray = {
	LogoBackTitle,LogoPractice,LogoExplanation,LogoStart,
	LogoFinish, SplitLine, TenTime ,OneTime, MapIcon,FirstIcon,SecondIcon,
	FirstScoreTen, FirstScoreOne, SecondScoreTen, SecondScoreOne,
	TransPause, TransPauseIcon
	};

	//�i���o�[�n���h���̔z��
	std::vector<int> ArrayHandle = {};

	//���ԕ\�L�̃i���o�[�n���h���̓Y����(10�̈�)
	int TimeIndexTen = 0;

	//���ԕ\�L�̃i���o�[�n���h���̓Y����(1�̈�)
	int TimeIndexOne = 0; 
	
	//�i���o�[�n���h���̓Y���� Player1�܂���Player2,Enemy���ꂼ��̏\�̈ʁA��̈�
	int FirstScoreIndexTen = 0;
	int FirstScoreIndexOne = 0;
	int SecondScoreIndexTen = 0;
	int SecondScoreIndexOne = 0;

	//���S�ύX�܂ł̃J�E���^�[
	float LogoChangeCount = 0;

	//----------�C�[�W���O�p�萔----------

	//Go!�̃��S�̍ŏ��g�嗦(�萔)
	float GoMinScale = 0.0f;

	//Go! �̃��S�̍ő�g�嗦(�萔)
	float GoMaxScale = 0.0f;

	//�������Ԃ̍ŏ��g�嗦(�萔)
	float TimeMinScale = 0.0f;

	//�������Ԃ̍ő�g�嗦(�萔)
	float TimeMaxScale = 0.0f;

	//�������Ԃɍs���C�[�W���O�����̎��s����(�萔)
	float TimeDuration = 0.0f;
}

HUD::HUD(GameObject* parent)
	:GameObject(parent, "HUD"), hBackTitleLogo_(-1), hPracticeNow_(-1), hGameExplanation_(-1),
	hStart_(-1),hReady_(-1),hGo_(-1),hFinish_(-1), hSplitLine_(-1),
	hNumber0_(-1), hNumber1_(-1), hNumber2_(-1), hNumber3_(-1), hNumber4_(-1),
	hNumber5_(-1), hNumber6_(-1), hNumber7_(-1), hNumber8_(-1), hNumber9_(-1),
	hMap_(-1), hFirstIcon_(-1), hSecondIcon_(-1), hPause_(-1),hPauseIcon_(-1),
	FirstScore_(0), SecondScore_(0),
	pGameTimer_(nullptr), pMiniMap_(nullptr), DrawMode_(Mode_None),DrawStart_(S_MaxStartMode),
	ReadyTimer_(0), EasingCount_(0) 

{
}

HUD::~HUD()
{
}

void HUD::Initialize()
{
	DrawStart_ = S_StartReady;

	//csv����p�����[�^�ǂݍ���
	SetHUDCSV();

	//�����f�B���N�g��������̃p�X�͏ȗ�
	//�p�X�̈ꕔ�𕶎���ɂ��A����������

	std::string Image = "Image\\";
	std::string Practice = "Practice\\";
	std::string Battle = "Battle\\";
	std::string Number = "number\\";
	std::string MiniMap = "MiniMap\\";


	//�e�摜�̓ǂݍ���
	hBackTitleLogo_ = Image::Load(Image + Practice + "BackTitleLogo.png");
	assert(hBackTitleLogo_ >= 0);

	hPracticeNow_ = Image::Load(Image + Practice + "PracticeLogo.png");
	assert(hPracticeNow_ >= 0);

	hGameExplanation_ = Image::Load(Image + Battle + "Rule.png");
	assert(hGameExplanation_ >= 0);

	hStart_ = Image::Load(Image + Battle + "StartLogo.png");
	assert(hStart_ >= 0);

	hReady_ = Image::Load(Image + Battle + "ReadyLogo.png");
	assert(hReady_ >= 0);

	hGo_ = Image::Load(Image + Battle + "GoLogo.png");
	assert(hGo_ >= 0);

	hFinish_ = Image::Load(Image + Battle + "FinishLogo.png");
	assert(hFinish_ >= 0);

	hSplitLine_ = Image::Load(Image + Battle + "ViewLine.png");
	assert(hSplitLine_ >= 0);

	hNumber0_ = Image::Load(Image + Number + "Number0.png");
	assert(hNumber0_ >= 0);

	hNumber1_ = Image::Load(Image + Number + "Number1.png");
	assert(hNumber1_ >= 0);

	hNumber2_ = Image::Load(Image + Number + "Number2.png");
	assert(hNumber2_ >= 0);
	
	hNumber3_ = Image::Load(Image + Number + "Number3.png");
	assert(hNumber3_ >= 0);
	
	hNumber4_ = Image::Load(Image + Number + "Number4.png");
	assert(hNumber4_ >= 0);

	hNumber5_ = Image::Load(Image + Number + "Number5.png");
	assert(hNumber5_ >= 0);

	hNumber6_ = Image::Load(Image + Number + "Number6.png");
	assert(hNumber6_ >= 0);

	hNumber7_ = Image::Load(Image + Number + "Number7.png");
	assert(hNumber7_ >= 0);

	hNumber8_ = Image::Load(Image + Number + "Number8.png");
	assert(hNumber8_ >= 0);

	hNumber9_ = Image::Load(Image + Number + "Number9.png");
	assert(hNumber9_ >= 0);

	hMap_ = Image::Load(Image + MiniMap + "MiniMap.png");
	assert(hMap_ >= 0);

	hFirstIcon_ = Image::Load(Image + MiniMap + "BlueIcon.png");
	assert(hFirstIcon_ >= 0);

	hSecondIcon_ = Image::Load(Image + MiniMap + "RedIcon.png");
	assert(hSecondIcon_ >= 0);

	hPause_ = Image::Load(Image + "Play\\Pause.png");
	assert(hPause_ >= 0);

	hPauseIcon_ = Image::Load(Image + "GameMode\\SelectIcon.png");
	assert(hPauseIcon_ >= 0);

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
	//���̂͏������A�A�h���X�̂ݖ���������
	if (pGameTimer_ != nullptr)
	{
		pGameTimer_ = nullptr;
	}
	
	if (pMiniMap_ != nullptr)
	{
		pMiniMap_ = nullptr;
	}
}

void HUD::DrawImGui()
{
	//�V�[���N���X����̎w���ɂ���ČĂԕ`��֐���ς���
	switch (DrawMode_)
	{
	case Mode_BeforeStart:
	{
		DrawImGuiExplanation();
	}
	break;
	case Mode_JustBefore:
	{
		DrawImGuiScore();
		DrawImGuiStartLogo();
	}
	break;
	case Mode_Playing:
	{
		DrawImGuiScore();
		DrawImGuiTimer();
	}
	break;
	case Mode_Finish:
	{
		DrawImGuiTimer();
		DrawImGuiFinishLogo();
		DrawImGuiScore();
	}
	break;
	case Mode_Practice:
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
	case Mode_BeforeStart:
	{
		DrawExplanation();
	}
	break;
	case Mode_JustBefore:
	{
		DrawScore();
		DrawStartLogo();
	}
	break;
	case Mode_Playing:
	{
		DrawScore();
		DrawTimer();
	}
	break;
	case Mode_Finish:
	{
		DrawTimer();
		DrawFinishLogo();
		DrawScore();
	}
	break;
	case Mode_Practice:
		DrawPracticeLogo();
		break;
	case Mode_None:
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
	csvTransform.Load("CSVdata\\HUDData\\HUDTransformData.csv");

	//csv�t�@�C��(HUDTransformData.csv)�̊e0��ڂ̕�����̔z����擾
	std::vector<std::string> ParamNames = {
		"backtitle","practice","explanation","start","finish","split",
		"tentime","onetime","minimap","firsticon", "secondicon",
		"firstscoreten","firstscoreone","secondscoreten","secondscoreone",
		"pause","pauseicon"
	};

	//�܂Ƃ߂ď�����
	InitCSVTransformArray(csvTransform, ParamNames, ImageArray);

	//csv�t�@�C����ǂݍ���
	CsvReader csvEasing;
	csvEasing.Load("CSVdata\\HUDData\\HUDSomeData.csv");

	//csv�t�@�C��(HUDSomeData.csv)��0��ڂ̕�������擾
	std::string EasingLogoName = "Easing";

	//0��ڂ̕������n���A���̍s�̃p�����[�^���擾
	std::vector<float> EasingData = GetCSVReadData(csvEasing, EasingLogoName);

	//�������̏��Ԃ�csv�̊e�s�̏��Ԃɍ��킹��
	//v�̓Y������namespace�Ő錾�����񋓌^���g�p
	GoMinScale = EasingData[i_GoMinScale];
	GoMaxScale = EasingData[i_GoMaxScale];
	TimeMinScale = EasingData[i_TimeMinScale];
	TimeMaxScale = EasingData[i_TimeMaxScale];
	TimeDuration = EasingData[i_TimeDuration];
}

void HUD::SetPauseIcon(XMFLOAT3 _position)
{
	TransPauseIcon.position_ = _position;
}

void HUD::DrawPracticeLogo()
{
	//"�^�C�g���ɖ߂�܂�"���S�`��
	Image::SetAndDraw(hBackTitleLogo_, LogoBackTitle);

	//"���K���[�h"���S�`��
	Image::SetAndDraw(hPracticeNow_, LogoPractice);
}

void HUD::DrawTimer()
{
	if(pGameTimer_ != nullptr)
	{
		//���݂̎���(�\�̈�,��̈�)���擾
		TimeIndexTen = pGameTimer_->GetTimeTen();
		TimeIndexOne = pGameTimer_->GetTimeOne();

		//�������ԂɃC�[�W���O�������s������
		DrawTimerEasing();

		//�������Ԃ̏\�̈�,��̈ʂ�`��
		Image::SetAndDraw(ArrayHandle[TimeIndexTen], TenTime);
		Image::SetAndDraw(ArrayHandle[TimeIndexOne], OneTime);
	}
}

void HUD::DrawTimerEasing()
{
	//�c�莞��n�b�ŃC�[�W���O�g�又��(����10�b)
	if (pGameTimer_->IsEasingTime() && pGameTimer_->IsCounting())
	{
		//���Ԃ��؂�ւ�����^�C�~���O��EasingCount��߂�
		if (pGameTimer_->GetIsSecondCount())
		{
			EasingCount_ = 0.0; 
		}

		if (EasingCount_ < TimeDuration)
		{
			//�C�[�W���O�o�ߎ��Ԃ��v�Z
			EasingCount_ += DeltaTime;

			//���K������
			double ratio = static_cast<double>(Normalize(EasingCount_));

			//�g�嗦���C�[�W���O�v�Z
			double eased = Easing::EaseOutCubic(ratio);

			//�g�嗦���ŏ��l~�ő�l�̊Ԃŕ⊮����
			double sca = Easing::Complement(TimeMinScale, TimeMaxScale, eased);

			//�g�����X�t�H�[���̊g��ʂɑ��
			TenTime.scale_.x = TenTime.scale_.y = OneTime.scale_.x = OneTime.scale_.y = static_cast<float>(sca);
		}
		else
		{
			//�C�[�W���O������͊g�嗦��߂�
			TenTime.scale_.x = TenTime.scale_.y = OneTime.scale_.x = OneTime.scale_.y = 1.0f;
		}
	}
	else
	{
		//�v�����łȂ��A�܂��͎c�莞��10�b�ȏ�Ȃ�g�嗦��1.0f
		TenTime.scale_.x = TenTime.scale_.y = OneTime.scale_.x = OneTime.scale_.y = 1.0f;
		EasingCount_ = 1.0f;
	}
}

void HUD::DrawExplanation()
{
	//�Q�[���������S�`��
	Image::SetAndDraw(hGameExplanation_, LogoExplanation);
}

void HUD::DrawStartLogo()
{
	//DrawStart�̏�Ԃɂ���ĕ`�悷�郍�S��؂�ւ���
	//DrawStart_�̏�Ԃ�start_ready->start_go�̏��ɕω����邪
	//start_ready�ɖ߂鏈����BattleScene����w��
	switch (DrawStart_)
	{
	case S_StartReady:
		DrawReady();
		break;
	case S_StartGo:
		DrawGo();
		break;
	default:
		break;
	}

}

void HUD::DrawFinishLogo()
{
	//"Finish!"���S�`��
	Image::SetAndDraw(hFinish_, LogoFinish);
}

void HUD::DrawSplitLine()
{
	//��ʕ����̘g���`��
	Image::SetAndDraw(hSplitLine_, SplitLine);
}

void HUD::DrawPause()
{
	//�|�[�Y��ʂƑI���A�C�R���̕`��
	Image::SetAndDraw(hPause_, TransPause);
	Image::SetAndDraw(hPauseIcon_, TransPauseIcon);
}

void HUD::DrawMiniMap()
{
	//�~�j�}�b�v��`��
	if (pMiniMap_ != nullptr) 
	{
		FirstIcon.position_ = pMiniMap_->GetFirstPos();
		SecondIcon.position_ = pMiniMap_->GetSecondPos();
	}

	//�}�b�v�摜,Player1,Player2�E�G(CPU)�̃A�C�R���`��
	Image::SetAndDraw(hMap_, MapIcon);
	Image::SetAndDraw(hFirstIcon_, FirstIcon);
	Image::SetAndDraw(hSecondIcon_, SecondIcon);
}

void HUD::DrawScore()
{
	//���݂̃X�R�A�����ꂼ��v�Z
	//�\�̈�:���݂̃X�R�A��10�ŏ��Z
	//��̈�:���݂̃X�R�A��10�ŏ��Z�����]��
	FirstScoreIndexTen = FirstScore_ / TenDivision;
	FirstScoreIndexOne = FirstScore_ % TenDivision;
	SecondScoreIndexTen = SecondScore_ / TenDivision;
	SecondScoreIndexOne = SecondScore_ % TenDivision;

	//Player1�̃X�R�A�̏\�̈�,��̈ʂ�`��
	Image::SetAndDraw(ArrayHandle[FirstScoreIndexTen], FirstScoreTen);
	Image::SetAndDraw(ArrayHandle[FirstScoreIndexOne], FirstScoreOne);

	//Player2,Enemy�̃X�R�A�̏\�̈�,��̈ʂ�`��
	Image::SetAndDraw(ArrayHandle[SecondScoreIndexTen], SecondScoreTen);
	Image::SetAndDraw(ArrayHandle[SecondScoreIndexOne], SecondScoreOne);
}

void HUD::DrawImGuiExplanation()
{
#ifdef _DEBUG
	if (ImGui::TreeNode("Explanation"))
	{
		if (ImGui::TreeNode("ExplanationPosition"))
		{
			ImGui::SliderFloat("Explanation_PositionX", &LogoExplanation.position_.x, Image::LeftEdge, Image::RightEdge);
			ImGui::SliderFloat("Explanation_PositionY", &LogoExplanation.position_.y, Image::UpEdge, Image::DownEdge);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("ExplanationRotate"))
		{
			ImGui::InputFloat("ExplanationRotateX", &LogoExplanation.rotate_.x, ZeroPointOne);
			ImGui::InputFloat("ExplanationRotateY", &LogoExplanation.rotate_.y, ZeroPointOne);
			ImGui::InputFloat("ExplanationRotateY", &LogoExplanation.rotate_.y, ZeroPointOne);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("ExplanationScale"))
		{
			ImGui::InputFloat("ExplanationScaleX", &LogoExplanation.scale_.x, ZeroPointOne);
			ImGui::InputFloat("ExplanationScaleY", &LogoExplanation.scale_.y, ZeroPointOne);
			ImGui::TreePop();
		}


		ImGui::TreePop();
	}
#endif
}

void HUD::DrawImGuiStartLogo()
{
#ifdef _DEBUG
	if (ImGui::TreeNode("Start"))
	{
		if (ImGui::TreeNode("StartPosition"))
		{
			ImGui::SliderFloat("StartPositionX", &LogoStart.position_.x, Image::LeftEdge, Image::RightEdge);
			ImGui::SliderFloat("StartPositionY", &LogoStart.position_.y, Image::UpEdge, Image::DownEdge);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("StartRotate"))
		{
			ImGui::InputFloat("StartRotateX", &LogoStart.rotate_.x, ZeroPointOne);
			ImGui::InputFloat("StartRotateY", &LogoStart.rotate_.y, ZeroPointOne);
			ImGui::InputFloat("StartRotateY", &LogoStart.rotate_.y, ZeroPointOne);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("StartScale"))
		{
			ImGui::InputFloat("StartScaleX", &LogoStart.scale_.x, ZeroPointOne);
			ImGui::InputFloat("StartScaleY", &LogoStart.scale_.y, ZeroPointOne);
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
#endif
}

void HUD::DrawImGuiFinishLogo()
{
#ifdef _DEBUG
	if (ImGui::TreeNode("Finish"))
	{
		if (ImGui::TreeNode("FinishPosition"))
		{
			ImGui::SliderFloat("FinishPositionX", &LogoFinish.position_.x, Image::LeftEdge, Image::RightEdge);
			ImGui::SliderFloat("FinishPositionY", &LogoFinish.position_.y, Image::UpEdge, Image::DownEdge);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("FinishRotate"))
		{
			ImGui::InputFloat("FinishRotateX", &LogoFinish.rotate_.x, ZeroPointOne);
			ImGui::InputFloat("FinishRotateY", &LogoFinish.rotate_.y, ZeroPointOne);
			ImGui::InputFloat("FinishRotateY", &LogoFinish.rotate_.y, ZeroPointOne);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("FinishScale"))
		{
			ImGui::InputFloat("FinishScaleX", &LogoFinish.scale_.x, ZeroPointOne);
			ImGui::InputFloat("FinishScaleY", &LogoFinish.scale_.y, ZeroPointOne);
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
#endif

}

void HUD::DrawImGuiPracticeLogo()
{
#ifdef _DEBUG
	if (ImGui::TreeNode("PracticeLogo"))
	{
		if (ImGui::TreeNode("PracticeLogoPosition"))
		{
			ImGui::SliderFloat("BackTitle_PositionX", &LogoBackTitle.position_.x, Image::LeftEdge, Image::RightEdge);
			ImGui::SliderFloat("BackTitle_PositionY", &LogoBackTitle.position_.y, Image::UpEdge, Image::DownEdge);

			ImGui::SliderFloat("Practice_PositionX", &LogoPractice.position_.x, Image::LeftEdge, Image::RightEdge);
			ImGui::SliderFloat("Practice_PositionY", &LogoPractice.position_.y, Image::UpEdge, Image::DownEdge);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("PracticeLogoRotate"))
		{
			ImGui::InputFloat("BackTitle_RotateX", &LogoBackTitle.rotate_.x, ZeroPointOne);
			ImGui::InputFloat("BackTitle_RotateY", &LogoBackTitle.rotate_.y, ZeroPointOne);
			ImGui::InputFloat("BackTitle_RotateZ", &LogoBackTitle.rotate_.z, ZeroPointOne);

			ImGui::InputFloat("Practice_RotateX", &LogoPractice.rotate_.x, ZeroPointOne);
			ImGui::InputFloat("Practice_RotateY", &LogoPractice.rotate_.y, ZeroPointOne);
			ImGui::InputFloat("Practice_RotateZ", &LogoPractice.rotate_.z, ZeroPointOne);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("PracticeLogoScale"))
		{
			ImGui::InputFloat("BackTitle_ScaleX", &LogoBackTitle.scale_.x, ZeroPointOne);
			ImGui::InputFloat("BackTitle_ScaleY", &LogoBackTitle.scale_.y, ZeroPointOne);

			ImGui::InputFloat("Practice_ScaleX", &LogoPractice.scale_.x, ZeroPointOne);
			ImGui::InputFloat("Practice_ScaleY", &LogoPractice.scale_.y, ZeroPointOne);
			ImGui::TreePop();
		}


		ImGui::TreePop();
	}
#endif
}

void HUD::DrawImGuiScore()
{
#ifdef _DEBUG
	if (ImGui::TreeNode("Score"))
	{
		if (ImGui::TreeNode("ScorePosition"))
		{
			ImGui::SliderFloat("FirstScoreTen_PositionX", &FirstScoreTen.position_.x, Image::LeftEdge, Image::RightEdge);
			ImGui::SliderFloat("FirstScoreTen_PositionY", &FirstScoreTen.position_.y, Image::UpEdge, Image::DownEdge);

			ImGui::SliderFloat("FirstScoreOne_PositionX", &FirstScoreOne.position_.x, Image::LeftEdge, Image::RightEdge);
			ImGui::SliderFloat("FirstScoreOne_PositionY", &FirstScoreOne.position_.y, Image::UpEdge, Image::DownEdge);

			ImGui::SliderFloat("SecondScoreTen_PositionX", &SecondScoreTen.position_.x, Image::LeftEdge, Image::RightEdge);
			ImGui::SliderFloat("SecondScoreTen_PositionY", &SecondScoreTen.position_.y, Image::UpEdge, Image::DownEdge);

			ImGui::SliderFloat("SecondScoreOne_PositionX", &SecondScoreOne.position_.x, Image::LeftEdge, Image::RightEdge);
			ImGui::SliderFloat("SecondScoreOne_PositionY", &SecondScoreOne.position_.y, Image::UpEdge, Image::DownEdge);

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("ScoreRotate"))
		{
			ImGui::InputFloat("FirstScoreTen_RotateX", &FirstScoreTen.rotate_.x, ZeroPointOne);
			ImGui::InputFloat("FirstScoreTen_RotateY", &FirstScoreTen.rotate_.y, ZeroPointOne);
			ImGui::InputFloat("FirstScoreTen_RotateZ", &FirstScoreTen.rotate_.z, ZeroPointOne);

			ImGui::InputFloat("FirstScoreOne_RotateX", &FirstScoreOne.rotate_.x, ZeroPointOne);
			ImGui::InputFloat("FirstScoreOne_RotateY", &FirstScoreOne.rotate_.y, ZeroPointOne);
			ImGui::InputFloat("FirstScoreOne_RotateZ", &FirstScoreOne.rotate_.z, ZeroPointOne);

			ImGui::InputFloat("SecondScoreTen_RotateX", &SecondScoreTen.rotate_.x, ZeroPointOne);
			ImGui::InputFloat("SecondScoreTen_RotateY", &SecondScoreTen.rotate_.y, ZeroPointOne);
			ImGui::InputFloat("SecondScoreTen_RotateZ", &SecondScoreTen.rotate_.z, ZeroPointOne);

			ImGui::InputFloat("SecondScoreOne_RotateX", &SecondScoreOne.rotate_.x, ZeroPointOne);
			ImGui::InputFloat("SecondScoreOne_RotateY", &SecondScoreOne.rotate_.y, ZeroPointOne);
			ImGui::InputFloat("SecondScoreOne_RotateZ", &SecondScoreOne.rotate_.z, ZeroPointOne);

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("ScoreScale"))
		{
			ImGui::InputFloat("FirstScoreTen_ScaleX", &FirstScoreTen.scale_.x, ZeroPointOne);
			ImGui::InputFloat("FirstScoreTen_ScaleY", &FirstScoreTen.scale_.y, ZeroPointOne);

			ImGui::InputFloat("FirstScoreOne_ScaleX", &FirstScoreOne.scale_.x, ZeroPointOne);
			ImGui::InputFloat("FirstScoreOne_ScaleY", &FirstScoreOne.scale_.y, ZeroPointOne);

			ImGui::InputFloat("SecondScoreTen_ScaleX", &SecondScoreTen.scale_.x, ZeroPointOne);
			ImGui::InputFloat("SecondScoreTen_ScaleY", &SecondScoreTen.scale_.y, ZeroPointOne);

			ImGui::InputFloat("SecondScoreOne_ScaleX", &SecondScoreOne.scale_.x, ZeroPointOne);
			ImGui::InputFloat("SecondScoreOne_ScaleY", &SecondScoreOne.scale_.y, ZeroPointOne);

			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
#endif
}

void HUD::DrawImGuiTimer()
{
#ifdef _DEBUG
	if (ImGui::TreeNode("Timer"))
	{
		if (ImGui::TreeNode("TimerPosition"))
		{
			ImGui::SliderFloat("TenTimePositionX", &TenTime.position_.x, Image::LeftEdge, Image::RightEdge);
			ImGui::SliderFloat("TenTimePositionY", &TenTime.position_.y, Image::UpEdge, Image::DownEdge);

			ImGui::SliderFloat("OneTimePositionX", &OneTime.position_.x, Image::LeftEdge, Image::RightEdge);
			ImGui::SliderFloat("OneTimePositionY", &OneTime.position_.y, Image::UpEdge, Image::DownEdge);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("TimerRotate"))
		{
			ImGui::InputFloat("TenTimeRotateX", &TenTime.rotate_.x, ZeroPointOne);
			ImGui::InputFloat("TenTimeRotateY", &TenTime.rotate_.y, ZeroPointOne);
			ImGui::InputFloat("TenTimeRotateZ", &TenTime.rotate_.z, ZeroPointOne);

			ImGui::InputFloat("OneTimeRotateX", &OneTime.rotate_.x, ZeroPointOne);
			ImGui::InputFloat("OneTimeRotateY", &OneTime.rotate_.y, ZeroPointOne);
			ImGui::InputFloat("OneTimeRotateZ", &OneTime.rotate_.z, ZeroPointOne);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("TimerScale"))
		{
			ImGui::InputFloat("TenTimeScaleX", &TenTime.scale_.x, ZeroPointOne);
			ImGui::InputFloat("TenTimeScaleY", &TenTime.scale_.y, ZeroPointOne);

			ImGui::InputFloat("OneTimeScaleX", &OneTime.scale_.x, ZeroPointOne);
			ImGui::InputFloat("OneTimeScaleY", &OneTime.scale_.y, ZeroPointOne);
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
#endif
}

void HUD::DrawImGuiMiniMap()
{
#ifdef _DEBUG
	if (ImGui::TreeNode("MiniMap"))
	{
		if (ImGui::TreeNode("MiniMapPosition"))
		{
			ImGui::SliderFloat("MiniMapPositionX", &MapIcon.position_.x, Image::LeftEdge, Image::RightEdge);
			ImGui::SliderFloat("MiniMapPositionY", &MapIcon.position_.y, Image::UpEdge, Image::DownEdge);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("MiniMapRotate"))
		{
			ImGui::InputFloat("MiniMapRotateX", &MapIcon.rotate_.x, ZeroPointOne);
			ImGui::InputFloat("MiniMapRotateY", &MapIcon.rotate_.y, ZeroPointOne);
			ImGui::InputFloat("MiniMapRotateZ", &MapIcon.rotate_.z, ZeroPointOne);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("MiniMapScale"))
		{
			ImGui::InputFloat("MiniMapScaleX", &MapIcon.scale_.x, ZeroPointOne);
			ImGui::InputFloat("MiniMapScaleY", &MapIcon.scale_.y, ZeroPointOne);
			ImGui::TreePop();
		}

		ImGui::Text("FirstIconX:%.3f", FirstIcon.position_.x);
		ImGui::Text("FirstIconY:%.3f", FirstIcon.position_.y);

		ImGui::Text("SecondIconX:%.3f", SecondIcon.position_.x);
		ImGui::Text("SecondIconY:%.3f", SecondIcon.position_.y);

		ImGui::TreePop();
	}
#endif
}

void HUD::DrawReady()
{
	if (++LogoChangeCount < ReadyTimer_)
	{
		//"Ready?"�̃��S�`��
		Image::SetAndDraw(hReady_, LogoStart);
	}
	else
	{
		LogoChangeCount = 0;
		DrawStart_ = S_StartGo;
	}
}

void HUD::DrawGo()
{
	//���X�Ƀ��S���g�傷�铮��

	//�C�[�W���O�o�ߎ��Ԃ��v�Z
	EasingCount_ += DeltaTime;

	//���K������
	double ratio = static_cast<double>(Normalize(EasingCount_));

	//�g�嗦���C�[�W���O�v�Z
	double eased = Easing::EaseOutElastic(ratio);

	//�g�嗦���ŏ��l~�ő�l�̊Ԃŕ⊮����
	double sca = Easing::Complement(GoMinScale, GoMaxScale, eased);

	//�g�����X�t�H�[���̊g��ʂɑ��
	LogoStart.scale_.x = LogoStart.scale_.y = static_cast<float>(sca);

	//"Go!"�̃��S�`��
	Image::SetAndDraw(hGo_, LogoStart);
}
