#pragma once
#include "Engine/GameObject.h"
#include"BaseScene.h"
#include"HUD.h"
#include"MiniMap.h"
#include"TransitionEffect.h"

//"�t���[�v���C"����n�܂���K���[�h�V�[��
class PracticeScene :
    public BaseScene
{
private:

	//----------�摜�n���h��----------

	//�^�C�g���摜
	int hBackScreen_;


	//----------�T�E���h�n���h��----------
	int hSoundPractice_; 

	//----------�C���X�^���X----------
	Player* pPlayer1_;
	Player* pPlayer2_;
	Enemy* pEnemy_;
	HUD* pHUD_;
	TransitionEffect* pTransitionEffect_;

	//HUD�N���X�ɓn���|�C���^
	MiniMap* pMiniMap_;

	//�������Ń^�C�g���ɑJ�ڂ���J�E���g
	int Press_;

public:
	PracticeScene(GameObject* parent);
	~PracticeScene();

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

