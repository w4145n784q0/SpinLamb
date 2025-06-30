#pragma once
#include "Engine/GameObject.h"
#include"Wire.h"

//¶‚Ìò‚Ì“–‚½‚è”»’è‚ğs‚¤ƒNƒ‰ƒX
class LeftWire :
    public Wire
{
public:
	LeftWire(GameObject* parent);
	~LeftWire();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

