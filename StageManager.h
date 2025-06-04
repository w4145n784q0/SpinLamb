#pragma once
#include "Engine/GameObject.h"
#include"Ground.h"
#include"Fence.h"

//�X�e�[�W�S�̊Ǘ��N���X
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

	XMFLOAT3 WirePosUpper_;//�S���̓����蔻��̈ʒu�i+z�����j
	XMFLOAT3 WirePosLower_;//�S���̓����蔻��̈ʒu�i-z�����j
	XMFLOAT3 WirePosRight_;//�S���̓����蔻��̈ʒu�i+x�����j
	XMFLOAT3 WirePosLeft_;//�S���̓����蔻��̈ʒu�i-x�����j

	XMFLOAT3 WireSizeUpper_;//�S���̓����蔻��̑傫���i+z�����j
	XMFLOAT3 WireSizeLower_;//�S���̓����蔻��̑傫���i-z�����j
	XMFLOAT3 WireSizeRight_;//�S���̓����蔻��̑傫���i+x�����j
	XMFLOAT3 WireSizeLeft_;//�S���̓����蔻��̑傫���i-x�����j

	XMFLOAT3 UpperNormal_;//�X�e�[�W�k�[�̖@���x�N�g��
	XMFLOAT3 LowerNormal_;//�X�e�[�W��[�̖@���x�N�g��
	XMFLOAT3 RightNormal_;//�X�e�[�W���[�̖@���x�N�g��
	XMFLOAT3 LeftNormal_;//�X�e�[�W���[�̖@���x�N�g��

public:
	StageManager(GameObject* parent);
	~StageManager();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void SetSCV();

	/// <summary>
	/// �n�ʃN���X�̃g�����X�t�H�[��������
	/// </summary>
	void InitGroundData();

	/// <summary>
	/// ��N���X�̃g�����X�t�H�[���̏�����
	/// </summary>
	void InitFenceData();

	/// <summary>
	/// ��N���X�̒��̖{���E�ʒu�E�S���N���X�̓����蔻���ݒ�
	/// </summary>
	void InitEndData();

	float GetNorthEnd() { return UpperEnd_; }
	float GetSouthEnd() { return LowerEnd_; }
	float GetEastEnd() { return RightEnd_; }
	float GetWestEnd() { return LeftEnd_; }
};

