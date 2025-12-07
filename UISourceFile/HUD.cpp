#include "HUD.h"
#include"../Engine/Image.h"
#include"../EffectSourceFile/Easing.h"
#include"../Engine/Global.h"

HUD::HUD(GameObject* parent)
	:GameObject(parent, "HUD"), HUDDebug_(nullptr), HUDParam_(nullptr),
	HUDCsvLoader_(nullptr), HUDImageLoader_(nullptr), HUDDrawTable_(nullptr)
{
}

HUD::~HUD()
{
}

void HUD::Initialize()
{
	//モジュールの初期化
	//必要に応じてHUDParam_をセット
	if (HUDParam_ == nullptr)
	{
		HUDParam_ = std::make_unique<HUDParam>(this);
	}
	if (HUDDebug_ == nullptr)
	{
		HUDDebug_ = std::make_unique<HUDDebugPanel>(this);
	}
	if(HUDCsvLoader_ == nullptr)
	{
		HUDCsvLoader_ = std::make_unique<HUDCsvLoader>(this);
		HUDCsvLoader_->SetHUDParam(HUDParam_.get());
	}
	if (HUDImageLoader_ == nullptr)
	{
		HUDImageLoader_ = std::make_unique<HUDImageLoader>(this);
		HUDImageLoader_->SetHUDParam(HUDParam_.get());
	}
	if (HUDDrawTable_ == nullptr)
	{
		HUDDrawTable_ = std::make_unique<HUDDrawTable>(this);
	}

	//開始ロゴ描画モードの初期化
	HUDParam_->SetDrawStartMode(S_StartReady);

	//csvからパラメータ読み込み
	HUDCsvLoader_->SetHUDCSV();

	//画像の初期化
	HUDImageLoader_->ImageInitialize();

	//ナンバー画像ハンドル配列の初期化
	//画像を読み込んでから配列に入れる
	HUDParam_->InitImageArray();

	//描画テーブルの初期化
	BuildDrawTable();
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

	RenderContext ctx{};

	//常時タスク（ミニマップなど）
	for (auto& task : HUDDrawTable_->always)
	{
		//predicateが設定されていない=nullptrなら無条件で描く
		//predicateが設定されているならpredicate()を実行してtrueの時だけ描く
		//初めからnullptr設定なら無条件で描画
		//true/falseを返す関数が設定されているならその結果に応じて描画
		if (!task.predicate || task.predicate())
		{
			//描画前処理があれば実行
			if (task.pre)
			{
				task.pre(ctx);
			}

			//純粋な描画処理
			task.draw();
		}
	}

	//モード別タスク
	auto it = HUDDrawTable_->byMode.find(HUDParam_->DrawMode_);

	//endではない=モードに対応するタスクが見つかったら実行
	if (it != HUDDrawTable_->byMode.end())
	{
		for (auto& task : it->second)
		{
			//predicateが設定されていない=nullptrなら無条件で描く
			//predicateが設定されているならpredicate()を実行してtrueの時だけ描く
			//初めからnullptr設定なら無条件で描画
			//true/falseを返す関数が設定されているならその結果に応じて描画	
			if (!task.predicate || task.predicate()) 
			{
				//描画前処理があれば実行
				if (task.pre) 
				{
					task.pre(ctx);
				}

				//純粋な描画処理
				task.draw();
			}
		}
	}
}

void HUD::DrawPracticeLogo()
{
	//"練習モード"ロゴ描画
	Image::SetAndDraw(HUDParam_->hPracticeNow_, HUDParam_->LogoPractice_);
}

void HUD::DrawTimer()
{
	//制限時間の十の位,一の位を描画
	Image::SetAndDraw(HUDParam_->ArrayHandle_[HUDParam_->TimeIndexTen_], HUDParam_->TenTime_);
	Image::SetAndDraw(HUDParam_->ArrayHandle_[HUDParam_->TimeIndexOne_], HUDParam_->OneTime_);
}

