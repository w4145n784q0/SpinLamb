#include "HUD.h"
#include"../Engine/Image.h"
#include"../EffectSourceFile/Easing.h"
#include"../Engine/Global.h"

//描画操作のみ扱うクラス
namespace
{
	//csv読み込み時のインデックス(イージング処理用の変数)
	enum EasingIndex
	{
		i_GoMinScale = 0,
		i_GoMaxScale,
		i_TimeMinScale,
		i_TimeMaxScale,
		i_TimeDuration,
		i_Max
	};

	//----------画像描画用トランスフォーム----------

	//"練習モード"
	Transform LogoPractice;

	//ゲーム簡易説明
	Transform LogoExplanation;

	//Ready,Go!ロゴ
	Transform LogoStart;

	//Finish!ロゴ
	Transform LogoFinish;

	//画面分割の枠
	Transform SplitLine;

	//時間表記十の位
	Transform TenTime;

	//時間表記一の位
	Transform OneTime;

	//マップのトランスフォーム
	Transform MapIcon;

	//プレイヤーアイコンのトランスフォーム
	Transform FirstIcon;

	//敵アイコンのトランスフォーム
	Transform SecondIcon;


	//スコア表示位置のトランスフォーム
	Transform FirstScoreTen;//プレイヤー1の10の位
	Transform FirstScoreOne;//プレイヤー1の1の位
	Transform SecondScoreTen;//プレイヤー2・Enemyの1の位
	Transform SecondScoreOne;//プレイヤー2・Enemyの1の位

	//ポーズ画面のトランスフォーム
	Transform TransPauseMenu;

	//ポーズ画面の選択アイコンのトランスフォーム
	Transform TransPauseIcon;

	//画像用トランスフォームを配列に入れる
	//初期化の際に使用する
	std::vector<std::reference_wrapper<Transform>> ImageArray = {
	LogoPractice,LogoExplanation,LogoStart,
	LogoFinish, SplitLine, TenTime ,OneTime, MapIcon,FirstIcon,SecondIcon,
	FirstScoreTen, FirstScoreOne, SecondScoreTen, SecondScoreOne,
	TransPauseMenu, TransPauseIcon
	};

	//ナンバーハンドルの配列
	std::vector<int> ArrayHandle = {};

	//時間表記のナンバーハンドルの添え字(10の位)
	int TimeIndexTen = 0;

	//時間表記のナンバーハンドルの添え字(1の位)
	int TimeIndexOne = 0; 
	
	//ナンバーハンドルの添え字 Player1またはPlayer2,Enemyそれぞれの十の位、一の位
	int FirstScoreIndexTen = 0;
	int FirstScoreIndexOne = 0;
	int SecondScoreIndexTen = 0;
	int SecondScoreIndexOne = 0;

	//ロゴ変更までのカウンター
	float LogoChangeCount = 0;

	//----------イージング用定数----------

	//Go!のロゴの最小拡大率(定数)
	float GoMinScale = 0.0f;

	//Go! のロゴの最大拡大率(定数)
	float GoMaxScale = 0.0f;

	//制限時間の最小拡大率(定数)
	float TimeMinScale = 0.0f;

	//制限時間の最大拡大率(定数)
	float TimeMaxScale = 0.0f;

	//制限時間に行うイージング処理の実行時間(定数)
	float TimeDuration = 0.0f;
}

HUD::HUD(GameObject* parent)
	:GameObject(parent, "HUD")/*, hPracticeNow_(-1), hGameExplanation_(-1),
	hStart_(-1),hReady_(-1),hGo_(-1),hFinish_(-1), hSplitLine_(-1),
	hNumber0_(-1), hNumber1_(-1), hNumber2_(-1), hNumber3_(-1), hNumber4_(-1),
	hNumber5_(-1), hNumber6_(-1), hNumber7_(-1), hNumber8_(-1), hNumber9_(-1),
	hMap_(-1), hFirstIcon_(-1), hSecondIcon_(-1), hPauseMenu_(-1),hPauseIcon_(-1),
	FirstScore_(0), SecondScore_(0),
	pGameTimer_(nullptr), pMiniMap_(nullptr), DrawMode_(Mode_None),DrawStart_(S_MaxStartMode),
	ReadyTimer_(0), EasingCount_(0) */

{
}

HUD::~HUD()
{
}

