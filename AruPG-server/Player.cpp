#include "lib.h"

Player::Player(char* name, uint16_t hp, uint16_t defense, char* names[], int16_t damage[5])
{
	strncpy_s(this->name, name, strlen(name));
	this->hp = hp;
	this->defense = defense;
	this->points = 0;
	for (int i = 0; i < 5; i++)
		setPower(_strdup(names[i]), i, damage[i]);
	this->status = PlayerStatus::PAUSE;
	setInfo();
}

Player::Player(const Player& newPlayer)
{
	if (!string_equal(this->name, newPlayer.name)) strncpy_s(this->name, name, strlen(name));
	this->hp = newPlayer.hp;
	this->defense = newPlayer.defense;
	this->points = newPlayer.points;
	for (int i = 0; i < 5; i++)
		setPower(_strdup(newPlayer.power[i].name), i, newPlayer.power[i].damage);
	this->status = newPlayer.status;
	this->setInfo();
}

Player::Player(const Player&& newPlayer) noexcept
			: name()
			, info(newPlayer.info)
			, hp(newPlayer.hp)
			, defense(newPlayer.defense)
			, points(newPlayer.points)
			, status(newPlayer.status)
			, power()
{ }

Player::~Player()
{ }

Player& Player::operator=(Player& newPlayer)
{
	if (!string_equal(this->name, newPlayer.name)) strncpy_s(this->name, name, strlen(name));
	this->hp = newPlayer.hp;
	this->defense = newPlayer.defense;
	this->points = newPlayer.points;
	for (int i = 0; i < 5; i++)
		if (!(string_equal(this->power[i].name, newPlayer.power[i].name)
			&& this->power[i].damage == newPlayer.power[i].damage))
			setPower(_strdup(newPlayer.power[i].name), i, newPlayer.power[i].damage);
	this->status = newPlayer.status;
	this->setInfo();
	return *this;
}

Player& Player::operator=(Player&& newPlayer) noexcept
{
	if (this != &newPlayer)
	{
		if (!string_equal(this->name, newPlayer.name)) strncpy_s(this->name, name, strlen(name));
		this->hp = newPlayer.hp;
		this->defense = newPlayer.defense;
		this->points = newPlayer.points;
		for (int i = 0; i < 5; i++)
			if (!(string_equal(this->power[i].name, newPlayer.power[i].name)
				&& this->power[i].damage == newPlayer.power[i].damage))
				setPower(_strdup(newPlayer.power[i].name), i, newPlayer.power[i].damage);
		this->status = newPlayer.status;
		this->setInfo();
	}
	return *this;
}

bool Player::setPower(char* name, int index, int16_t damage)
{
	if (index < 0 || index >= 5)
	{
		free(name);
		return false;
	}
	strncpy_s(this->power[index].name, name, strlen(name));
	free(name);
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