void HUD::UpdateTimer()
{
	if (HUDParam_->pGameTimer_ != nullptr)
	{
		//現在の時間(十の位,一の位)を取得
		HUDParam_->TimeIndexTen_ = HUDParam_->pGameTimer_->GetTimeTen();
		HUDParam_->TimeIndexOne_ = HUDParam_->pGameTimer_->GetTimeOne();

		//制限時間にイージング処理を行う処理
		TimerEasing();
	}
}

void HUD::TimerEasing()
{
	//残り時間n秒でイージング拡大処理(今は10秒)
	if (HUDParam_->pGameTimer_->IsEasingTime() && HUDParam_->pGameTimer_->IsCounting())
	{
		//時間が切り替わったタイミングでEasingCountを戻す
		if (HUDParam_->pGameTimer_->GetIsSecondCount())
		{
			HUDParam_->EasingCount_ = 0.0;
		}

		if (HUDParam_->EasingCount_ < HUDParam_->TimeDuration_)
		{
			//イージング経過時間を計算
			HUDParam_->EasingCount_ += Dt;

			//正規化する
			double ratio = static_cast<double>(Normalize(HUDParam_->EasingCount_));

			//拡大率をイージング計算
			double eased = Easing::EaseOutCubic(ratio);

			//拡大率を最小値~最大値の間で補完する
			double sca = Easing::Complement
			(HUDParam_->TimeMinScale_, HUDParam_->TimeMaxScale_, eased);

			//トランスフォームの拡大量に代入
			HUDParam_->TenTime_.scale_.x = HUDParam_->TenTime_.scale_.y =
				HUDParam_->OneTime_.scale_.x = HUDParam_->OneTime_.scale_.y =
				static_cast<float>(sca);
		}
		else
		{
			//イージング完了後は拡大率を戻す
			HUDParam_->TenTime_.scale_.x = HUDParam_->TenTime_.scale_.y = 
				HUDParam_->OneTime_.scale_.x = HUDParam_->OneTime_.scale_.y = 1.0f;
		}
	}
	else
	{
		//計測中でない、または残り時間10秒以上なら拡大率は1.0f
		HUDParam_->TenTime_.scale_.x = HUDParam_->TenTime_.scale_.y
			= HUDParam_->OneTime_.scale_.x = HUDParam_->OneTime_.scale_.y = 1.0f;
		HUDParam_->EasingCount_ = 1.0f;
	}
}

void HUD::UpdateMiniMapPosition()
{
	//ミニマップの位置,回転を取得
	if (HUDParam_->pMiniMap_ != nullptr)
	{
		HUDParam_->FirstIcon_.position_ = HUDParam_->pMiniMap_->GetFirstPos();
		HUDParam_->FirstIcon_.rotate_ = HUDParam_->pMiniMap_->GetFirstRot();

		HUDParam_->SecondIcon_.position_ = HUDParam_->pMiniMap_->GetSecondPos();
		HUDParam_->SecondIcon_.rotate_ = HUDParam_->pMiniMap_->GetSecondRot();
	}
}

void HUD::UpdateScoreCalculate()
{
	//現在のスコアをそれぞれ計算
	//十の位:現在のスコアを10で除算
	//一の位:現在のスコアを10で除算した余り
	HUDParam_->FirstScoreIndexTen_ = DIVISION_TEN(HUDParam_->FirstScore_);
	HUDParam_->FirstScoreIndexOne_ = MODULO_TEN(HUDParam_->FirstScore_);

	HUDParam_->SecondScoreIndexTen_ = DIVISION_TEN(HUDParam_->SecondScore_);
	HUDParam_->SecondScoreIndexOne_ = MODULO_TEN(HUDParam_->SecondScore_);
}

void HUD::DrawExplanation()
{
	//ゲーム説明ロゴ描画
	Image::SetAndDraw(HUDParam_->hGameExplanation_, HUDParam_->LogoExplanation_);
}

