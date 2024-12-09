#pragma once
#include "Engine/GameObject.h"
class Enemy :
    public GameObject
{
private:
    int hModel_Enemy;

	enum EnemyState {
		S_IDLE = 0,//ë“ã@
		S_SUSPICIOUS,//âˆÇµÇﬁ
		S_CHASE,//í«Ç¢Ç©ÇØÇÈ
		S_ATTACK,//çUåÇ
		S_MOVE,//à⁄ìÆ
		S_WINCE,//Ç–ÇÈÇﬁ
		S_MAX
	};
	EnemyState enemy_state;

	XMVECTOR EnemyPosition;

public:
	Enemy(GameObject* parent);
	~Enemy();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void UpdateIdle();
	void UpdateSuspicious();
	void UpdateChase();
	void UpdateAttack();
	void UpdateMove();
	void UpdateWince();

	void OnCollision(GameObject* pTarget) override;
	bool ComparePosition(XMFLOAT3 pos);
};

