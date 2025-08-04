#pragma once

//�C�[�W���O�������s��
//�g�p�����T�C�g: https://easings.net/ja
namespace Easing
{
	//----------�C�[�W���O�֐�(��{�I�Ȍv�Z)----------

	//�ŏ��������₩�ɉ����A���X�Ɉ��̑��x
	double EaseInSine(double x);

	//�ŏ��͈��̑��x�ŁA���X�ɂ��₩�Ɍ���
	double EaseOutSine(double x);

	//�ŏ��͂��₩�ɉ����A���X�Ɉ��̑��x�A�Ō�Ɍ���
	double EaseInOutSine(double x);

	//�n�߂͊ɂ₩�ɁA���X�ɉ�������
	double EaseInQuad(double x);

	//���X�ɉ������A�Ō�Ɋɂ₩�Ɍ�������
	double EaseOutQuad(double x);

	//�ɂ₩�ɉ��������̑��x���ɂ₩�Ɍ�������
	double EaseInOutQuad(double x);

	//�n�߂͊ɂ₩�ɁA���̃��C���ő傫������
	double EaseInCubic(double x);

	//�n�߂ɑ傫���������A���X�Ɋɂ₩�ɂȂ�
	double easeOutCubic(double x);

	//�ɂ₩�ɉ����A���̃��C���ő傫�����A�ɂ₩�Ɍ�������
	double EaseInOutCubic(double x);

	//�n�߂͊ɂ₩�ɁA���̃��C���ň�C�ɉ���
	double EaseInQuart(double x);

	//�n�߂Ɉ�C�ɉ������A��C�Ɍ�������
	double EaseOutQuart(double x);

	//�ɂ₩�ɉ��������Ԃň�C�ɉ�������C�Ɍ�������
	double EaseInOutQuart(double x);

	//�n�߂͂��܂蓮�����A�㔼�ŋ}����
	double EaseInQuint(double x);

	//�n�߂ɋ}�������A�㔼�͂��܂蓮���Ȃ�
	double EaseOutQuint(double x);

	//�n�߂͂��܂蓮���Ȃ������Ԃŋ}�������㔼���܂蓮���Ȃ�
	double EaseInOutQuint(double x);

	//�n�߂͂قړ������㔼�ɋ}��������
	double EaseInExpo(double x);

	//�n�߂ɋ}�������A�㔼�͂قړ����Ȃ�
	double EaseOutExpo(double x);

	//�n�߂͂قړ������A�㔼�ɋ}�������A�Ō�Ɍ�������
	double EaseInOutExpo(double x);

	//�n�߂͊ɂ₩�ɏ㏸���A�㔼�ɋ}����
	double EaseInCirc(double x);

	//�n�߂͋}�������A�㔼�͊ɂ₩�ɏ㏸
	double EaseOutCirc(double x);

	//�ɂ₩�ɉ��������Ԃŋ}�������㔼�͊ɂ₩�ɏ㏸
	double EaseInOutCirc(double x);

	//�n�߂͉��~���A�}�㏸����
	double EaseInBack(double x);

	//�傫���㏸���Ă���߂�
	double easeOutBack(double x);

	//���~���}�㏸�����~����
	double EaseInOutBack(double x);

	//�n�߂͊ɂ₩�ɁA���X�ɐU������
	double EaseInElastic(double x);

	//�n�߂͐U�����A���X�Ɋɂ₩�Ɍ�������
	double EaseOutElastic(double x);

	//�ɂ₩�ɉ������U�������X�Ɍ�������
	double EaseInOutElastic(double x);

	//���X�ɒ��ˏオ��e���̓���
	double EaseInBounce(double x);

	//���X�Ɍ�������e���̓���
	double EaseOutBounce(double x);

	//���X�ɒ��ˏオ��A���X�Ɍ�������e���̓���
	double EaseInOutBounce(double x);

	//----------�C�[�W���O�֐����p(�K�v�ɉ����Ēǉ�)----------

	/// <summary>
	/// �g�嗦�v�Z�֐�
	/// </summary>
	/// <param name="_MaxScale">�ŏI�I�ȍő�g��l</param>
	/// <param name="_EasingCount">�A�j���[�V�����i�s�x</param>
	/// <returns>�g�劄��</returns>
	double calculateScale(float _MaxScale,float _EasingCount);

}