void HUD::DrawStartLogo()
{
	//DrawStartの状態によって描画するロゴを切り替える
	//DrawStart_の状態はStartReady->StartGoの順に変化するが
	//start_readyに戻る処理はBattleSceneから指示
	switch (HUDParam_->DrawStart_)
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
	Image::SetAndDraw(HUDParam_->hFinish_, HUDParam_->LogoFinish_);
}

void HUD::DrawSplitLine()
{
	//画面分割の枠線描画
	Image::SetAndDraw(HUDParam_->hSplitLine_, HUDParam_->SplitLine_);
}

void HUD::DrawPause()
{
	//ポーズ画面と選択アイコンの描画
	Image::SetAndDraw(HUDParam_->hPauseMenu_, HUDParam_->TransPauseMenu_);
	Image::SetAndDraw(HUDParam_->hPauseIcon_, HUDParam_->TransPauseIcon_);
}

void HUD::DrawMiniMap()
{
	//マップ画像,Player1,Player2・敵(CPU)のアイコン描画
	Image::SetAndDraw(HUDParam_->hMap_, HUDParam_->MapIcon_);
	Image::SetAndDraw(HUDParam_->hFirstIcon_, HUDParam_->FirstIcon_);
	Image::SetAndDraw(HUDParam_->hSecondIcon_, HUDParam_->SecondIcon_);
}

void HUD::DrawScore()
{
	//Player1のスコアの十の位,一の位を描画
	Image::SetAndDraw(HUDParam_->ArrayHandle_[HUDParam_->FirstScoreIndexTen_], HUDParam_->FirstScoreTen_);
	Image::SetAndDraw(HUDParam_->ArrayHandle_[HUDParam_->FirstScoreIndexOne_], HUDParam_->FirstScoreOne_);

	//Player2,Enemyのスコアの十の位,一の位を描画
	Image::SetAndDraw(HUDParam_->ArrayHandle_[HUDParam_->SecondScoreIndexTen_], HUDParam_->SecondScoreTen_);
	Image::SetAndDraw(HUDParam_->ArrayHandle_[HUDParam_->SecondScoreIndexOne_], HUDParam_->SecondScoreOne_);
}



