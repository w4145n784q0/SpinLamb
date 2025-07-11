#pragma once
#include "global.h"
#include "GameObject.h"

//ゲームに登場するシーン
enum SCENE_ID
{
	SCENE_ID_TITLE = 0,//タイトル
	SCENE_ID_GAMEMODE,//ゲームモード選択
	SCENE_ID_BATTLE,//バトル
	SCENE_ID_PRACTICE,//フリープレイ(練習)
	SCENE_ID_HOWTOPLAY,//あそびかた
	SCENE_ID_RESULT,//リザルト
};

//-----------------------------------------------------------
//シーン切り替えを担当するオブジェクト
//-----------------------------------------------------------
class SceneManager : public GameObject
{
public:

	//コンストラクタ
	//引数：parent	親オブジェクト（基本的にゲームマネージャー）
	SceneManager(GameObject* parent);

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	//シーン切り替え（実際に切り替わるのはこの次のフレーム）
	//引数：next	次のシーンのID
	void ChangeScene(SCENE_ID next);

	bool IsBattleScene() { if (currentSceneID_ == SCENE_ID_BATTLE) return true; else return false; }
	bool IsPracticeScene() { if (currentSceneID_ == SCENE_ID_PRACTICE) return true; else return false; }

	void SetPlayerScore(int score) { PlayerScore_ = score; }
	void SetEnemyScore(int score) { EnemyScore_ = score; }

	int GetPlayerScore() const { return PlayerScore_; }
	int GetEnemyScore() const { return EnemyScore_; }

private:
	SCENE_ID currentSceneID_;	//現在のシーン
	SCENE_ID nextSceneID_;		//次のシーン

	int PlayerScore_;		//プレイヤーのスコア
	int EnemyScore_;		//敵のスコア

};