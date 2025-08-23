#include "PracticeScene.h"
#include"../Engine/Image.h"
#include"../Engine/Input.h"
#include"../Engine/Audio.h"
#include"../Engine/SceneManager.h"
#include"../Engine/Camera.h"
#include"../StageSourceFile/StageManager.h"
#include"../ViewSourceFile/GameView.h"

namespace
{
	//csv読み込み時のインデックス(練習シーンの各変数)
	enum PracticeIndex
	{
		i_CharacterAddID = 0,
	};

	//キャラクターの初期化時、IDに加算する値(for文でiに加算する値)
	int CharacterAddID = 0;
}

PracticeScene::PracticeScene(GameObject* parent)
	:PlayScene(parent,"PracticeScene"),
	hSoundPractice_(-1),
	pPlayer1_(nullptr), pPlayer2_(nullptr), pEnemy_(nullptr), pHUD_(nullptr),
	pTransitionEffect_(nullptr), pMiniMap_(nullptr),
	ActivePlayers_({}), ActiveEnemys_({}),PracticeState_(S_Now)
{
}

PracticeScene::~PracticeScene()
{
}

void PracticeScene::Initialize()
{
	//プレイシーン(基底クラス)の初期化を行う
	PlayScene::Initialize();

	//csvからパラメータ読み込み
	SetCSVPractice();

	//StageManagerクラス生成
	Instantiate<StageManager>(this);

	//StageManagerから各移動制限の値を取得
	StageManager* pStageManager = (StageManager*)FindObject("StageManager");
	assert(pStageManager != nullptr);

	float North = pStageManager->GetNorthEnd();
	float South = pStageManager->GetSouthEnd();
	float West = pStageManager->GetWestEnd();
	float East = pStageManager->GetEastEnd();

	//各キャラクターの初期化用の配列
	std::vector<Player*> InitPlayers = {};
	std::vector<Enemy*> InitEnemys = {};
	std::vector<Character*> InitCharacters = {};

	//登場させるキャラクターは事前に生成だけ行う
	//値の初期化は後に行う
	pPlayer1_ = Instantiate<Player>(this);
	assert(pPlayer1_ != nullptr);

	//初期化配列に追加
	InitPlayers.push_back(pPlayer1_);
	InitCharacters.push_back(pPlayer1_);

	//「ひとりで」か「ふたりで」かで生成するキャラクターを変更
	SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
	assert(pSceneManager != nullptr);

	if (pSceneManager->IsPlayerVSEnemy())
	{
		//事前に生成
		pEnemy_ = Instantiate<Enemy>(this);
		assert(pEnemy_ != nullptr);

		//初期化配列に追加
		InitCharacters.push_back(pEnemy_);
		InitEnemys.push_back(pEnemy_);

		//画面状態のセット(一人プレイ用)
		GameView::SetGameViewMode(GameView::S_Single);

		//画面を一人プレイ用に設定
		Camera::FullScreen();
	}
	if (pSceneManager->IsPlayerVSPlayer())
	{
		//事前に生成
		pPlayer2_ = Instantiate<Player>(this);
		assert(pPlayer2_ != nullptr);

		//初期化配列に追加
		InitPlayers.push_back(pPlayer2_);
		InitCharacters.push_back(pPlayer2_);

		//画面状態のセット(二人プレイ用、左右分割する)
		GameView::SetGameViewMode(GameView::S_Dual);

		//二人プレイ用のカメラを設定
		Camera::HalfScreen();
	}

	//Player,Enemy共通の初期化処理
	for (int i = 0; i < InitCharacters.size(); i++)
	{
		//移動制限(各ステージの端)を渡す
		InitCharacters[i]->SetEnd(North, South, West, East);

		//IDを割り振る
		InitCharacters[i]->SetID(i + CharacterAddID);
	}

	//Playerの初期化処理

	//player初期化時の文字列配列
	std::string PlayerNames[] = { "Player1","Player2" };

	//プレイヤー初期化時に読み込むcsvファイルのパス
	std::string csvPath[] = { "CSVdata\\CharacterData\\PlayerData1.csv" ,
		"CSVdata\\CharacterData\\PlayerData2.csv" };

	//プレイヤー初期化時に読み込むfbxモデルのパス
	std::string modelPath[] = { "Model\\Character.fbx" ,"Model\\Character_black.fbx" };

	//InitPlayers分だけ初期化
	for (int i = 0; i < InitPlayers.size(); i++)
	{
		//プレイヤーの名前を設定
		InitPlayers[i]->SetObjectName(PlayerNames[i]);

		//使うコントローラーのID設定
		InitPlayers[i]->SetControllerID(i);

		//プレイヤーの初期化(csv、モデルのパス)
		InitPlayers[i]->PlayerInit(csvPath[i], modelPath[i]);

		//実際に動くプレイヤー(CPUではない)を登録
		ActivePlayers_.push_back(InitPlayers[i]);

		//プレイヤーに移動を許可
		InitPlayers[i]->PlayerStart();
	}
	
	//Enemyの初期化処理
	for (int i = 0; i < InitEnemys.size(); i++)
	{
		if (!InitPlayers.empty())
		{
			//プレイヤーの初期化配列からランダムなインスタンスをセット
			//(現状は敵と一対一なのでplayer1が選ばれる)
			//PracticeSceneから設定することで値の相違・結合度の上昇を防ぐ
			InitEnemys[i]->SetPlayerPointer(InitPlayers[rand() % InitPlayers.size()]);

			//生成した敵を登録
			ActiveEnemys_.push_back(InitEnemys[i]);
		}
	}

	//プレイヤーのポインタを設定
	GameView::SetPlayers(InitPlayers);

	//敵のポインタを設定
	GameView::SetEnemy(pEnemy_);

	//各クラス生成
	Instantiate<MiniMap>(this);
	Instantiate<HUD>(this);
	Instantiate<TransitionEffect>(this);

	//インスタンスを初期化
	pMiniMap_ = (MiniMap*)FindObject("MiniMap");
	assert(pMiniMap_ != nullptr);

	pHUD_ = (HUD*)FindObject("HUD");
	assert(pHUD_ != nullptr);

	pTransitionEffect_ = (TransitionEffect*)FindObject("TransitionEffect");
	assert(pTransitionEffect_ != nullptr);

	//MiniMapのポインタを渡す
	//HUDクラスと同じポインタを渡すことで値の相違を防ぐ
	pHUD_->SetMiniMapPointer(pMiniMap_);

	//GameViewにHUDのポインタを渡す

	GameView::SetHUD(pHUD_);
	GameView::SetTransitionEffect(pTransitionEffect_);

	//各画像・サウンドの読み込み

	//同じディレクトリ内からのパスは省略
	//パスの一部を文字列にし、結合させる
	std::string Sound = "Sound\\";
	std::string BGM = "BGM\\";

	hSoundPractice_ = Audio::Load(Sound + BGM + "Practice.wav", true);
	assert(hSoundPractice_ >= 0);

	//フリープレイ用サウンド再生
	Audio::Play(hSoundPractice_);
}

