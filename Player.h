#pragma once
#include "Engine/GameObject.h"
class Player :
    public GameObject
{
private:
	int hModel_Player;
	bool IsOnGround_;
	float JumpSpeed_;//+ならジャンプしている状態 -なら下降～地面にいる状態

	bool IsDash_;//ダッシュ関係
	float Acceleration_;

	XMFLOAT3 PlayerDirection;
public:
	Player(GameObject* parent);
	~Player();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

