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

	//�e�摜�̃��X�g
	std::list<HowtoImage> ImageList_;

	//ModeList_�̃C���f�b�N�X���w���l
	std::list<HowtoImage>::iterator itr;

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

	//----------BaseScene�̌p���֐�----------
	//�ʏ�̏���
	void UpdateActive() override;

	//�V�[���J�ڒ��̏���
	void UpdateTransition() override;
};

