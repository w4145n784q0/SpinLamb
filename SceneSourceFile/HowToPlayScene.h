#pragma once
#include "../Engine/GameObject.h"
#include"BaseScene.h"
#include"../EffectSourceFile/TransitionEffect.h"

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

	//�����т����V�[��BGM
	int hSoundHowtoPlay_;
	
	//�I��SE
	int hSoundSelect_;

	//�Q�[�����[�h�ɖ߂�SE
	int hSoundBackGameMode_;

	//----------��ԑJ��----------

	//�\���摜�X�e�[�g
	//�����̒l�ɉ�����Draw�ŕ`�悷��摜��ς���
	enum HowtoImage 
	{
		S_Explanation = 0,//�Q�[������
		S_OperateKeyBoard,//������@(�L�[�{�[�h)
		S_OperateController,//������@(�R���g���[��)
		MaxImage,
	};
	HowtoImage ImageState_;

	//�e�摜�̃��X�g
	std::list<HowtoImage> ImageList_;

	//ModeList_�̃C���f�b�N�X���w���l
	std::list<HowtoImage>::iterator itr;

	//----------�C���X�^���X----------
	TransitionEffect* pTransitionEffect_;

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

