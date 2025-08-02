#pragma once

//�C�[�W���O�������s��
namespace Easing
{
	//----------�C�[�W���O�֐�----------

	//�傫���o�Ă���߂铮��
	double easeOutBack(double x);

	//�n�߂ɉ�������������
	double easeOutCubic(double x);

	//----------�C�[�W���O�֐����p----------

	/// <summary>
	/// �g�嗦�v�Z�֐�
	/// </summary>
	/// <param name="_MaxScale">�ŏI�I�ȍő�g��l</param>
	/// <param name="_EasingCount">�A�j���[�V�����i�s�x</param>
	/// <returns>�g�劄��</returns>
	double calculateScale(float _MaxScale,float _EasingCount);

	//CSV�t�@�C������K�v�p�����[�^��ǂݍ��݂���
	void SetCSVEasing();
}

