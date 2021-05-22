#include "lib.h"

Player::Player(char* name, uint16_t hp, uint16_t defense, char* names[], int16_t damage[5], AbilityKind kind)
{
	strncpy_s(this->name, name, strlen(name));
	this->hp = hp;
	this->defense = defense;
	this->points = 0;
	this->pos_x = 0;
	this->pos_y = 0;
	for (int i = 0; i < 5; i++)
		setPower(names[i], i, damage[i], kind);
	this->status = PlayerStatus::ALIVE;
	setInfo();
}

bool Player::setPower(char* name, int index, int16_t damage, AbilityKind kind)
{
	if (index < 0 || index >= 5)
		return false;
	strncpy_s(this->power[index].name, name, strlen(name));
	this->power[index].damage = damage;
	this->power[index].kind = kind;
	//this->power[index].animation = image_anim;
	return true;
}

Abilities Player::getPower(int index)
{
	return this->power[index];
}

AbilityKind Player::getPowerKind(int index)
{
	return this->power[index].kind;
}

void Player::hit(int16_t damage)
{
	if (this->status == PlayerStatus::ALIVE)
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

void Player::move(int x, int y)
{
	this->pos_x = x;
	this->pos_y = y;
}

int Player::getPosX()
{
	return this->pos_x;
}

int Player::getPosY()
{
	return this->pos_y;
}

bool Player::isDEAD()
{
	return this->status == PlayerStatus::DEAD ? true : false;
}

bool Player::isALIVE()
{
	return this->status != PlayerStatus::DEAD ? true : false;
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

uint16_t Player::getHP()
{
	return this->hp;
}

void Player::start()
{
	this->status = PlayerStatus::ALIVE;
}