void PracticeScene::Update()
{
	//BaseSceneの更新処理を呼ぶ
	//UpdateActive,UpdateTransitionは継承先の関数が呼ばれる
	BaseScene::Update();
}

void PracticeScene::Draw()
{
	//背景描画
	PlayScene::DrawBackScreen();

	//今のPracticeStateの状態から、HUDクラスに描画するものを指示
	switch (PracticeState_)
	{
	case PracticeScene::S_Now:
		pHUD_->SetDrawMode(Mode_Practice);
		break;
	case PracticeScene::S_Pause:
		pHUD_->SetDrawMode(Mode_PracticePause);
		break;
	default:
		break;
	}

}

void PracticeScene::Release()
{
	//実体は消さず、アドレスのみ無効化する
	if (pPlayer1_ != nullptr)
	{
		pPlayer1_ = nullptr;
	}
	if (pPlayer2_ != nullptr)
	{
		pPlayer2_ = nullptr;
	}
	if (pEnemy_ != nullptr)
	{
		pEnemy_ = nullptr;
	}
	if (pHUD_ != nullptr)
	{
		pHUD_ = nullptr;
	}
	if (pTransitionEffect_ != nullptr)
	{
		pTransitionEffect_ = nullptr;
	}

	if (pMiniMap_ != nullptr)
	{
		pMiniMap_ = nullptr;
	}
}

