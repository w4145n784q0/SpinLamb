#include "HUD.h"
#include"Engine/Image.h"
#include"BattleScene.h"
#include <unordered_map>
#include<functional>


//描画操作のみ扱うクラス
namespace
{
	//時間の数字表記のインデックス
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

	//csv読み込み時のインデックス
	enum EasingIndex
	{
		i_logochange = 0,
		i_maxscale,
		i_max
	};

	//----------画像描画用トランスフォーム----------
	//"タイトルに戻ります"
	Transform logo_backtitle;

	//"練習モード"
	Transform logo_practice;

	//ゲーム簡易説明
	Transform logo_explanation;

	//Ready,Go!ロゴ
	Transform logo_start;

	//Finish!ロゴ
	Transform logo_Finish;

	//時間表記十の位
	Transform TenTime;

	//時間表記一の位
	Transform OneTime;

	//マップのトランスフォーム
	Transform MapIcon;

	//プレイヤーアイコンのトランスフォーム
	Transform PlayerIcon;

	//敵アイコンのトランスフォーム
	Transform EnemyIcon;

	//スコア表示位置のトランスフォーム
	Transform PlayerScoreTen;
	Transform PlayerScoreOne;
	Transform EnemyScoreTen;
	Transform EnemyScoreOne;

	//画像用トランスフォームを配列に入れる
	//初期化の際に使用する
	std::vector<std::reference_wrapper<Transform>> ImageArray = {
	logo_backtitle,logo_practice,logo_explanation,logo_start,
	logo_Finish ,TenTime ,OneTime, MapIcon,PlayerIcon,EnemyIcon,
	PlayerScoreTen,PlayerScoreOne,EnemyScoreTen, EnemyScoreOne
	};

	//ナンバーハンドルの配列
	std::array<int, MaxNumberIndex> ArrayHandle;

	//時間表記のナンバーハンドルの添え字(10の位)
	int TimeIndexTen = 0;

	//時間表記のナンバーハンドルの添え字(1の位)
	int TimeIndexOne = 0; 
	
	//ナンバーハンドルの添え字 Player,Enemyそれぞれの十の位、一の位
	int pScoreIndexTen = 0;
	int pScoreIndexOne = 0;
	int eScoreIndexTen = 0;
	int eScoreIndexOne = 0;

	//ロゴ変更までのカウンター
	float LogoChangeCount = 0;

	//startlogo変更の際、どのタイミングで切り替えるか(定数)
	float LogoChange = 0.0f;

	//Go! のロゴの最大拡大率(定数)
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

	//csvからパラメータ読み込み
	SetHUDCSV();
	Easing::SetSCVEasing();

	//各画像の読み込み
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


	//数字画像ハンドル配列を初期化
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
	//シーンクラスからの指示によって呼ぶ描画関数を変える
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

	//常に表示するものはswitch文の外で記述
	DrawImGuiMiniMap();
}

void HUD::DrawFullScreen()
{
	//各オブジェクトに被さることを防ぐため、この関数から呼ぶ
	//HUDクラスのDraw()は最後に呼ぶように設計
	//状況に応じて切り替えたいもの(スタート/プレイ/終了時のロゴなど)は
	//このswitch文で切り替わる

	//シーンクラスからの指示によって呼ぶ描画関数を変える
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

	//常に表示するものはswitch文の外で記述
	DrawMiniMap();
}

void HUD::SetHUDCSV()
{
	//csvファイルを読み込む
	CsvReader csvTransform;
	csvTransform.Load("CSVdata\\HUDData\\HUDData.csv");

	//csvファイルの各0列目の文字列の配列を取得
	std::vector<std::string> ParamNames = {
		"backtitle","practice","explanation","start","finish",
		"tentime","onetime","minimap","playericon", "enemyicon",
		"playerscoreten","playerscoreone","enemyscoreten","enemyscoreone",
	};

	//まとめて初期化
	InitCSVTransformArray(csvTransform, ParamNames, ImageArray);

	//csvファイルを読み込む
	CsvReader csveasing;
	csveasing.Load("CSVdata\\HUDData\\HUDSomeData.csv");

	//csvファイルの0列目の文字列を取得
	std::string easingLogoName = "Easing";

	//0列目の文字列を渡し、その行のパラメータを取得
	std::vector<float> easingData = GetCSVReadData(csveasing, easingLogoName);

	//初期化の順番はcsvの各行の順番に合わせる
	//vの添え字はnamespaceで宣言した列挙型を使用
	LogoChange = easingData[i_logochange];
	MaxScale = easingData[i_maxscale];
}

void HUD::DrawPracticeLogo()
{


	//"タイトルに戻ります"ロゴ描画
	Image::SetAndDraw(hBackTitleLogo_, logo_backtitle);

	//"練習モード"ロゴ描画
	Image::SetAndDraw(hPracticeNow_, logo_practice);
}

void HUD::DrawTimer()
{



	if(pGameTimer_ != nullptr)
	{
		//現在の時間(十の位,一の位)を取得
		TimeIndexTen = pGameTimer_->GetTimeTen();
		TimeIndexOne = pGameTimer_->GetTimeOne();

		//制限時間の十の位,一の位を描画
		Image::SetAndDraw(ArrayHandle[TimeIndexTen], TenTime);
		Image::SetAndDraw(ArrayHandle[TimeIndexOne], OneTime);
	}
}

void HUD::DrawExplanation()
{


	//ゲーム説明ロゴ描画
	Image::SetAndDraw(hGameExplanation_, logo_explanation);
}

void HUD::DrawStartLogo()
{


	//DrawStartの状態によって描画するロゴを切り替える
	//DrawStart_の状態はstart_ready->start_goの順に変化するが
	//start_readyに戻る処理はBattleSceneから指示
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

	//"Finish!"ロゴ描画
	Image::SetAndDraw(hFinish_, logo_Finish);
}

void HUD::DrawMiniMap()
{


	if (pMiniMap_ != nullptr) 
	{
		PlayerIcon.position_ = pMiniMap_->GetFirstPos();
		EnemyIcon.position_ = pMiniMap_->GetSecondPos();
	}

	//マップ画像,Player,Enemyのアイコン描画
	Image::SetAndDraw(hMap_, MapIcon);
	Image::SetAndDraw(hFirstIcon_, PlayerIcon);
	Image::SetAndDraw(hSecondIcon_, EnemyIcon);
}

void HUD::DrawScore()
{
	//現在のスコアをそれぞれ計算
	//十の位:現在のスコアを10で除算
	//一の位:現在のスコアを10で除算した余り
	pScoreIndexTen = FirstScore_ / TenDivision;
	pScoreIndexOne = FirstScore_ % TenDivision;
	eScoreIndexTen = SecondScore_ / TenDivision;
	eScoreIndexOne = SecondScore_ % TenDivision;

	//Playerのスコアの十の位,一の位を描画
	Image::SetAndDraw(ArrayHandle[pScoreIndexTen], PlayerScoreTen);
	Image::SetAndDraw(ArrayHandle[pScoreIndexOne], PlayerScoreOne);

	//Enemyのスコアの十の位,一の位を描画
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
		//"Ready?"のロゴ描画
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
	//徐々にロゴが拡大する動き

	//カウンターに毎フレーム加算
	EasingCount_ += DeltaTime;

	//拡大率をイージング処理で計算
	float scale = static_cast<float>(Easing::calculateScale(MaxScale, EasingCount_));

	//トランスフォームの拡大量に代入
	logo_start.scale_.x = scale;
	logo_start.scale_.y = scale;

	//"Go!"のロゴ描画
	Image::SetAndDraw(hGo_, logo_start);
}
