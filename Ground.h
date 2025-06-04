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
public:
	Ground(GameObject* parent);
	~Ground();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	int GetModelHandle() { return hGrass_; }
};