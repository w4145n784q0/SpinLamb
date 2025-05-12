#include "Character.h"

Character::Character(GameObject* parent)
	:GameObject(parent,"Character")
{
	csv.Load("CSVdata\\CharacterData.csv");
}

Character::~Character()
{
}

XMFLOAT3 Character::GetStartPosition()
{
	XMFLOAT3 tmp = { 0,0,0 };
	tmp.x = csv.GetValueFloat(2, 2);
	tmp.y = csv.GetValueFloat(2, 3);
	tmp.z = csv.GetValueFloat(2, 4);
	return tmp;
}
