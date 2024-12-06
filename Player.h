#pragma once
#include "Engine/GameObject.h"
class Player :
    public GameObject
{
private:
	int hModel_Player;
	bool IsOnGround_;
	float JumpSpeed_;//+‚È‚çƒWƒƒƒ“ƒv‚µ‚Ä‚¢‚éó‘Ô -‚È‚ç‰º~`’n–Ê‚É‚¢‚éó‘Ô
public:
	Player(GameObject* parent);
	~Player();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

