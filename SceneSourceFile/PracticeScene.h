#pragma once
#include "../Engine/GameObject.h"
#include"BaseScene.h"
#include"../CharacterSourceFile/Player.h"
#include"../CharacterSourceFile/Enemy.h"
#include"../UISourceFile/MiniMap.h"
#include"../UISourceFile/HUD.h"
#include"../EffectSourceFile/TransitionEffect.h"
#include"PlayScene.h"

//"�t���[�v���C"����n�܂���K���[�h�V�[��
class PracticeScene :
    public PlayScene
{
private:

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

	//���ۂɓ����v���C���[(CPU�ł͂Ȃ�)�̃��X�g
	std::vector<Player*> ActivePlayers_;

	//�������ꂽ�G(CPU)��o�^����z��
	std::vector<Enemy*> ActiveEnemys_;

	//----------��ԑJ��----------

	//���K�V�[���̏�ԑJ��
	//�����̒l�ɉ����ĕ`��w��
	//PracticeScene�ł͂��̃V�[�����L��Update�֐��͂Ȃ�
	enum PracticeState
	{
		S_Now = 0,		//���K��
		S_Pause,		//�|�[�Y��
		MaxPracticeMode
	};
	PracticeState PracticeState_;

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

	//�~�߂Ă���Ƃ��̏���
	void UpdateInActive() override;

	//�V�[���J�ڒ��̏���
	void UpdateTransition() override;

	//----------PlayScene�̌p���֐�----------

	void GotoPause() override;

	void GotoPlay() override;

	void GotoTitle() override;

	void SetPauseIconY() override;

	//CSV����K�v�p�����[�^��ǂݍ��݂���
	void SetCSVPractice();
};

