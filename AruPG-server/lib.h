#include <iostream>
#include <vector>
#include <cstdio>
#include <cstdint>
#include <Windows.h>
//#include <WinSock2.h>
#include <conio.h>
//#include "./SDL2/include/SDL.h"

typedef uint8_t byte;

enum class PlayerStatus
{
    ALIVE,
    DEAD,
    PAUSE,
    GOD
};

struct Abilities
{
    char* name;
    uint16_t damage;
    /*graphics animation; para receber um sprite de animacao - SDL2 */
};

class Player
{
private:
    //graphics person;  /* para receber um sprite de personagem - SDL2 */
    char* name;
    std::string info;
    byte lifes;
    uint16_t hp;    //HARII POTTAA
    uint16_t defense;
    Abilities power[5];
    PlayerStatus status;
    void setInfo();
public:
    Player(char* name,  uint16_t hp, uint16_t defense, char* names[5], int index, uint16_t damage[5]);
    void setPower(char* name, int index, uint16_t damage);
    void setGodMode();
    void hit(uint16_t damage);
    const char* getInfo();
    void start();
};

class Map
{
private:
    //graphics background;  /* para receber uma imagem de fundo - SDL2 */
    
};