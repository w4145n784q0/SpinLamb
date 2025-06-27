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

	//この状態は他クラスから操作できないようにcppファイルに記述
	enum DrawStartMode
	{
		start_ready = 0,
		start_go,
		start_max
	};
	DrawStartMode DrawStart;

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

	//イージング使用時のカウンター
	float EasingCount = 0;

	//ロゴ変更までのカウンター
	float LogoChangeCount = 0;

	//startlogo変更の際、どのタイミングで切り替えるか
	float LogoChange = 0.0f;

	//Go! のロゴの最大拡大率
	float MaxScale = 0.0f;
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

	//csvからパラメータ読み込み
	SetHUDCSV();

	//csvからパラメータ読み込み
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

	hPlayerIcon_ = Image::Load("Image\\MiniMap\\blue_circle.png");
	assert(hPlayerIcon_ >= 0);

	hEnemyIcon_ = Image::Load("Image\\MiniMap\\red_circle.png");
	assert(hEnemyIcon_ >= 0);


	//数字画像ハンドル配列を初期化
	ArrayHandle = { hNumber0_,hNumber1_,hNumber2_,hNumber3_,hNumber4_,
	hNumber5_,hNumber6_,hNumber7_,hNumber8_,hNumber9_ };
}

void HUD::Update()
{
}

