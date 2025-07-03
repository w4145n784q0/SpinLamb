#pragma once
#include "Engine/GameObject.h"
#include<vector>

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

	//----------���f���p�g�����X�t�H�[��----------
	std::vector<Transform> CabinData_;

	std::vector<Transform> TreeData_;

	std::vector<Transform> LogsData_;

	std::vector<Transform> StrawData_;
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

	//----------�Z�b�^�[�E�Q�b�^�[�֐�----------
	void SetCabinTransform(std::vector<Transform> _data) { CabinData_  =  _data; }

	void SetTreeTransform(std::vector<Transform> _data) { TreeData_  = _data; }

	void SetLogsTransform(std::vector<Transform> _data) { LogsData_ = _data; }

	void SetStrawTransform(std::vector<Transform> _data) { StrawData_ = _data; }
};

