#include "lib.h"

Player::Player(char* name, uint16_t hp, uint16_t defense, char* names[5], int index, uint16_t damage[5])
{
	this->name = name;
	this->lifes = 0;
	this->hp = hp;
	this->defense = defense;
	for (int i = 0; i < 5; i++)
		setPower(names[i], i, damage[i]);
	this->status = PlayerStatus::PAUSE;
	setInfo();
}

void Player::setPower(char* name, int index, uint16_t damage)
{
	this->power[index].name = name;
	this->power[index].damage = damage;
	//this->power[index].animation = image_anim;
}

void Player::setGodMode()
{
	this->status = PlayerStatus::GOD;
}

void Player::hit(uint16_t damage)
{
	if (this->status != PlayerStatus::GOD)
		if (damage >= this->defense)
			if (damage - this->defense >= this->hp)
			{
				this->hp = 0;
				this->status = PlayerStatus::DEAD;
				setInfo();
			}
			else
			{
				this->hp -= (damage - this->defense);
				setInfo();
			}
}

void Player::setInfo()
{
	std::string temp;
	temp = this->name;
	this->info = "Nome: " + temp;
	temp = this->lifes;
	this->info += "\nVidas extras: " + temp;
	temp = this->hp;
	this->info = "\nHP: " + temp;
}

const char* Player::getInfo()
{
	return this->info.c_str();
}

void Player::start()
{
	this->status = PlayerStatus::ALIVE;
}