void HUD::Draw()
{
	//各オブジェクトに被さることを防ぐため、この関数から呼ぶ
	//HUDクラスのDraw()は最後に呼ぶように設計
	//状況に応じて切り替えたいもの(スタート/プレイ/終了時のロゴなど)はこのswitch文で切り替える
	
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

void HUD::Release()
{
}

void HUD::SetHUDCSV()
{
	//csvファイルを読み込む
	CsvReader csvTransform;
	csvTransform.Load("CSVdata\\HUDData.csv");

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
	csveasing.Load("CSVdata\\HUDSomeData.csv");

	//csvファイルの0列目の文字列を取得
	std::string easing = "Easing";

	//指定した文字列がいずれかの0列目に存在したら
	if (csveasing.IsGetParamName(easing))
	{
		//その行を配列として全取得
		std::vector<float> v = csveasing.GetParam(easing);

		//初期化の順番はcsvの各行の順番に合わせる
		//vの添え字はnamespaceで宣言した列挙型を使用
		LogoChange = v[i_logochange];
		MaxScale = v[i_maxscale];
	}
}

void HUD::DrawPracticeLogo()
{
#ifdef _DEBUG
	if (ImGui::TreeNode("PracticeLogo"))
	{
		ImGui::SliderFloat("positionX", &logo_backtitle.position_.x, Image::LeftEdge, Image::RightEdge);
		ImGui::SliderFloat("positionY", &logo_backtitle.position_.y, Image::UpEdge, Image::DownEdge);

		ImGui::SliderFloat("positionX", &logo_practice.position_.x, Image::LeftEdge, Image::RightEdge);
		ImGui::SliderFloat("positionY", &logo_practice.position_.y, Image::UpEdge, Image::DownEdge);
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
#ifdef _DEBUG
	if (ImGui::TreeNode("Timer"))
	{
		ImGui::SliderFloat("positionX", &TenTime.position_.x, Image::LeftEdge, Image::RightEdge);
		ImGui::SliderFloat("positionY", &TenTime.position_.y, Image::UpEdge, Image::DownEdge);

		ImGui::SliderFloat("positionX", &OneTime.position_.x, Image::LeftEdge, Image::RightEdge);
		ImGui::SliderFloat("positionY", &OneTime.position_.y, Image::UpEdge, Image::DownEdge);
		ImGui::TreePop();
	}
#endif


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
#ifdef _DEBUG
	if (ImGui::TreeNode("Explanation"))
	{
		ImGui::SliderFloat("positionX", &logo_explanation.position_.x, Image::LeftEdge, Image::RightEdge);
		ImGui::SliderFloat("positionY", &logo_explanation.position_.y, Image::UpEdge, Image::DownEdge);
		ImGui::TreePop();
	}
#endif

	Image::SetTransform(hGameExplanation_, logo_explanation);
	Image::Draw(hGameExplanation_);
}

void HUD::DrawStartLogo()
{
#ifdef _DEBUG
	if (ImGui::TreeNode("Start"))
	{
		ImGui::SliderFloat("positionX", &logo_start.position_.x, Image::LeftEdge, Image::RightEdge);
		ImGui::SliderFloat("positionY", &logo_start.position_.y, Image::UpEdge, Image::DownEdge);
		ImGui::TreePop();
	}
#endif

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
#ifdef _DEBUG
	if (ImGui::TreeNode("Timer"))
	{
		ImGui::SliderFloat("positionX", &logo_Finish.position_.x, Image::LeftEdge, Image::RightEdge);
		ImGui::SliderFloat("positionY", &logo_Finish.position_.y, Image::UpEdge, Image::DownEdge);
		ImGui::TreePop();
	}
#endif

	Image::SetTransform(hFinish_, logo_Finish);
	Image::Draw(hFinish_);
}

void HUD::DrawMiniMap()
{
#ifdef _DEBUG
	if (ImGui::TreeNode("MiniMap"))
	{
		ImGui::SliderFloat("positionX", &MapIcon.position_.x, Image::LeftEdge, Image::RightEdge);
		ImGui::SliderFloat("positionY", &MapIcon.position_.y, Image::UpEdge, Image::DownEdge);

		ImGui::Text("positionX", PlayerIcon.position_.x);
		ImGui::Text("positionX", PlayerIcon.position_.y);

		ImGui::Text("positionX", EnemyIcon.position_.x);
		ImGui::Text("positionX", EnemyIcon.position_.y);

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
#ifdef _DEBUG
	if (ImGui::TreeNode("Score"))
	{
		ImGui::SliderFloat("positionX", &PlayerScoreTen.position_.x, Image::LeftEdge, Image::RightEdge);
		ImGui::SliderFloat("positionY", &PlayerScoreTen.position_.y, Image::UpEdge, Image::DownEdge);

		ImGui::SliderFloat("positionX", &PlayerScoreOne.position_.x, Image::LeftEdge, Image::RightEdge);
		ImGui::SliderFloat("positionY", &PlayerScoreOne.position_.y, Image::UpEdge, Image::DownEdge);

		ImGui::SliderFloat("positionX", &EnemyScoreTen.position_.x, Image::LeftEdge, Image::RightEdge);
		ImGui::SliderFloat("positionY", &EnemyScoreTen.position_.y, Image::UpEdge, Image::DownEdge);

		ImGui::SliderFloat("positionX", &EnemyScoreOne.position_.x, Image::LeftEdge, Image::RightEdge);
		ImGui::SliderFloat("positionY", &EnemyScoreOne.position_.y, Image::UpEdge, Image::DownEdge);

		ImGui::TreePop();
	}
#endif

	//現在のスコアをそれぞれ計算
	//十の位:現在のスコアを10で除算
	//一の位:現在のスコアを10で除算した余り
	pScoreIndexTen = PlayerScore_ / TenDivision;
	pScoreIndexOne = PlayerScore_ % TenDivision;
	eScoreIndexTen = EnemyScore_ / TenDivision;
	eScoreIndexOne = EnemyScore_ % TenDivision;

	Image::SetTransform(ArrayHandle[pScoreIndexTen], PlayerScoreTen);
	Image::Draw(ArrayHandle[pScoreIndexTen]);
	Image::SetTransform(ArrayHandle[pScoreIndexOne], PlayerScoreOne);
	Image::Draw(ArrayHandle[pScoreIndexOne]);

	Image::SetTransform(ArrayHandle[eScoreIndexTen], EnemyScoreTen);
	Image::Draw(ArrayHandle[eScoreIndexTen]);
	Image::SetTransform(ArrayHandle[eScoreIndexOne], EnemyScoreOne);
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
