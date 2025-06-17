#pragma once
#include "Engine/GameObject.h"
#include"BaseScene.h"

//�����т��������V�[��
class HowToPlayScene :
    public BaseScene
{
private:
	//----------�摜�n���h��----------

	//���[������
	int hExplanation_;

	//�L�[�{�[�h�������
	int hOperateKeyboard_;

	//�R���g���[���[�������
	int hOperateController_;

	//----------�T�E���h�n���h��----------
	int hSoundHowtoPlay_;
	
	//�\���摜�X�e�[�g
	enum HowtoImage 
	{
		Explanation = 0,
		OperateKeyBoard,
		OperateController,
	};
	HowtoImage ImageState_;

	std::list<HowtoImage> ImageList_;//�e�摜�̃��X�g
	std::list<HowtoImage>::iterator itr;//ModeList_�̃C���f�b�N�X���w���l

public:
	HowToPlayScene(GameObject* parent);
	~HowToPlayScene();

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

	void UpdateSelect() override;
	void UpdateDecide() override;
};

