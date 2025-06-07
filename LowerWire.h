#pragma once
#include "Engine/GameObject.h"

//����̍�̓����蔻����s���N���X
class LowerWire :
    public GameObject
{
private:
	XMVECTOR Normal_;//��̖@��
	XMFLOAT3 CollisionPos_;//�����蔻��̈ʒu
	XMFLOAT3 CollisionSize_;//�����蔻��̃T�C�Y
public:
	LowerWire(GameObject* parent);
	~LowerWire();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void OnCollision(GameObject* pTarget) override;
	void InitCollision(XMFLOAT3 pos, XMFLOAT3 size);

	void SetNormal(XMVECTOR _normal) { Normal_ = _normal; }
	XMVECTOR GetNormal() { return Normal_; }
};