void HUD::Initialize()
{
	//モジュールの初期化
	if (HUDDebug_ == nullptr)
	{
		HUDDebug_ = std::make_unique<HUDDebugPanel>(this);
	}
	if (HUDParam_ == nullptr)
	{
		HUDParam_ = std::make_unique<HUDParam>(this);
	}
	if(HUDCsvLoader_ == nullptr)
	{
		HUDCsvLoader_ = std::make_unique<HUDCsvLoader>(this);
	}
	if (HUDImageLoader_ == nullptr)
	{
		HUDImageLoader_ = std::make_unique<HUDImageLoader>(this);
	}


	HUDParam_->DrawStart_ = S_StartReady;

	//csvからパラメータ読み込み
	
	//	SetHUDCSV();
	HUDCsvLoader_->SetHUDCSV();

	//画像の初期化
	HUDImageLoader_->ImageInitialize();

	//同じディレクトリ内からのパスは省略
	//パスの一部を文字列にし、結合させる

	//std::string Image = "Image\\";
	//std::string Practice = "Practice\\";
	//std::string Battle = "Battle\\";
	//std::string Play = "Play\\";
	//std::string Number = "number\\";
	//std::string MiniMap = "MiniMap\\";


	////各画像の読み込み
	//hPracticeNow_ = Image::Load(Image + Practice + "PracticeLogo.png");
	//assert(hPracticeNow_ >= 0);

	//hGameExplanation_ = Image::Load(Image + Battle + "Rule.png");
	//assert(hGameExplanation_ >= 0);

	//hStart_ = Image::Load(Image + Battle + "StartLogo.png");
	//assert(hStart_ >= 0);

	//hReady_ = Image::Load(Image + Battle + "ReadyLogo.png");
	//assert(hReady_ >= 0);

	//hGo_ = Image::Load(Image + Battle + "GoLogo.png");
	//assert(hGo_ >= 0);

	//hFinish_ = Image::Load(Image + Battle + "FinishLogo.png");
	//assert(hFinish_ >= 0);

	//hSplitLine_ = Image::Load(Image + Play + "ViewLine.png");
	//assert(hSplitLine_ >= 0);

	//hNumber0_ = Image::Load(Image + Number + "Number0.png");
	//assert(hNumber0_ >= 0);

	//hNumber1_ = Image::Load(Image + Number + "Number1.png");
	//assert(hNumber1_ >= 0);

	//hNumber2_ = Image::Load(Image + Number + "Number2.png");
	//assert(hNumber2_ >= 0);
	//
	//hNumber3_ = Image::Load(Image + Number + "Number3.png");
	//assert(hNumber3_ >= 0);
	//
	//hNumber4_ = Image::Load(Image + Number + "Number4.png");
	//assert(hNumber4_ >= 0);

	//hNumber5_ = Image::Load(Image + Number + "Number5.png");
	//assert(hNumber5_ >= 0);

	//hNumber6_ = Image::Load(Image + Number + "Number6.png");
	//assert(hNumber6_ >= 0);

	//hNumber7_ = Image::Load(Image + Number + "Number7.png");
	//assert(hNumber7_ >= 0);

	//hNumber8_ = Image::Load(Image + Number + "Number8.png");
	//assert(hNumber8_ >= 0);

	//hNumber9_ = Image::Load(Image + Number + "Number9.png");
	//assert(hNumber9_ >= 0);

	//hMap_ = Image::Load(Image + MiniMap + "MiniMap.png");
	//assert(hMap_ >= 0);

	//hFirstIcon_ = Image::Load(Image + MiniMap + "FirstCharaArrow.png");
	//assert(hFirstIcon_ >= 0);

	//hSecondIcon_ = Image::Load(Image + MiniMap + "SecondCharaArrow.png");
	//assert(hSecondIcon_ >= 0);

	//hPauseMenu_ = Image::Load(Image + "Play\\Pause.png");
	//assert(hPauseMenu_ >= 0);

	//hPauseIcon_ = Image::Load(Image + "GameMode\\SelectIcon.png");
	//assert(hPauseIcon_ >= 0);

	//数字画像ハンドル配列を初期化
	//HUDParam_->ArrayHandle = { hNumber0_,hNumber1_,hNumber2_,hNumber3_,hNumber4_,
	//hNumber5_,hNumber6_,hNumber7_,hNumber8_,hNumber9_ };

	//数字画像ハンドル配列を初期化
	HUDParam_->InitImageArray();
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
	//ImGui描画はモジュールに委任
	HUDDebug_->ImGuiDrawer(this);
}

void HUD::DrawFullScreen()
{
	//各オブジェクトに被さることを防ぐため、この関数から呼ぶ
	//これらの処理はGameViewから呼ばれる(通常のDrawとは別に呼ばれる)
	
	//状況に応じて切り替えたいもの(スタート/プレイ/終了時のロゴなど)は
	//このswitch文で切り替わる

	//シーンクラスからの指示によって呼ぶ描画関数を変える
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
	case Mode_PlayPause:
	{
		DrawScore();
		DrawTimer();
		DrawPause();
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
	{
		DrawPracticeLogo();
	}
	break;
	case Mode_PracticePause:
	{
		DrawPracticeLogo();
		DrawPause();
	}
	break;
	default:
		break;
	}

	//常に表示するものはswitch文の外で記述
	DrawMiniMap();
}

