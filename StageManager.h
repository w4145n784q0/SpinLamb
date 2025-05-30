#pragma once
#include "Engine/GameObject.h"
#include"Ground.h"
#include"Fence.h"

class StageManager :
    public GameObject
{
private:
	Transform GroundData_;//�n�ʃN���X�ɓn���f�[�^
	Transform WireData_;//��N���X(�S��)�ɓn���f�[�^
	Transform PillerData_;//��N���X(��)�ɓn���f�[�^

	float UpperEnd_;//�X�e�[�W�k�[
	float LowerEnd_; //�X�e�[�W��[
	float RightEnd_;//�X�e�[�W���[
	float LeftEnd_;//�X�e�[�W���[
	int PillerNum_;//���̐�

public:
	StageManager(GameObject* parent);
	~StageManager();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void SetSCV();

	void InitGroundData();
	void InitFenceData();
	void InitEndData();

	float GetNorthEnd() { return UpperEnd_; }
	float GetSouthEnd() { return LowerEnd_; }
	float GetEastEnd() { return RightEnd_; }
	float GetWestEnd() { return LeftEnd_; }
};

