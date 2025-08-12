#pragma once
#include "Engine/GameObject.h"
#include"Player.h"
#include"Enemy.h"
#include"HUD.h"
#include"Ground.h"
#include"Fence.h"
#include"OutStageThing.h"

//-----------------------------------------------------------
// ��ʕ����E�r���[�|�[�g�ݒ�E�J�����ݒ蓙���s�����O���
// ��ʕ����֘A�̏����͂����ōs��
//-----------------------------------------------------------
namespace GameView
{
	//��ʕ`�惂�[�h
	enum GameViewMode
	{
		S_NormalScreen = 0,//�^�C�g��,�Q�[�����[�h�Ȃǂ̑S�̉��
		S_Single,//��l�v���C���̕`��
		S_Dual,//��l�v���C���̕`��
		MaxScreen
	};

	//�Q�[����ʂ̉f�����̏�����
	void Initialize();

	//��ʑS�̕`��(���j���[�ȂǂɎg�p)
	void ViewNormal();

	//���
	void Release();

	//��l�v���C�p�`��(�v���C���[�̔w���ɃJ������u��)
	void ViewPvE();

	//��l�v���C�p�`��(����ʕ���)
	void ViewPlayer1();

	//��l�v���C�p�`��(�E��ʕ���)
	void ViewPlayer2();

	//ImGui�`��
	void ViewImGui();

	//HUD�p�`��(��ʑS��)
	void ViewHUDNormal();

	///��ʕ`�惂�[�h����l�v���C�����ǂ����Ԃ�
	bool IsSingle();

	///��ʕ`�惂�[�h����l�v���C�����ǂ����Ԃ�
	bool IsDual();

	//�`�惂�[�h��ݒ�
	void SetGameViewMode(GameViewMode mode);

	//----------�|�C���^�Z�b�^�[�֐�----------

	//�v���C���[�̃C���X�^���X��z��Ŏ󂯎��ݒ�(Battle,Practice�ŌĂ�)
	void SetPlayers(std::vector<Player*> _players);

	//HUD�̃C���X�^���X��ݒ�
	void SetHUD(HUD* _HUD);

	//Enemy�̃C���X�^���X��ݒ�
	void SetEnemy(Enemy* _enemy);

	//Ground�̃C���X�^���X��ݒ�
	void SetGround(Ground* _ground);

	//Fence�̃C���X�^���X��ݒ�
	void SetFence(Fence* _fence);

	//OutStageThing�̃C���X�^���X��ݒ�
	void SetOutStageThing(OutStageThing* _stagething);
};

