#pragma once
#include "Engine/GameObject.h"
#include"StageManager.h"

class Ground :
    public GameObject
{
private:
	//----------���f���n���h��----------

	//�n�ʃ��f��
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