#include "ResultScene.h"
#include"Engine/CsvReader.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/Audio.h"
#include"Engine/SceneManager.h"



namespace
{
	//結果の判別

	//キャラクター1のスコア
	int FirstCharaScore = 0;

	//キャラクター2(CPU)のスコア
	int SecondCharaScore = 0;
}

ResultScene::ResultScene(GameObject* parent)
	:BaseScene(parent, "ResultScene"), hBackScreen_(-1), hYouWin_(-1),hCpuWin_(-1),
	hPlayer1Win_(-1),hPlayer2Win_(-1),
	hDraw_(-1), hPushTitle_(-1),
	hSoundResult_(-1), hSoundBackTitle_(-1), winner_(RESULTMAX), ResultArray_({}),
	pTransitionEffect_(nullptr)
{
}

ResultScene::~ResultScene()
{
}

void ResultScene::Initialize()
{
	//各クラス生成
	Instantiate<TransitionEffect>(this);

	//csvからパラメータ読み込み
	SetCSVResult();

	//各画像・サウンドの読み込み
	hBackScreen_ = Image::Load("Image\\GameMode\\GameModeScreen.jpg");
	assert(hBackScreen_ >= 0);

	//同じディレクトリ内からのパスは省略
	//パスの一部を文字列にし、結合させる
	std::string path = "Image\\Result\\";

	hYouWin_ = Image::Load(path + "YouWin.png");
	assert(hYouWin_ >= 0);

	hCpuWin_ = Image::Load(path + "CPUWin.png");
	assert(hCpuWin_ >= 0);

	hPlayer1Win_ = Image::Load(path + "Player1Win.png");
	assert(hPlayer1Win_);

	hPlayer2Win_ = Image::Load(path + "Player2Win.png");
	assert(hPlayer2Win_);

	hDraw_ = Image::Load(path + "Draw.png");
	assert(hDraw_ >= 0);

	hPushTitle_ = Image::Load(path + "PushToTitle.png");
	assert(hPushTitle_ >= 0);

	hSoundResult_ = Audio::Load("Sound\\BGM\\Result.wav", true);
	assert(hSoundResult_ >= 0);

	hSoundBackTitle_ = Audio::Load("Sound\\SE\\Cancel.wav", false, Audio::GetCancelNum());
	assert(hSoundBackTitle_ >= 0);

	//SceneManagerインスタンスからBattleSceneから各スコアを取得
	SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
	FirstCharaScore =  pSceneManager->GetFirstCharaScore();
	SecondCharaScore = pSceneManager->GetSecondCharaScore();

	//スコアによって勝敗状態を変化
	if (FirstCharaScore > SecondCharaScore)
	{
		winner_ = YOU_WIN;
	}
	else if (FirstCharaScore < SecondCharaScore)
	{
		winner_ = CPU_WIN;
	}
	else if (FirstCharaScore == SecondCharaScore)
	{
		winner_ = DRAW;
	}

	//画像ハンドル配列を初期化(プレイ人数によって分ける)
	if (pSceneManager->IsPlayerVSEnemy())
	{
		ResultArray_ = { hYouWin_, hCpuWin_, hDraw_ };
	}
	else if (pSceneManager->IsPlayerVSPlayer())
	{
		ResultArray_ = { hPlayer1Win_, hPlayer2Win_, hDraw_ };
	}

	//インスタンス生成
	pTransitionEffect_ = (TransitionEffect*)FindObject("TransitionEffect");
}

void ResultScene::Update()
{
	//BaseSceneの更新処理を呼ぶ
	//UpdateActive,UpdateTransitionは継承先の関数が呼ばれる
	BaseScene::Update();
	Audio::Play(hSoundResult_);
}

void ResultScene::Draw()
{
	//背景描画
	Image::SetAndDraw(hBackScreen_, this->transform_);

	//"Push Title"描画
	Image::SetAndDraw(hPushTitle_, PushTitle_);

	//結果のテキスト画像を表示
	//ResultArray_の添え字に勝敗状態を使う
	switch (winner_)
	{
	case ResultScene::YOU_WIN:
	case ResultScene::CPU_WIN:
	case ResultScene::DRAW:
	{
		Image::SetAndDraw(ResultArray_[winner_], Result_);
	}
		break;
	default:
		break;
	}
	

#ifdef _DEBUG
	if (ImGui::TreeNode("GameModeSelect"))
	{
		if (ImGui::TreeNode("Result"))
		{
			ImGui::SliderFloat("ResultPositionX", &Result_.position_.x, Image::LeftEdge, Image::RightEdge);
			ImGui::SliderFloat("ResultPositionY", &Result_.position_.y, Image::UpEdge, Image::DownEdge);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("PushTitle"))
		{
			ImGui::SliderFloat("PushTitlePositionX", &PushTitle_.position_.x, Image::LeftEdge, Image::RightEdge);
			ImGui::SliderFloat("PushTitlePositionY", &PushTitle_.position_.y, Image::UpEdge, Image::DownEdge);
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
#endif
}

void ResultScene::Release()
{
}

void ResultScene::SetCSVResult()
{
	//csvファイルを読み込む
	CsvReader csv;
	csv.Load("CSVdata\\SceneData\\ResultData.csv");

	//csvファイルの各0列目の文字列の配列を取得
	std::vector<std::string> ParamNames = {
		"Result","PushTitle"
	};

	//画像用トランスフォームを配列に入れる
	std::vector<std::reference_wrapper<Transform>> ImageArray = {
		Result_,PushTitle_
	};

	//まとめて初期化
	InitCSVTransformArray(csv, ParamNames, ImageArray);
}

void ResultScene::UpdateActive()
{
	//決定ボタン(Pキー・B/Startボタン)を押したらシーン遷移状態へ
	if (Input::IsKeyUp(DIK_P) || Input::IsPadButtonUp(XINPUT_GAMEPAD_B) || Input::IsPadButtonUp(XINPUT_GAMEPAD_START))
	{
		SceneState_ = S_Transition;

		//シーン遷移エフェクト(フェードアウト)を設定
		pTransitionEffect_->FadeOutStartBlack();
		pTransitionEffect_->SetTransitionTime(SceneShortTransition);

		//決定音を再生
		Audio::Play(hSoundBackTitle_);
	}
}

void ResultScene::UpdateTransition()
{
	//時間経過で次のシーンに遷移
	//カウント中はシーン遷移エフェクト行う
	if (++SceneTransitionTimer_ > SceneShortTransition)
	{
		//SceneManagerのインスタンスからタイトルシーンへ
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TITLE);

		//シーン遷移用タイマーをリセット
		SceneTransitionTimer_ = 0;

		//リザルト用サウンド停止
		Audio::Stop(hSoundResult_);

		//ゲームシーン状態を通常に戻しておく
		SceneState_ = S_Active;
	}
}