//void HUD::SetHUDCSV()
//{
//	//csvファイルを読み込む
//	CsvReader csvTransform;
//	csvTransform.Load("CSVdata\\HUDData\\HUDTransformData.csv");
//
//	//csvファイル(HUDTransformData.csv)の各0列目の文字列の配列を取得
//	std::vector<std::string> ParamNames = {
//		"Practice","Explanation","Start","Finish","Split",
//		"TenTime","OneTime","MiniMap","FirstIcon", "SecondIcon",
//		"FirstScoreTen","FirstScoreOne","SecondScoreTen","SecondScoreOne",
//		"PauseMenu","PauseIcon"
//	};
//
//	//まとめて初期化
//	InitCSVTransformArray(csvTransform, ParamNames, ImageArray);
//
//	//csvファイルを読み込む
//	CsvReader csvEasing;
//	csvEasing.Load("CSVdata\\HUDData\\HUDSomeData.csv");
//
//	//csvファイル(HUDSomeData.csv)の0列目の文字列を取得
//	std::string EasingLogoName = "Easing";
//
//	//0列目の文字列を渡し、その行のパラメータを取得
//	std::vector<float> EasingData = GetCSVReadData(csvEasing, EasingLogoName);
//
//	//初期化の順番はcsvの各行の順番に合わせる
//	//vの添え字はnamespaceで宣言した列挙型を使用
//	GoMinScale = EasingData[i_GoMinScale];
//	GoMaxScale = EasingData[i_GoMaxScale];
//	TimeMinScale = EasingData[i_TimeMinScale];
//	TimeMaxScale = EasingData[i_TimeMaxScale];
//	TimeDuration = EasingData[i_TimeDuration];
//}

//void HUD::SetPauseIcon(float _posY)
//{
//	//Y座標だけをトランスフォームに渡す(変わるのはY座標の位置のみ)
//	TransPauseIcon.position_.y = _posY;
//}

void HUD::DrawPracticeLogo()
{
	//"練習モード"ロゴ描画
	Image::SetAndDraw(hPracticeNow_, LogoPractice);
}

void HUD::DrawTimer()
{
	if(pGameTimer_ != nullptr)
	{
		//現在の時間(十の位,一の位)を取得
		TimeIndexTen = pGameTimer_->GetTimeTen();
		TimeIndexOne = pGameTimer_->GetTimeOne();

		//制限時間にイージング処理を行う処理
		DrawTimerEasing();

		//制限時間の十の位,一の位を描画
		Image::SetAndDraw(ArrayHandle[TimeIndexTen], TenTime);
		Image::SetAndDraw(ArrayHandle[TimeIndexOne], OneTime);
	}
}

void HUD::DrawTimerEasing()
{
	//残り時間n秒でイージング拡大処理(今は10秒)
	if (pGameTimer_->IsEasingTime() && pGameTimer_->IsCounting())
	{
		//時間が切り替わったタイミングでEasingCountを戻す
		if (pGameTimer_->GetIsSecondCount())
		{
			EasingCount_ = 0.0; 
		}

		if (EasingCount_ < TimeDuration)
		{
			//イージング経過時間を計算
			EasingCount_ += Dt;

			//正規化する
			double ratio = static_cast<double>(Normalize(EasingCount_));

			//拡大率をイージング計算
			double eased = Easing::EaseOutCubic(ratio);

			//拡大率を最小値~最大値の間で補完する
			double sca = Easing::Complement(TimeMinScale, TimeMaxScale, eased);

			//トランスフォームの拡大量に代入
			TenTime.scale_.x = TenTime.scale_.y = OneTime.scale_.x = OneTime.scale_.y = static_cast<float>(sca);
		}
		else
		{
			//イージング完了後は拡大率を戻す
			TenTime.scale_.x = TenTime.scale_.y = OneTime.scale_.x = OneTime.scale_.y = 1.0f;
		}
	}
	else
	{
		//計測中でない、または残り時間10秒以上なら拡大率は1.0f
		TenTime.scale_.x = TenTime.scale_.y = OneTime.scale_.x = OneTime.scale_.y = 1.0f;
		EasingCount_ = 1.0f;
	}
}

void HUD::DrawExplanation()
{
	//ゲーム説明ロゴ描画
	Image::SetAndDraw(hGameExplanation_, LogoExplanation);
}

void HUD::DrawStartLogo()
{
	//DrawStartの状態によって描画するロゴを切り替える
	//DrawStart_の状態はStartReady->StartGoの順に変化するが
	//start_readyに戻る処理はBattleSceneから指示
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
	//"Finish!"ロゴ描画
	Image::SetAndDraw(hFinish_, LogoFinish);
}

