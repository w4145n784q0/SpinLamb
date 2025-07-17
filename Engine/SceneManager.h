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
private:
	SCENE_ID currentSceneID_;	//現在のシーン
	SCENE_ID nextSceneID_;		//次のシーン

	int FirstCharaScore_;		//プレイヤー1のスコア
	int SecondCharaScore_;		//プレイヤー2or敵のスコア

	enum PlayMode {
		PlayerVSEnemy,//一人プレイの時(PvE)
		PlayerVSPlayer,//二人プレイの時(PvP)
		MaxPlay
	};
	PlayMode PlayMode_;
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

	void SetFirstCharaScore(int score) { FirstCharaScore_ = score; }
	void SetSecondCharaScore(int score) { SecondCharaScore_ = score; }

	int GetFirstCharaScore() const { return FirstCharaScore_; }
	int GetSecondCharaScore() const { return SecondCharaScore_; }

	PlayMode GetPlayMode() const { return PlayMode_; }
	void SetPvEMode() { PlayMode_ = PlayerVSEnemy; }
	void SetPvPMode() { PlayMode_ = PlayerVSPlayer; }
	bool IsPlayerVSEnemy() const { return PlayMode_ == PlayerVSEnemy; }
	bool IsPlayerVSPlayer() const { return PlayMode_ == PlayerVSPlayer; }
};