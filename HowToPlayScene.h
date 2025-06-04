#pragma once
#include "Engine/GameObject.h"
class HowToPlayScene :
    public GameObject
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

	std::list<HowtoImage> ImageList_;//各画像のリスト
	std::list<HowtoImage>::iterator itr;//ModeList_のインデックスを指す値

public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	HowToPlayScene(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
};

