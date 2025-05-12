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
	

	StartPosition_.x = csv.GetValueFloat(1, 1);
	StartPosition_.y = csv.GetValueFloat(1, 2);
	StartPosition_.z = csv.GetValueFloat(1, 3);
	this->transform_.rotate_.x = csv.GetValueFloat(1, 4);
	this->transform_.rotate_.y = csv.GetValueFloat(1, 5);
	this->transform_.rotate_.z = csv.GetValueFloat(1, 6);
	this->transform_.scale_.x = csv.GetValueFloat(1, 7);
	this->transform_.scale_.y = csv.GetValueFloat(1, 8);
	this->transform_.scale_.z = csv.GetValueFloat(1, 9);
	
}