void PracticeScene::UpdateActive()
{
	//通常の動いている状態

	//登録されたプレイヤー・CPUを更新
	//プレイヤーが複数存在する場合を想定して
	//Battle,Practiceシーンから動かす
	for (auto player : ActivePlayers_)
	{
		player->PlayerRun();
	}
	for (auto enemy : ActiveEnemys_)
	{
		enemy->EnemyRun();
	}

	//ミニマップの位置を更新
	pMiniMap_->SetOriginalFirstPos(pPlayer1_->GetPosition());
	if (pPlayer2_ != nullptr)
	{
		pMiniMap_->SetOriginalSecondPos(pPlayer2_->GetPosition());
	}
	else if (pEnemy_ != nullptr)
	{
		pMiniMap_->SetOriginalSecondPos(pEnemy_->GetPosition());
	}

	PlayScene::WaitGotoPause();
}

void PracticeScene::UpdateInActive()
{	
	//画面を止めている状態

	//Pause表示中の処理
	PlayScene::UpdatePauseMenu();
}

void PracticeScene::UpdateTransition()
{
	//シーン遷移状態

	if (++SceneTransitionTimer_ > SceneShortTransition)
	{
		//SceneManagerのインスタンスからタイトルシーンへ
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		assert(pSceneManager != nullptr);

		pSceneManager->ChangeScene(SCENE_ID_TITLE);

		//シーン遷移用タイマーをリセット
		SceneTransitionTimer_ = 0;

		//練習用サウンド停止
		Audio::Stop(hSoundPractice_);

		//ゲームシーン状態を通常に戻しておく
		SceneState_ = S_Active;
		
		//1画面に戻す
		GameView::SetGameViewMode(GameView::S_NormalScreen);

		//画面分割しているならカメラを全体表示に戻す
		if (GameView::IsDual())
		{
			Camera::FullScreen();
		}
	}
}

void PracticeScene::GotoPause()
{
	//ポーズ画面に向かう処理 PlaySceneから上書き

	//ポーズ画面状態へ移行
	PracticeState_ = S_Pause;
}

void PracticeScene::GotoPlay()
{
	//ゲーム画面に向かう処理 PlaySceneから上書き

	//練習中状態へ移行
	PracticeState_ = S_Now;
}

void PracticeScene::GotoTitle()
{
	//タイトルに向かう処理 PlaySceneから上書き

	//シーン遷移エフェクト(黒くフェードアウト)を設定
	if (pTransitionEffect_ != nullptr)
	{
		pTransitionEffect_->FadeOutStartBlack();
		pTransitionEffect_->SetTransitionTime(SceneShortTransition);
	}
}

void PracticeScene::SetPauseIconY()
{
	if (pHUD_ != nullptr)
	{
		pHUD_->SetPauseIcon(TmpIconPosY_);
	}
}

void PracticeScene::SetCSVPractice()
{
	//csvファイル読み込む
	CsvReader csv;
	csv.Load("CSVdata\\SceneData\\PracticeData.csv");

	//csvファイルの各0列目の文字列を取得
	std::string PracticeName = "Practice";

	//0列目の文字列を渡し、その行のパラメータを取得
	std::vector<float> PracticeData = GetCSVReadData(csv, PracticeName);

	//初期化の順番はcsvの各行の順番に合わせる
	//vの添え字はnamespaceで宣言した列挙型を使用
	CharacterAddID = static_cast<int>(PracticeData[i_CharacterAddID]);
}
