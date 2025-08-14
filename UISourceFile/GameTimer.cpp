#include "GameTimer.h"

namespace
{
	//���Ԍv���p�̐��� ���t���[�����Z
	int TimeCounter = 0;
}

GameTimer::GameTimer(GameObject* parent)
	: GameObject(parent,"GameTimer"),
	CurrentGameTime_(0), Timeten_(0), Timeone_(0),
	IsSecondCount_(false),EasingTime_(0),
	TimeState_(S_Stop)
{
}

GameTimer::~GameTimer()
{
}

void GameTimer::Initialize()
{
	TimeCalculation();
}

void GameTimer::Update()
{
	//���݂̎��Ԃ̏�Ԃɂ���čX�V�𕪂���
	switch (TimeState_)
	{
	case GameTimer::S_Stop:
		UpdateTimeStop();
		break;
	case GameTimer::S_Counting:
		UpdateTimeCount();
		break;
	default:
		break;
	}
}

void GameTimer::Draw()
{
}

void GameTimer::Release()
{
}

void GameTimer::UpdateTimeStop()
{
	//��~�����v�Z�͍s��
	TimeCalculation();
}

void GameTimer::UpdateTimeCount()
{
	//���Ԃ̍X�V����
	if (++TimeCounter > oneSecond)
	{
		if (CurrentGameTime_ > 0)
		{
			CurrentGameTime_--;
		}

		//��b�o�߂̓x�Ɍv�Z
		TimeCalculation();

		//���Ԍv���p�̃J�E���^�[�����Z�b�g
		TimeCounter = 0;

		//���̃^�C�~���O��true
		IsSecondCount_ = true;
	}
	else
	{
		//�ʏ��false
		IsSecondCount_ = false;
	}
}

void GameTimer::TimeCalculation()
{
	//���݂̎��Ԃ����ꂼ��v�Z
	//�\�̈�:���݂̎��Ԃ�10�ŏ��Z
	//��̈�:���݂̎��Ԃ�10�ŏ��Z�����]��
	//Timeten,Timeone��HUD�N���X����擾����`�悳���

	Timeten_ = CurrentGameTime_ / TenDivision;
	Timeone_ = CurrentGameTime_ % TenDivision;
}