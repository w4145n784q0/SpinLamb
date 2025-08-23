#pragma once
#include <string>

//-----------------------------------------------------------
//�T�E���h���Ǘ�����iXAudio�g�p�j
//-----------------------------------------------------------
namespace Audio
{
	//������
	void Initialize();

	//�T�E���h�t�@�C��(.wav�j�����[�h
	//���łɓ������O�̃t�@�C�������[�h�ς݂̏ꍇ�́A�����̃f�[�^�̔ԍ���Ԃ�
	//�����FfileName	�t�@�C����
	//�����FisLoop		���[�v�Đ����������Ȃ�true
	//�����FsvNum�@		�����ɖ炷�ő吔�i�ȗ��j
	//�ߒl�F���̃f�[�^�Ɋ��蓖�Ă�ꂽ�ԍ�
	int Load(std::string fileName, bool isLoop = false, int svNum = 1);

	//�Đ�
	//�����Fhandle	�炵�����T�E���h�̔ԍ�
	void Play(int ID);

	//��~
	//�����Fhandle	�~�߂����T�E���h�̔ԍ�
	void Stop(int ID);

	//�V�[�����Ƃ̉���i���[�h�����T�E���h������j
	void Release();

	//�{�̂����
	void AllRelease();

	//�炷�񐔂�CSV�ǂݍ���
	void InitCSVAudio();

	//----------SE��炷�񐔂̃Q�b�^�[�֐�----------

	//�X�^�[�g���𓯎��ɖ炷��
	int GetStartNum();

	//�I�����𓯎��ɖ炷��
	int GetSelectNum();

	//���艹�𓯎��ɖ炷��
	int GetDecideNum();

	//�L�����Z�����𓯎��ɖ炷��
	int GetCancelNum();

	//�z�C�b�X�����𓯎��ɖ炷��
	int GetWhistleNum();

	//�`���[�W���𓯎��ɖ炷��
	int GetChargeNum();

	//�U�����𓯎��ɖ炷��
	int GetAttackNum();

	//�q�b�g���𓯎��ɖ炷��
	int GetCollisionNum();

	//�W�����v���𓯎��ɖ炷��
	int GetJumpNum();
};