void HUD::BuildDrawTable()
{
	//DrawMode_によってどんな描画をするか
	//それに応じた描画タスクを登録する
	//描画するものの文字列(name),条件(predicate),前処理(pre),描画処理(draw)の順に登録

	// -------- 常時描画（ミニマップ） --------
	HUDDrawTable_->always.push_back(RenderTask{
		"MiniMap", //描画するもの
		nullptr,   //常時
		[this](RenderContext&) { UpdateMiniMapPosition(); }, //前処理としてミニマップ位置更新
		[this]() { DrawMiniMap(); } //描画処理
		});

	// -------- Mode_BeforeStart --------
	HUDDrawTable_->byMode[Mode_BeforeStart].push_back(RenderTask{
		"Explanation",//描画するもの
		nullptr,      //常時
		nullptr,	  //前処理があれば記述
		[this]() { DrawExplanation(); } //描画処理
		});

	// -------- Mode_JustBefore --------
	HUDDrawTable_->byMode[Mode_JustBefore].push_back(RenderTask{
		"Score",	//描画するもの
		nullptr,	//常時
		[this](RenderContext&) { UpdateScoreCalculate(); }, //前処理としてスコア更新
		[this]() { DrawScore(); } //描画処理
		});

	HUDDrawTable_->byMode[Mode_JustBefore].push_back(RenderTask{
		"StartLogo",//描画するもの
		nullptr,	//常時
		nullptr,	//前処理があれば記述
		[this]() { DrawStartLogo(); } //描画処理
		});

	// -------- Mode_Playing --------
	HUDDrawTable_->byMode[Mode_Playing].push_back(RenderTask{
		"Score",	//描画するもの
		nullptr,	//常時
		[this](RenderContext&) { UpdateScoreCalculate(); }, //前処理としてスコア更新
		[this]() { DrawScore(); } //描画処理
		});

	HUDDrawTable_->byMode[Mode_Playing].push_back(RenderTask{
		"Timer",	//描画するもの
		[this]() { return HUDParam_->pGameTimer_ != nullptr; },//ポインタがnullでなければ描画
		[this](RenderContext&) { UpdateTimer(); }, //前処理としてタイマー更新
		[this]() { DrawTimer(); } //描画処理
		});

	// -------- Mode_PlayPause（Playing + Pause）--------
	HUDDrawTable_->byMode[Mode_PlayPause] = HUDDrawTable_->byMode[Mode_Playing];
	HUDDrawTable_->byMode[Mode_PlayPause].push_back(RenderTask{
		"Pause",    //描画するもの
		nullptr,	//常時
		nullptr,	//前処理があれば記述
		[this]() { DrawPause(); }
		});

	// -------- Mode_Finish --------
	HUDDrawTable_->byMode[Mode_Finish].push_back(RenderTask{
		"Timer",
		[this]() { return HUDParam_->pGameTimer_ != nullptr; },	//ポインタがnullでなければ描画
		[this](RenderContext&) { UpdateTimer(); },				//前処理としてタイマー更新
		[this]() { DrawTimer(); } //描画処理
		});

	HUDDrawTable_->byMode[Mode_Finish].push_back(RenderTask{
		"FinishLogo",	//描画するもの
		nullptr,		//常時
		nullptr,		//前処理があれば記述
		[this]() { DrawFinishLogo(); }
		});

	HUDDrawTable_->byMode[Mode_Finish].push_back(RenderTask{
		"Score",	//描画するもの
		nullptr,	//常時
		[this](RenderContext&) { UpdateScoreCalculate(); }, //前処理としてスコア更新
		[this]() { DrawScore(); }
		});

	// -------- Mode_Practice --------
	HUDDrawTable_->byMode[Mode_Practice].push_back(RenderTask{
		"PracticeLogo",	//描画するもの
		nullptr,		//常時
		nullptr,		//前処理があれば記述
		[this]() { DrawPracticeLogo(); }
		});

	// -------- Mode_PracticePause（Practice + Pause）--------
	HUDDrawTable_->byMode[Mode_PracticePause] = HUDDrawTable_->byMode[Mode_Practice];
	HUDDrawTable_->byMode[Mode_PracticePause].push_back(RenderTask{
		"Pause",	//描画するもの
		nullptr,	//常時
		nullptr,	//前処理があれば記述
		[this]() { DrawPause(); }
		});
}

void HUD::DrawReady()
{
	//一定時間"Ready?"のロゴを描画した後に"Go!"に遷る

	//直接インクリメントできないので仮のカウント用意
	int cnt = HUDParam_->LogoChangeCount_;

	if (++cnt < HUDParam_->ReadyTimer_)
	{
		//タイマーが超えるまで"Ready?"のロゴ描画
		Image::SetAndDraw(HUDParam_->hReady_, HUDParam_->LogoStart_);

		//仮のカウントを元に戻す
		HUDParam_->LogoChangeCount_ = cnt;
	}
	else
	{
		//カウンターを0に戻し、状態遷移
		HUDParam_->LogoChangeCount_ = 0;
		HUDParam_->DrawStart_ = S_StartGo;
	}
}

void HUD::DrawGo()
{
	//徐々にロゴが拡大する動き

	//イージング経過時間を計算
	HUDParam_->EasingCount_ += Dt;

	//正規化する
	double ratio = static_cast<double>(Normalize(HUDParam_->EasingCount_));

	//拡大率をイージング計算
	double eased = Easing::EaseOutElastic(ratio);

	//拡大率を最小値~最大値の間で補完する
	double sca = Easing::Complement(HUDParam_->GoMinScale_, HUDParam_->GoMaxScale_, eased);

	//トランスフォームの拡大量に代入
	HUDParam_->LogoStart_.scale_.x = HUDParam_->LogoStart_.scale_.y = static_cast<float>(sca);

	//"Go!"のロゴ描画
	Image::SetAndDraw(HUDParam_->hGo_, HUDParam_->LogoStart_);
}
