#pragma once
#include "Engine/GameObject.h"
class HowToPlayScene :
    public GameObject
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
	//�R���X�g���N�^
	//�����Fparent  �e�I�u�W�F�N�g�iSceneManager�j
	HowToPlayScene(GameObject* parent);

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;
};

