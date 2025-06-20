#pragma once
#include "Engine/GameObject.h"
#include"BaseScene.h"
#include"HUD.h"
#include"MiniMap.h"

//"�t���[�v���C"����n�܂���K���[�h�V�[��
class PracticeScene :
    public BaseScene
{
private:

	//----------�摜�n���h��----------

	//�^�C�g���摜
	int hBackScreen_;

	//�A�C���X�A�E�g���o�p�摜
	int hBackIrisIn_;

	//----------�T�E���h�n���h��----------
	int hSoundPractice_; 

	//�C���X�^���X
	Player* pPlayer_;
	Enemy* pEnemy_;

	HUD* pHUD_;
	MiniMap* pMiniMap_;

	//�������Ń^�C�g���ɑJ�ڂ���J�E���g
	int Press_;

public:
	//�R���X�g���N�^
	//�����Fparent  �e�I�u�W�F�N�g�iSceneManager�j
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

	void UpdateActive() override;
	void UpdateTransition() override;

	void SetSCVTitle();
};

