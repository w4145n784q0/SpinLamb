#include "sceneManager.h"

#include "Model.h"
#include "Image.h"
#include "Audio.h"

#include"../TitleScene.h"
#include"../ResultScene.h"
#include"../GameModeScene.h"
#include"../BattleScene.h"
#include"../PracticeScene.h"
#include"../HowToPlayScene.h"


//コンストラクタ
SceneManager::SceneManager(GameObject * parent)
	: GameObject(parent, "SceneManager"), currentSceneID_(SCENE_ID_TITLE), nextSceneID_(SCENE_ID_TITLE),
	FirstCharaScore_(0), SecondCharaScore_(0),PlayMode_(PlayerVSPlayer)
{
}

//初期化
void SceneManager::Initialize()
{
	//最初のシーンを準備
	currentSceneID_ = SCENE_ID_TITLE;
	nextSceneID_ = currentSceneID_;
	Instantiate<TitleScene>(this);
}

//更新
void SceneManager::Update()
{
	//次のシーンが現在のシーンと違う　＝　シーンを切り替えなければならない
	if (currentSceneID_ != nextSceneID_)
	{
		//そのシーンのオブジェクトを全削除
		KillAllChildren();

		//ロードしたデータを全削除
		Audio::Release();
		Model::AllRelease();
		Image::AllRelease();

		//次のシーンを作成
		switch (nextSceneID_)
		{
		case SCENE_ID_TITLE: Instantiate<TitleScene>(this); break;
		case SCENE_ID_GAMEMODE: Instantiate<GameModeScene>(this); break;
		case SCENE_ID_BATTLE: Instantiate<BattleScene>(this); break;
		case SCENE_ID_PRACTICE: Instantiate<PracticeScene>(this); break;
		case SCENE_ID_HOWTOPLAY: Instantiate<HowToPlayScene>(this); break;
		case SCENE_ID_RESULT: Instantiate<ResultScene>(this); break;
		}
		Audio::Initialize();
		currentSceneID_ = nextSceneID_;
	}
}

//描画
void SceneManager::Draw()
{
}

//開放
void SceneManager::Release()
{
}

//シーン切り替え（実際に切り替わるのはこの次のフレーム）
void SceneManager::ChangeScene(SCENE_ID next)
{
	nextSceneID_ = next;
}