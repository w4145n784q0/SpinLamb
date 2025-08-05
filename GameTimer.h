#pragma once
#include "Engine/GameObject.h"

//���Ԃ̌v�Z���s���N���X
//�������Ԃ͕ʃN���X����󂯎��
class GameTimer :
    public GameObject
{
private:

	//----------���ԕ`��----------

	//���݂̎���
	int CurrentGameTime_;

	//10�̈ʂ̒l
	int Timeten_;

	//1�̈ʂ̒l
	int Timeone_;

	//���̃t���[���ŕb�J�E���g�����Ă��邩
	//�b�J�E���g�����^�C�~���O��true
	bool IsSecondCount_;

	//�C�[�W���O����(�c��n�b�ɂȂ����當�����g��\�L)�����鎞��
	//BattleScene����󂯎��(GameTimer�̒��ł͒�`���Ȃ�)
	int EasingTime_ = 0;

	//���Ԃ̏��
	//�����̒l�ɉ����ĊeUpdate�֐����Ăяo��
	enum TimeState
	{
		STOP = 0,  //�ҋ@��
		COUNTING, //�J�E���g��
		MAXTIME 
	};
	TimeState TimeState_;

public:
	GameTimer(GameObject* parent);
	~GameTimer();

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

	//----------TimeState_�ɉ����ē��e���ς��Update�֐�----------
	//���Ԍv���X�g�b�v
	void UpdateTimeStop();

	//���Ԍv����
	void UpdateTimeCount();

	//�Q�[�����Ԃ̌v�Z
	void TimeCalculation();

	//�C�[�W���O�������s�����Ԃ��ǂ�����ݒ�
	bool IsEasingTime() {if (CurrentGameTime_ < EasingTime_) return true; else return false;}

	//----------�Z�b�^�[�E�Q�b�^�[�֐�----------
	int GetTimeTen() const { return Timeten_; }
	int GetTimeOne() const { return Timeone_; }
	int GetCurrentGameTime() const { return CurrentGameTime_; }
	void SetCurrentGameTime(int _time) { CurrentGameTime_ = _time; }
	void SetEasingTime(int _time) { EasingTime_ = _time; }
	int GetEasingTime() const { return EasingTime_; }
	bool GetIsSecondCount() const { return IsSecondCount_; }

	//���Ԍv�������ǂ����Ԃ�
	bool IsCounting() const { if (TimeState_ == COUNTING) return true; else return false; }

	//���Ԓ�~���w��
	void StartTimer() { TimeState_ = COUNTING; }

	//���Ԍv�����w��
	void StopTimer() { TimeState_ = STOP; }
};
