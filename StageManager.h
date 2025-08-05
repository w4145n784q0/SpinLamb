#pragma once
#include "Engine/GameObject.h"

//�X�e�[�W�S�̊Ǘ��N���X
class StageManager :
    public GameObject
{
private:

	//----------���������ēn���g�����X�t�H�[��----------
	//�n�ʃN���X�ɓn���f�[�^
	Transform GroundData_;

	//��N���X(�S��)�ɓn���f�[�^
	Transform WireData_;

	//��N���X(��)�ɓn���f�[�^
	Transform PillarData_;

	//�X�e�[�W�O�I�u�W�F�N�g�ɓn���f�[�^(����)
	std::vector<Transform>  CabinData_;

	//�X�e�[�W�O�I�u�W�F�N�g�ɓn���f�[�^(��)
	std::vector<Transform>  TreeData_;

	//�X�e�[�W�O�I�u�W�F�N�g�ɓn���f�[�^(�ۑ�)
	std::vector<Transform>  LogsData_;

	//�X�e�[�W�O�I�u�W�F�N�g�ɓn���f�[�^(�m��)
	std::vector<Transform> StrawData_;


	//�X�e�[�W�k�[
	float UpperEnd_;

	//�X�e�[�W��[
	float LowerEnd_; 

	//�X�e�[�W���[
	float RightEnd_;

	//�X�e�[�W���[
	float LeftEnd_;

	//���̐�
	int PillarNum_;

	//----------�����蔻��֌W----------

	//----------�����蔻��̈ʒu----------
	//�S���̓����蔻��̈ʒu�i+z�����j
	XMFLOAT3 WirePosUpper_;

	//�S���̓����蔻��̈ʒu�i-z�����j
	XMFLOAT3 WirePosLower_;

	//�S���̓����蔻��̈ʒu�i+x�����j
	XMFLOAT3 WirePosRight_;

	//�S���̓����蔻��̈ʒu�i-x�����j
	XMFLOAT3 WirePosLeft_;

	//----------�����蔻��̃T�C�Y----------

	//�S���̓����蔻��̑傫���i+z�����j
	XMFLOAT3 WireSizeUpper_;

	//�S���̓����蔻��̑傫���i-z�����j
	XMFLOAT3 WireSizeLower_;

	//�S���̓����蔻��̑傫���i+x�����j
	XMFLOAT3 WireSizeRight_;

	//�S���̓����蔻��̑傫���i-x�����j
	XMFLOAT3 WireSizeLeft_;

	//----------���ꂼ��̍�̖@��----------

	//�X�e�[�W�k�[�̖@���x�N�g��
	XMFLOAT3 UpperNormal_;

	//�X�e�[�W��[�̖@���x�N�g��
	XMFLOAT3 LowerNormal_;

	//�X�e�[�W���[�̖@���x�N�g��
	XMFLOAT3 RightNormal_;

	//�X�e�[�W���[�̖@���x�N�g��
	XMFLOAT3 LeftNormal_;

public:
	StageManager(GameObject* parent);
	~StageManager();

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

	//CSV�t�@�C������K�v�p�����[�^��ǂݍ��݂���
	void SetStageInitCSV();

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

	/// <summary>
	/// �X�e�[�W�O�̃I�u�W�F�N�g�̏�����
	/// </summary>
	void InitOutStageThingData();

	//----------�Z�b�^�[�E�Q�b�^�[�֐�----------

	float GetNorthEnd() const { return UpperEnd_; }
	float GetSouthEnd() const { return LowerEnd_; }
	float GetEastEnd() const { return RightEnd_; }
	float GetWestEnd() const { return LeftEnd_; }
};