void HUD::DrawSplitLine()
{
	//画面分割の枠線描画
	Image::SetAndDraw(hSplitLine_, SplitLine);
}

void HUD::DrawPause()
{
	//ポーズ画面と選択アイコンの描画
	Image::SetAndDraw(hPauseMenu_, TransPauseMenu);
	Image::SetAndDraw(hPauseIcon_, TransPauseIcon);
}

void HUD::DrawMiniMap()
{
	//ミニマップを描画
	if (pMiniMap_ != nullptr) 
	{
		FirstIcon.position_ = pMiniMap_->GetFirstPos();
		FirstIcon.rotate_ = pMiniMap_->GetFirstRot();

		SecondIcon.position_ = pMiniMap_->GetSecondPos();
		SecondIcon.rotate_ = pMiniMap_->GetSecondRot();
	}

	//マップ画像,Player1,Player2・敵(CPU)のアイコン描画
	Image::SetAndDraw(hMap_, MapIcon);
	Image::SetAndDraw(hFirstIcon_, FirstIcon);
	Image::SetAndDraw(hSecondIcon_, SecondIcon);
}

void HUD::DrawScore()
{
	//現在のスコアをそれぞれ計算
	//十の位:現在のスコアを10で除算
	//一の位:現在のスコアを10で除算した余り
	FirstScoreIndexTen = DIVISION_TEN(FirstScore_);
	FirstScoreIndexOne = MODULO_TEN(FirstScore_);

	SecondScoreIndexTen = DIVISION_TEN(SecondScore_);
	SecondScoreIndexOne = MODULO_TEN(SecondScore_);

	//Player1のスコアの十の位,一の位を描画
	Image::SetAndDraw(ArrayHandle[FirstScoreIndexTen], FirstScoreTen);
	Image::SetAndDraw(ArrayHandle[FirstScoreIndexOne], FirstScoreOne);

	//Player2,Enemyのスコアの十の位,一の位を描画
	Image::SetAndDraw(ArrayHandle[SecondScoreIndexTen], SecondScoreTen);
	Image::SetAndDraw(ArrayHandle[SecondScoreIndexOne], SecondScoreOne);
}

//Transform& HUD::GetLogoExplanation() const
//{
//	return LogoExplanation;
//}
//
//Transform& HUD::GetLogoStart() const
//{
//	return LogoStart;
//}
//
//Transform& HUD::GetLogoFinish() const
//{
//	return LogoFinish;
//}
//
//Transform& HUD::GetLogoPractice() const
//{
//	return LogoPractice;
//}
//
//Transform& HUD::GetFirstScoreTen() const
//{
//	return FirstScoreTen;//プレイヤー1の10の位
//}
//
//Transform& HUD::GetFirstScoreOne() const
//{
//	return FirstScoreOne;//プレイヤー1の1の位
//}
//
//Transform& HUD::GetSecondScoreTen() const
//{
//	return SecondScoreTen;//プレイヤー2・Enemyの1の位
//}
//
//Transform& HUD::GetSecondScoreOne() const
//{
//	return SecondScoreOne;//プレイヤー2・Enemyの1の位
//}
//
//Transform& HUD::GetTenTime() const
//{
//	return TenTime; 
//}
//
//Transform& HUD::GetOneTime() const
//{
//	return OneTime;
//}
//
//Transform& HUD::GetMapIcon() const
//{
//	return MapIcon;
//}
//
//Transform& HUD::GetFirstIcon() const
//{
//	return FirstIcon;
//}
//
//Transform& HUD::GetSecondIcon() const
//{
//	return SecondIcon;
//}
//
//Transform& HUD::GetPauseMenu() const
//{
//	return TransPauseMenu;
//}
//
//Transform& HUD::GetPauseIcon() const
//{
//	return TransPauseIcon;
//}

void HUD::DrawReady()
{
	if (++LogoChangeCount < ReadyTimer_)
	{
		//タイマーが超えるまで"Ready?"のロゴ描画
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
	//徐々にロゴが拡大する動き

	//イージング経過時間を計算
	EasingCount_ += Dt;

	//正規化する
	double ratio = static_cast<double>(Normalize(EasingCount_));

	//拡大率をイージング計算
	double eased = Easing::EaseOutElastic(ratio);

	//拡大率を最小値~最大値の間で補完する
	double sca = Easing::Complement(GoMinScale, GoMaxScale, eased);

	//トランスフォームの拡大量に代入
	LogoStart.scale_.x = LogoStart.scale_.y = static_cast<float>(sca);

	//"Go!"のロゴ描画
	Image::SetAndDraw(hGo_, LogoStart);
}
