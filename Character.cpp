#include "Character.h"

Character::Character(GameObject* parent)
	:GameObject(parent,"Character")
{
	//csv_.Load("CSVdata\\CharacterDataSample.csv");
}

Character::Character(GameObject* parent, const std::string& name):GameObject(parent, name)
{
}

Character::~Character()
{
}

void Character::CharacterGravity()
{
	JumpSpeed_ -= Gravity_;//重力分の値を引き、プレイヤーは常に下方向に力がかかっている
	this->transform_.position_.y += JumpSpeed_;//フィールドに乗っているかは関係なく重力はかかり続ける

	if (this->transform_.position_.y <= HeightLowerLimit_)//プレイヤーめりこみ防止に一定以下のy座標で値を固定
	{
		this->transform_.position_.y = HeightLowerLimit_;
		IsOnGround_ = true;
	}
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
	FrontDirection_ = { csv.GetValueFloat(1, 10),csv.GetValueFloat(1, 11),csv.GetValueFloat(1, 12) };
	Velocity_ = csv.GetValueFloat(1, 13);
	
    Acceleration_ = csv.GetValueFloat(1, 14);
    AcceleValue_ = csv.GetValueFloat(1, 15);
    FullAccelerate_ = csv.GetValueFloat(1, 16);
	ForwardVector_ = { csv.GetValueFloat(1, 17), csv.GetValueFloat(1, 18), csv.GetValueFloat(1, 19) };

	MoveRotateX = csv.GetValueFloat(1, 20);
    FastRotateX = csv.GetValueFloat(1, 21);

    Gravity_ = csv.GetValueFloat(1, 22);
	IsOnGround_ = false;
    JumpSpeed_ = csv.GetValueFloat(1, 23);
	HeightLowerLimit_ = csv.GetValueFloat(1, 24);
	HeightUpperLimit_ = csv.GetValueFloat(1, 25);

    ColliderSize_ = csv.GetValueFloat(1, 26);
	KnockBack_Direction_ = { csv.GetValueFloat(1, 27), csv.GetValueFloat(1, 28) , csv.GetValueFloat(1, 29) };
    KnockBack_Velocity_ = { csv.GetValueFloat(1, 30), csv.GetValueFloat(1, 31) , csv.GetValueFloat(1, 32) };
	KnockBackPower = csv.GetValueFloat(1, 33);

    InvincibilityTime_ = csv.GetValueFloat(1, 34);
	IsInvincibility_ = false;
    InvincibilityValue = csv.GetValueFloat(1, 35);
}
