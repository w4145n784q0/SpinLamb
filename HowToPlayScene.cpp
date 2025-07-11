#include "HowToPlayScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/SceneManager.h"
#include"Engine/Audio.h"
#include"Engine/CsvReader.h"

HowToPlayScene::HowToPlayScene(GameObject* parent)
	:BaseScene(parent, "GameModeScene"),
	hExplanation_(-1), hOperateKeyboard_(-1), hOperateController_(-1),
	hSoundHowtoPlay_(-1), hBackGameMode_(-1),
	ImageState_(Explanation),pTransitionEffect_(nullptr)
{
}

HowToPlayScene::~HowToPlayScene()
{
}

void HowToPlayScene::Initialize()
{
	//各クラス生成
	Instantiate<TransitionEffect>(this);

	//各画像・サウンドの読み込み

	//同じディレクトリ内からのパスは省略
	//パスの一部を文字列にし、結合させる
	std::string path = "Image\\HowToPlay\\";

	hExplanation_ = Image::Load(path + "spinlamb_explanation.jpg");
	assert(hExplanation_ >= 0);

	hOperateKeyboard_ = Image::Load(path + "Howto_keyboard.jpg");
	assert(hOperateKeyboard_ >= 0);

	hOperateController_ = Image::Load(path + "Howto_controller.jpg");
	assert(hOperateController_ >= 0);

	hSoundHowtoPlay_ = Audio::Load("Sound\\BGM\\howtoPlay.wav", true);
	assert(hSoundHowtoPlay_ >= 0);

	hBackGameMode_ = Audio::Load("Sound\\SE\\backgamemode.wav");
	assert(hBackGameMode_ >= 0);

	//リストに各状態を追加
	ImageList_ = { Explanation ,OperateKeyBoard,OperateController};

	////インデックスの初期位置を指定
	itr = ImageList_.begin();

	//インスタンス生成
	pTransitionEffect_ = (TransitionEffect*)FindObject("TransitionEffect");

	//あそびかた用サウンド再生
	Audio::Play(hSoundHowtoPlay_);
}

void HowToPlayScene::Update()
{
	//BaseSceneの更新処理を呼ぶ
	//UpdateActive,UpdateTranslationは継承先の関数が呼ばれる
	BaseScene::Update();
}

void HowToPlayScene::Draw()
{
	switch (ImageState_)
	{
	case HowToPlayScene::Explanation:
		//ゲーム説明描画
		Image::SetAndDraw(hExplanation_, this->transform_);
		break;
	case HowToPlayScene::OperateKeyBoard:
		//キーボード操作説明描画
		Image::SetAndDraw(hOperateKeyboard_, this->transform_);
		break;
	case HowToPlayScene::OperateController:
		//コントローラー操作説明描画
		Image::SetAndDraw(hOperateController_, this->transform_);
		break;
	default:
		break;
	}
}

void HowToPlayScene::Release()
{
}

void HowToPlayScene::UpdateActive()
{
	//表示画像の移動
	//インデックスが先頭/末尾なら末尾/先頭へ戻る
	//前置デクリメントで配列オーバー防ぐ
	if (Input::IsKeyDown(DIK_RIGHT) || Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_LEFT))
	{
		if (itr == ImageList_.begin())
		{
			itr = --ImageList_.end();
		}
		else
		{
			--itr;
		}
		ImageState_ = *itr;
	}
	if (Input::IsKeyDown(DIK_LEFT) || Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_RIGHT))
	{
		if (itr == --ImageList_.end())
		{
			itr = ImageList_.begin();
		}
		else
		{
			++itr;
		}
		ImageState_ = *itr;
	}

	//決定ボタン(Aキー・A/Startボタン)を押したらシーン遷移状態へ
	if (Input::IsKeyUp(DIK_A) || Input::IsPadButtonUp(XINPUT_GAMEPAD_A) || Input::IsPadButtonUp(XINPUT_GAMEPAD_START))
	{
		SceneState_ = S_Transition;

		//シーン遷移エフェクト(フェードアウト)を設定
		pTransitionEffect_->FadeOutStartBlack();
		pTransitionEffect_->SetTransitionTime(SceneShortTransition);

		//決定(ゲームモードシーンに戻る)SE再生
		Audio::Play(hBackGameMode_);
	}
}

void HowToPlayScene::UpdateTransition()
{
	//時間経過で次のシーンに遷移
	//カウント中はシーン遷移エフェクト行う

	if (++SceneTransitionTimer_ > SceneShortTransition)
	{
		//SceneManagerのインスタンスからタイトルシーンへ
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_GAMEMODE);

		//シーン遷移用タイマーを戻す
		SceneTransitionTimer_ = 0;

		//あそびかた用サウンド停止
		Audio::Stop(hSoundHowtoPlay_);

		//ゲームシーン状態を通常に戻しておく
		SceneState_ = S_Active;
	}
}
