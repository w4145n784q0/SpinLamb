#pragma once
#include "Engine/GameObject.h"
#include"Wire.h"

//Œã•û‚Ìò‚Ì“–‚½‚è”»’è‚ğs‚¤ƒNƒ‰ƒX
class LowerWire :
    public Wire
{
public:
	LowerWire(GameObject* parent);
	~LowerWire();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

