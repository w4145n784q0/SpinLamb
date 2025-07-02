#pragma once
#include "Engine/GameObject.h"
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
	Transform CabinTransform;

	Transform TreeTransform;

	Transform LogsTransform;
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
	void SetCabinTransform(Transform _transform) { CabinTransform = _transform; }

	void SetTreeTransform(Transform _transform) { TreeTransform = _transform; }

	void SetLogsTransform(Transform _transform) { LogsTransform = _transform; }
};

