#pragma once
#include "Engine/GameObject.h"
#include"Player.h"
#include"HUD.h"
#include"Engine/Direct3D.h"

#include"Engine/Camera.h"
//-----------------------------------------------------------
//��ʕ����E�`��
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

	//��ʑS�̕`��(���j���[�Ȃǎg�p)
	void ViewNormal();

	void Release();

	//��l�v���C�p�`��(�v���C���[�̔w���ɃJ������u��)
	void ViewPvE();

	//��l�v���C�p�`��(����ʕ���)
	void ViewPlayer1();

	//��l�v���C�p�`��(�E��ʕ���)
	void ViewPlayer2();

	//HUD�p�`��(��ʑS��)
	void ViewHUDNormal();

	///��ʕ`�惂�[�h����l�v���C�����ǂ����Ԃ�
	bool IsSingle();

	///��ʕ`�惂�[�h����l�v���C�����ǂ����Ԃ�
	bool IsDual();

	//�`�惂�[�h��ݒ�
	void SetGameViewMode(GameViewMode mode);

	//�v���C���[1�̃C���X�^���X��ݒ�(Battle,Practice�ŌĂ�)
	void SetPlayer1(Player* _pPlayer);

	//�v���C���[2�̃C���X�^���X��ݒ�(Battle,Practice�ŌĂ�)
	void SetPlayer2(Player* _pPlayer);

	void SetPlayers(std::vector<Player*> _players);

	//HUD�̃C���X�^���X��ݒ�
	void SetHUD(HUD* _HUD);
};

