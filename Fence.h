#pragma once
#include "Engine/GameObject.h"

class Fence :
    public GameObject
{
private:
	//----------���f���n���h��----------

	//�����f��
	int hPiller_;

	//�򃂃f��
	int hFence_;

	//----------��̈ʒu----------

	//�S���̃g�����X�t�H�[��
	Transform wire;
	
	//����
	XMFLOAT3 piller_UpperLeft_;

	//�E��
	XMFLOAT3 piller_UpperRight_;

	//����
	XMFLOAT3 piller_LowerLeft_;

	//�E��
	XMFLOAT3 piller_LowerRight_;
	

public:
	Fence(GameObject* parent);
	~Fence();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void OnCollision(GameObject* pTarget) override;

	/// <summary>
	/// 4�̍�̈ʒu����
	/// </summary>
	/// <param name="upper">�k�[</param>
	/// <param name="lower">��[</param>
	/// <param name="left">���[</param>
	/// <param name="right">���[</param>
	/// <param name="height">���̍���</param>
	void SetPiller(float upper, float lower, float left, float right, float height);

	void InitWireTransform(Transform _t) { wire = _t; }

	void InitPillerTransform(Transform _t);
	
};
