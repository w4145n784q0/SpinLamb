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
	XMFLOAT3 Pillar_UpperLeft_;

	//�E��
	XMFLOAT3 Pillar_UpperRight_;

	//����
	XMFLOAT3 Pillar_LowerLeft_;

	//�E��
	XMFLOAT3 Pillar_LowerRight_;
	

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
	/// <param name="upper">�k�[</param>
	/// <param name="lower">��[</param>
	/// <param name="left">���[</param>
	/// <param name="right">���[</param>
	/// <param name="height">���̍���</param>
	void SetPillar(float upper, float lower, float left, float right, float height);

	/// <summary>
	///	���̐���ݒ�
	/// </summary>
	/// <param name="num">���̐�</param>
	void SetPillarNum(int num);

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
	/// <param name="pos">���`�̓����蔻��̈ʒu</param>
	/// <param name="size">���`�̓����蔻��̑傫��</param>
	/// <param name="normal">�e�S���N���X�ɓn���@��</param>
	
	void SetWireCollisionUpper(XMFLOAT3 pos, XMFLOAT3 size,XMFLOAT3 normal);

	void SetWireCollisionLower(XMFLOAT3 pos, XMFLOAT3 size, XMFLOAT3 normal);

	void SetWireCollisionLeft(XMFLOAT3 pos, XMFLOAT3 size, XMFLOAT3 normal);

	void SetWireCollisionRight(XMFLOAT3 pos, XMFLOAT3 size, XMFLOAT3 normal);
};
