#pragma once
#include "../Engine/GameObject.h"

//��̓S���E���̈ʒu��T�C�Y�������N���X
class Fence :
    public GameObject
{
private:
	//----------���f���n���h��----------

	//�����f��
	int hPillar_;

	//�򃂃f��
	int hFence_;

	//----------��̈ʒu----------

	//����
	XMFLOAT3 PillarUpperLeft_;

	//�E��
	XMFLOAT3 PillarUpperRight_;

	//����
	XMFLOAT3 PillarLowerLeft_;

	//�E��
	XMFLOAT3 PillarLowerRight_;
	

public:
	Fence(GameObject* parent);
	~Fence();

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

	//ImGui�̕`��
	void DrawImGui();

	/// <summary>
	/// 4�̍�̈ʒu����
	/// </summary>
	/// <param name="_upper">�k�[</param>
	/// <param name="_lower">��[</param>
	/// <param name="_left">���[</param>
	/// <param name="_right">���[</param>
	/// <param name="_height">���̍���</param>
	void SetPillar(float _upper, float _lower, float _left, float _right, float _height);

	/// <summary>
	///	���̐���ݒ�
	/// </summary>
	/// <param name="num">���̐�</param>
	void SetPillarNum(int _num);

	/// <summary>
	/// �S���̃g�����X�t�H�[����������
	/// </summary>
	/// <param name="_t">StageManager�ŏ����������g�����X�t�H�[��</param>
	void InitWireTransform(Transform _t); 
	
	/// <summary>
	/// ���̃g�����X�t�H�[����������
	/// </summary>
	/// <param name="_t">StageManager�ŏ����������g�����X�t�H�[��</param>
	void InitPillarTransform(Transform _t);
	
	/// <summary>
	/// �e�S���̏Փ˔����ݒ�
	/// </summary>
	/// <param name="_pos">���`�̓����蔻��̈ʒu</param>
	/// <param name="_size">���`�̓����蔻��̑傫��</param>
	/// <param name="_normal">�e�S���N���X�ɓn���@��</param>
	
	void SetWireCollisionUpper(XMFLOAT3 _pos, XMFLOAT3 _size,XMFLOAT3 _normal);

	void SetWireCollisionLower(XMFLOAT3 _pos, XMFLOAT3 _size, XMFLOAT3 _normal);

	void SetWireCollisionLeft(XMFLOAT3 _pos, XMFLOAT3 _size, XMFLOAT3 _normal);

	void SetWireCollisionRight(XMFLOAT3 _pos, XMFLOAT3 _size, XMFLOAT3 _normal);
};
