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
	enum EnemyState {
		S_IDLE = 0,//‘Ò‹@
		S_SUSPICIOUS,//‰ö‚µ‚Ş
		S_CHASE,//’Ç‚¢‚©‚¯‚é
		S_ATTACK,//UŒ‚
		S_MOVE,//ˆÚ“®
		S_WINCE,//‚Ğ‚é‚Ş
		S_MAX
	};
	EnemyState enemy_state;

	XMVECTOR EnemyDirection;//“G‚Ì•ûŒüƒxƒNƒgƒ‹
	XMVECTOR EnemyPosition;//“G‚ÌˆÊ’uƒxƒNƒgƒ‹
	float RotateY;//‰ñ“]Šp“xiŠî–{‚x²‚Ì‚İj
	bool IsTargetted;//“G‚Ì‹ŠE‚É“ü‚Á‚½‚©
	float FrontLength = 5.0f;//“G‚Ì‹ŠE‚Ì’·‚³


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
	bool ComparePosition(XMFLOAT3 pos);
};

