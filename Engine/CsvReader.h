#pragma once
#include <vector>
#include <string>

//-----------------------------------------------------------
//CSV�t�@�C���������N���X
//-----------------------------------------------------------
class CsvReader
{
	//�ǂݍ��񂾃f�[�^�����Ă���2�����z��
	std::vector<std::vector<std::string>> data_;

	//�u,�v���u���s�v�܂ł̕�������擾
	//�����Fresult	���ʂ�����A�h���X
	//�����Fdata	���Ƃ̕�����f�[�^
	//�����Findex	�������ڂ��璲�ׂ邩
	//�ߒl�F�Ȃ�
	void GetToComma(std::string *result, std::string data, DWORD* index);

public:
	//�R���X�g���N�^
	CsvReader();

	//�f�X�g���N�^
	~CsvReader();

	//CSV�t�@�C���̃��[�h
	//�����FfileName�@�t�@�C����
	//�ߒl�F������true�@���s��false
	bool Load(std::string fileName);


	//�w�肵���ʒu�̃f�[�^�𕶎���Ŏ擾
	//�����Fx,y�@�擾�������ʒu
	//�ߒl�F�擾����������
	std::string GetString(DWORD x, DWORD y);

	//�w�肵���ʒu�̃f�[�^�𐮐��Ŏ擾
	//�����Fx,y�@�擾�������ʒu
	//�ߒl�F�擾�����l
	int GetValue(DWORD x, DWORD y);

	float GetValueFloat(DWORD x, DWORD y);

	//�t�@�C���̗񐔂��擾
	//�����F�Ȃ�
	//�ߒl�F��
	size_t GetWidth();

	//�t�@�C���̍s�����擾
	//�����F�Ȃ�
	//�ߒl�F�s��
	size_t GetHeight();

	/// <summary>
	/// CSV�̍s�̒��g���擾
	/// </summary>
	/// <param name="ParamName">csv��0��ڂ̕�����</param>
	/// <returns>�擾����vector�z��</returns>
	std::vector<float> GetParam(std::string ParamName);

	/// <summary>
	/// �w�肵�����O��csv�ɑ��݂��邩
	/// </summary>
	/// <param name="ParamName">0��ڂ̃p�����[�^�̖��O</param>
	/// <returns></returns>
	bool IsGetParamName(std::string ParamName);
};

