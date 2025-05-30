#pragma once
#include "Engine/GameObject.h"
class SceneData :
    public GameObject
{
protected:
	XMFLOAT3 ResultPosition_ = { 0,0,0 }; //���ʉ�ʂ̃��S�̈ʒu
	XMFLOAT3 UnderPosition_ = { 0,0,0 }; //���ʉ�ʂ̉��̃��S�̈ʒu
	int practiceEnd_ = 0; //���K���[�h�̏I�����̃J�E���g

public:
	SceneData(GameObject* parent);
	SceneData(GameObject* parent, const std::string& name);
	virtual ~SceneData();

	void SetCSV();
	int GetPracticeEnd() { return practiceEnd_; }
	XMFLOAT3 GetResultPosition() { return ResultPosition_; }
	XMFLOAT3 GetUnderPosition() { return UnderPosition_; }
};

