#include "TitleScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/SceneManager.h"
#include"Engine/Audio.h"

TitleScene::TitleScene(GameObject* parent)
	:BaseScene(parent,"TitleScene"), hBackScreen_(-1),hSoundTitle_(-1),hSoundStart_(-1),
	pTransitionEffect_(nullptr)
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
	//各クラス生成
	Instantiate<TransitionEffect>(this);

	//各画像・サウンドの読み込み
	hBackScreen_ = Image::Load("Image\\Title\\TitleScreen.jpg");
	assert(hBackScreen_ >= 0);
	hSoundTitle_ = Audio::Load("Sound\\BGM\\title.wav",true); 
	assert(hSoundTitle_ >= 0);
	hSoundStart_ = Audio::Load("Sound\\SE\\start.wav",false);
	assert(hSoundStart_ >= 0);

	//タイトル用サウンド再生
	Audio::Play(hSoundTitle_);

	//インスタンス生成
	pTransitionEffect_ = (TransitionEffect*)FindObject("TransitionEffect");
}

void TitleScene::Update()
{
	//BaseSceneの更新処理を呼ぶ
	//UpdateActive,UpdateTranslationは継承先の関数が呼ばれる
	BaseScene::Update();
}

void TitleScene::Draw()
{
	//背景描画
	Image::SetTransform(hBackScreen_, this->transform_);
	Image::Draw(hBackScreen_);
}

void TitleScene::Release()
{
}

void TitleScene::UpdateActive()
{
	//決定ボタン(Pキー・B/Startボタン)を押したらシーン遷移状態へ
	if (Input::IsKeyUp(DIK_P) || Input::IsPadButtonUp(XINPUT_GAMEPAD_B) || Input::IsPadButtonUp(XINPUT_GAMEPAD_START))
	{
		SceneState_ = S_Transition;

		//シーン遷移エフェクトを設定
		pTransitionEffect_->FadeOutStartBlack();
		//pTransitionEffect_->ZoomOutStart();
		pTransitionEffect_->SetTransitionTime(SceneTransition);

		//決定音を再生
		Audio::Play(hSoundStart_);
	}
}

void TitleScene::UpdateTransition()
{
	if (++SceneTransitionTimer_ > SceneTransition)
	{
		//SceneManagerのインスタンスからゲーム選択シーンへ
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_GAMEMODE);

		//シーン遷移用タイマーを戻す
		SceneTransitionTimer_ = 0;

		//タイトル用サウンド停止
		Audio::Stop(hSoundTitle_);

		//ゲームシーン状態を通常に戻しておく
		SceneState_ = S_Active;

		//画像の透明度を戻す
		//pTransitionEffect_->ResetTransitionAlpha();
	}
}
