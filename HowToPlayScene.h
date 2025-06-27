#pragma once
#include "Engine/GameObject.h"
#include"BaseScene.h"

//あそびかた説明シーン
class HowToPlayScene :
    public BaseScene
{
private:
	//----------画像ハンドル----------

	//ルール説明
	int hExplanation_;

	//キーボード操作説明
	int hOperateKeyboard_;

	//コントローラー操作説明
	int hOperateController_;

	//----------サウンドハンドル----------
	int hSoundHowtoPlay_;
	
	//表示画像ステート
	enum HowtoImage 
	{
		Explanation = 0,
		OperateKeyBoard,
		OperateController,
	};
	HowtoImage ImageState_;

	//各画像のリスト
	std::list<HowtoImage> ImageList_;

	//ModeList_のインデックスを指す値
	std::list<HowtoImage>::iterator itr;

public:
	HowToPlayScene(GameObject* parent);
	~HowToPlayScene();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	//----------BaseSceneの継承関数----------
	//通常の処理
	void UpdateActive() override;

	//シーン遷移中の処理
	void UpdateTransition() override;
};

