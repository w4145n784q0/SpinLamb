#include "HowToPlayScene.h"
#include"../Engine/Image.h"
#include"../Engine/Input.h"
#include"../Engine/Audio.h"
#include"../Engine/SceneManager.h"
#include"../ViewSourceFile/GameView.h"

HowToPlayScene::HowToPlayScene(GameObject* parent)
	:BaseScene(parent, "HowToPlayScene"),
	hExplanation_(-1), hOperateKeyboard_(-1), hOperateController_(-1),
	hSoundHowtoPlay_(-1), hSoundSelect_(-1),hSoundBackGameMode_(-1),
	ImageState_(S_Explanation),pTransitionEffect_(nullptr)
{
}

HowToPlayScene::~HowToPlayScene()
{
}

void HowToPlayScene::Initialize()
{
	//シーン遷移エフェクトクラス生成
	Instantiate<TransitionEffect>(this);

	//各画像・サウンドの読み込み

	//同じディレクトリ内からのパスは省略
	//パスの一部を文字列にし、結合させる
	std::string HowToPlay = "Image\\HowToPlay\\";
	std::string Sound = "Sound\\";
	std::string BGM = "BGM\\";
	std::string SE = "SE\\";

	hExplanation_ = Image::Load(HowToPlay + "Explanation.jpg");
	assert(hExplanation_ >= 0);

	hOperateKeyboard_ = Image::Load(HowToPlay + "HowToKeyboard.jpg");
	assert(hOperateKeyboard_ >= 0);

	hOperateController_ = Image::Load(HowToPlay + "HowToController.jpg");
	assert(hOperateController_ >= 0);

	hSoundHowtoPlay_ = Audio::Load(Sound + BGM + "HowToPlay.wav", true);
	assert(hSoundHowtoPlay_ >= 0);

	hSoundSelect_ = Audio::Load(Sound + SE + "Select.wav", false, Audio::GetSelectNum());
	assert(hSoundSelect_ >= 0);

	hSoundBackGameMode_ = Audio::Load(Sound + SE + "Cancel.wav", false, Audio::GetCancelNum());
	assert(hSoundBackGameMode_ >= 0);

	//リストに各状態を追加
	ImageList_ = { S_Explanation, S_OperateKeyBoard, S_OperateController };

	////インデックスの初期位置を指定
	itr = ImageList_.begin();

	//インスタンス生成
	pTransitionEffect_ = (TransitionEffect*)FindObject("TransitionEffect");
	assert(pTransitionEffect_ != nullptr);

	//GameViewにポインタを渡す
	GameView::SetTransitionEffect(pTransitionEffect_);

	//あそびかた用サウンド再生
	Audio::Play(hSoundHowtoPlay_);
}

void HowToPlayScene::Update()
{
	//BaseSceneの更新処理を呼ぶ
	//UpdateActive,UpdateTransitionは継承先の関数が呼ばれる
	BaseScene::Update();
}

void HowToPlayScene::Draw()
{
	switch (ImageState_)
	{
	case HowToPlayScene::S_Explanation:
		//ゲーム説明描画
		Image::SetAndDraw(hExplanation_, this->transform_);
		break;
	case HowToPlayScene::S_OperateKeyBoard:
		//キーボード操作説明描画
		Image::SetAndDraw(hOperateKeyboard_, this->transform_);
		break;
	case HowToPlayScene::S_OperateController:
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
	//インデックスが先頭/末尾の場合、末尾/先頭へ戻る
	//前置デクリメントで配列オーバー防ぐ
	if (Input::IsKeyDown(DIK_RIGHT) || Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_LEFT)
		|| Input::IsStickTiltLX_RIGHT())
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

		//選択SE再生
		Audio::Play(hSoundSelect_);
	}
	if (Input::IsKeyDown(DIK_LEFT) || Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_RIGHT)
		|| Input::IsStickTiltLX_LEFT())
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

		//選択SE再生
		Audio::Play(hSoundSelect_);
	}

	//決定ボタン(Aキー・A/Startボタン)を押したらシーン遷移状態へ
	if (Input::IsKeyUp(DIK_A) || Input::IsPadButtonUp(XINPUT_GAMEPAD_A) || Input::IsPadButtonUp(XINPUT_GAMEPAD_START))
	{
		SceneState_ = S_Transition;

		//シーン遷移エフェクト(フェードアウト)を設定
		pTransitionEffect_->FadeOutStartBlack();
		pTransitionEffect_->SetTransitionTime(SceneShortTransition);

		//決定(ゲームモードシーンに戻る)SE再生
		Audio::Play(hSoundBackGameMode_);
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
		assert(pSceneManager != nullptr);

		pSceneManager->ChangeScene(SCENE_ID_GAMEMODE);

		//シーン遷移用タイマーをリセット
		SceneTransitionTimer_ = 0;

		//あそびかた用サウンド停止
		Audio::Stop(hSoundHowtoPlay_);

		//ゲームシーン状態を通常に戻しておく
		SceneState_ = S_Active;
	}
}
