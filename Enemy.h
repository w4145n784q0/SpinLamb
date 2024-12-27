#pragma once
#include "Engine/GameObject.h"
#include"Player.h"

class Player;

class Enemy :
    public GameObject
{
private:
    int hModel_Enemy;
	Player* pPlayer_;
	enum State {
		S_IDLE = 0,//‘Ò‹@
		S_SUSPICIOUS,//‰ö‚µ‚Ş
		S_CHASE,//’Ç‚¢‚©‚¯‚é
		S_ATTACK,//UŒ‚
		S_MOVE,//ˆÚ“®
		S_WINCE,//‚Ğ‚é‚Ş
		S_MAX
	};
	State EnemyState;

	XMVECTOR EnemyFrontDirection;//“G‚Ì³–ÊƒxƒNƒgƒ‹
	XMVECTOR EnemyPosition;//“G‚ÌˆÊ’uƒxƒNƒgƒ‹
	float RotateY;//‰ñ“]Šp“xiŠî–{‚x²‚Ì‚İj
	float FrontLength = 10.0f;//“G‚Ì‹ŠE‚Ì’·‚³
	float Fov;
	bool IsHit_;

	XMFLOAT3 pPosition;
	XMVECTOR pPositionVec;

	XMFLOAT3 ChasePoint;

	XMVECTOR sightLength;//“G‚Ì‹ŠEiîŒ^‚Ì‹ŠEj‚Ì’·‚³

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
	int GetModelHandle() { return hModel_Enemy; }
};

