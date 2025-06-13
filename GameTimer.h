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

	//���Ԃ̏��
	enum TimeState {
		STOP = 0,  //�ҋ@��
		COUNTING, //�J�E���g��
		MAXTIME 
	};
	TimeState TimeState_;

public:
	//�R���X�g���N�^
	//�����Fparent  �e�I�u�W�F�N�g�iSceneManager�j
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

	void UpdateTimeStop();
	void UpdateTimeCount();

	void StartTimer() { TimeState_ = COUNTING; }
	void StopTimer() { TimeState_ = STOP; }

	int GetTimeTen() { return Timeten_; }
	int GetTimeOne() { return Timeone_; }
	int GetCurrentGameTime() { return CurrentGameTime_; }
	void SetCurrentGameTime(int _time) { CurrentGameTime_ = _time; }

	/// <summary>
	/// �Q�[�����Ԃ̌v�Z
	/// </summary>
	void TimeCalclation();
};
