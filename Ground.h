#pragma once
#include "Engine/GameObject.h"
#include"Engine/CsvReader.h"
#include"StageManager.h"

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