#include "Character.h"

Character::Character(GameObject* parent)
	:GameObject(parent,"Character")
{
	//csv_.Load("CSVdata\\CharacterDataSample.csv");
}

Character::~Character()
{
}

void Character::SetcsvStatus(std::string _path)
{
	CsvReader csv;
	csv.Load(_path);

	//StartPosition_.x = csv.GetValueFloat(1, 1);
	//StartPosition_.y = csv.GetValueFloat(1, 2);
	//StartPosition_.z = csv.GetValueFloat(1, 3);
	Velocity_ = csv.GetValueFloat(1, 7);
	
}
