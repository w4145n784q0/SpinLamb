#pragma once
#include "Engine/GameObject.h"
#include"StageManager.h"

//地面の描画・更新を行うクラス
class Ground :
    public GameObject
{
private:
	//----------モデルハンドル----------

	//地面モデル
	int hGrass_;

	int hhouse_;
public:
	Ground(GameObject* parent);
	~Ground();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	//----------セッター・ゲッター関数----------

	//ステージモデルのモデル番号を返す(レイキャスト用)
	int GetModelHandle() { return hGrass_; }
};