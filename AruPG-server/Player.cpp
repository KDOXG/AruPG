#include "lib.h"

Player::Player(char* name, uint16_t hp, uint16_t defense, char* names[], int16_t damage[5])
{
	strncpy_s(this->name, name, strlen(name));
	this->hp = hp;
	this->defense = defense;
	this->points = 0;
	for (int i = 0; i < 5; i++)
		setPower(names[i], i, damage[i]);
	this->status = PlayerStatus::PAUSE;
	setInfo();
}

bool Player::setPower(char* name, int index, int16_t damage)
{
	if (index < 0 || index >= 5)
		return false;
	strncpy_s(this->power[index].name, name, strlen(name));
	this->power[index].damage = damage;
	//this->power[index].animation = image_anim;
	return true;
}

void Player::hit(int16_t damage)
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

void Player::setGodMode()
{
	this->status = PlayerStatus::GOD;
}

bool Player::isDEAD()
{
	return this->status == PlayerStatus::DEAD ? true : false;
}

bool Player::isPAUSE()
{
	return this->status == PlayerStatus::PAUSE ? true : false;
}

void Player::setInfo()
{
	std::string temp;
	temp = this->name;
	this->info = "Nome: " + temp;
	temp = std::to_string(this->hp);
	this->info = "\nHP: " + temp;
	temp = std::to_string(this->points);
	this->info += "\nPontos: " + temp;
}

const char* Player::getInfo()
{
	return this->info.c_str();
}

const char* Player::getName()
{
	return this->name;
}

void Player::start()
{
	this->status = PlayerStatus::ALIVE;
}