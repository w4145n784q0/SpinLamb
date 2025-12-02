#pragma once
#include "../Engine/GameObject.h"

//各シーンの基底クラス
//シーンごとの共通処理をまとめる
class BaseScene :
    public GameObject
{
protected:

	//----------状態遷移----------
	
	//ゲームシーン全般の状態
	//これらの値に応じて各Update関数を呼び出す
	enum SceneState
	{
		S_Active = 0,//通常のプレイ状態
		S_InActive,  //止めている状態
		S_Transition,//次のシーンに遷移中
		S_MaxScene
	};
	SceneState SceneState_;

	//----------タイマー----------

	//シーン遷移までの時間を計る
	int SceneTransitionTimer_;

	//----------シーン遷移時間----------
	//一度読み込んだら値は変化しないのでstatic

	//次のシーン遷移までの時間(1.0秒程)
	static int SceneShortTransition;

	//次のシーン遷移までの時間(1.5秒程)
	static int SceneTransition;

	//次のシーン遷移までの時間(2秒程)
	static int SceneLongTransition;

public:
	BaseScene(GameObject* parent);
	BaseScene(GameObject* parent, const std::string& name);
	virtual ~BaseScene();

	//更新
	void Update() override;

	//csvデータ初期化
	static void CSVSceneDataInitialize();

	//GameViewに自身のポインタをセット
	void SetPointerGameView();

	//ImGui描画(BaseScene関係のみ)
	void DrawImGuiBaseScene();

	//ImGui描画(継承したシーン用 継承先によって中身をオーバーライド)
	virtual void DrawImGuiMyScene() {};

	//以下の処理は純粋仮想関数のため、継承先で必ずオーバーライド

	//通常の処理
	virtual void UpdateActive() = 0;

	//止めているときの処理
	virtual void UpdateInActive() = 0;

	//シーン遷移中の処理(シーン遷移中の演出などに使用)
	virtual void UpdateTransition() = 0;
};

