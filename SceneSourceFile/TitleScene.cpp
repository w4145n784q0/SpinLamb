#include "TitleScene.h"
#include"../Engine/Image.h"
#include"../Engine/Input.h"
#include"../Engine/Audio.h"
#include"../Engine/SceneManager.h"
#include"../ViewSourceFile/GameView.h"

TitleScene::TitleScene(GameObject* parent)
	:BaseScene(parent,"TitleScene"),
	hBackScreen_(-1),hSoundTitle_(-1),hSoundStart_(-1),
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

	//同じディレクトリ内からのパスは省略
	//パスの一部を文字列にし、結合させる
	std::string Title = "Image\\Title\\";
	std::string Sound = "Sound\\";
	std::string BGM = "BGM\\";
	std::string SE = "SE\\";

	hBackScreen_ = Image::Load(Title + "TitleScreen.jpg");
	assert(hBackScreen_ >= 0);
	hSoundTitle_ = Audio::Load(Sound + BGM + "Title.wav",true);
	assert(hSoundTitle_ >= 0);
	hSoundStart_ = Audio::Load(Sound + SE + "Start.wav", false, Audio::StartSoundNum_);
	assert(hSoundStart_ >= 0);

	//タイトル用サウンド再生
	Audio::Play(hSoundTitle_);

	//ポインタを取得
	pTransitionEffect_ = (TransitionEffect*)FindObject("TransitionEffect");
	assert(pTransitionEffect_ != nullptr);

	//GameViewにポインタを渡す
	GameView::SetTransitionEffect(pTransitionEffect_);
	SetPointerGameView();
}

void TitleScene::Update()
{
	//BaseSceneの更新処理を呼ぶ
	//UpdateActive,UpdateTransitionは継承先の関数が呼ばれる
	BaseScene::Update();
}

void TitleScene::Draw()
{
	//背景描画
	Image::SetAndDraw(hBackScreen_, this->transform_);
}

void TitleScene::Release()
{

}

void TitleScene::UpdateActive()
{
	//決定ボタン(Pキー/エンターキー・B/Startボタン)を押したらシーン遷移状態へ
	if (IsPushConfirmButton())
	{
		SceneState_ = S_Transition;

		//シーン遷移エフェクト(フェードアウト)を設定
		pTransitionEffect_->FadeOutStartBlack();
		pTransitionEffect_->SetTransitionTime(SceneTransition);

		//決定音を再生
		Audio::Play(hSoundStart_);
	}
}

void TitleScene::UpdateTransition()
{
	//時間経過で次のシーンに遷移
	//カウント中はシーン遷移エフェクト行う

	if (++SceneTransitionTimer_ > SceneTransition)
	{
		//SceneManagerのポインタからゲーム選択シーンへ
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		assert(pSceneManager != nullptr);

		pSceneManager->ChangeScene(SCENE_ID_GAMEMODE);

		//シーン遷移用タイマーをリセット
		SceneTransitionTimer_ = 0;

		//タイトル用サウンド停止
		Audio::Stop(hSoundTitle_);

		//ゲームシーン状態を通常に戻しておく
		SceneState_ = S_Active;
	}
}
