#pragma once
#include "Engine/GameObject.h"
#include<vector>

//�X�e�[�W�O�I�u�W�F�N�g���Ǘ�����N���X

class OutStageThing :
    public GameObject
{
private:
	//----------���f���n���h��----------

	//�������f��
	int hCabin_;

	//�؃��f��
	int hTree_;

	//�ۑ����f��
	int hLogs_;

	//�m�����f��
	int hStraw_;

	//�e���f���n���h���z��
	std::vector<int> ModelHandleArray_;

	//----------���f���p�g�����X�t�H�[��----------

	//�����̃g�����X�t�H�[���z��
	std::vector<Transform> CabinData_;

	//�؂̃g�����X�t�H�[���z��
	std::vector<Transform> TreeData_;

	//�ۑ��̃g�����X�t�H�[���z��
	std::vector<Transform> LogsData_;

	//�m���̃g�����X�t�H�[���z��
	std::vector<Transform> StrawData_;

	//�e���f���p�g�����X�t�H�[���z��
	std::vector<std::vector<Transform>*> StageTransArray_;
public:
	OutStageThing(GameObject* parent);
	~OutStageThing();

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

	//ImGui�̕`��
	void DrawImGui();

	//----------�Z�b�^�[�E�Q�b�^�[�֐�----------
	void SetCabinTransform(std::vector<Transform> _data) { CabinData_  =  _data; }

	void SetTreeTransform(std::vector<Transform> _data) { TreeData_  = _data; }

	void SetLogsTransform(std::vector<Transform> _data) { LogsData_ = _data; }

	void SetStrawTransform(std::vector<Transform> _data) { StrawData_ = _data; }
};

