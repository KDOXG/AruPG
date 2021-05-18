#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <thread>
#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstdint>
#include <Windows.h>
//#include <WinSock2.h>
#include <conio.h>
//#include "./SDL2/include/SDL.h"

#define CYCLE_SIZE 10
#define string_equal(s,se) !strcmp(s, se)

#define PLAYER_N 2

enum class PlayerStatus
{
    ALIVE,
    DEAD,
    PAUSE,
    GOD
};

struct Abilities
{
    char name[50];
    int16_t damage;
    /*graphics animation; para receber um sprite de animacao - SDL2 */
};

class Player
{
private:
    //graphics person;  /* para receber um sprite de personagem - SDL2 */
    char name[50];
    std::string info;
    uint16_t hp;    //HARII POTTAA
    uint16_t defense;
    int64_t points;
    Abilities power[5];
    PlayerStatus status;
    void setInfo();
public:
    Player(char* name,  uint16_t hp, uint16_t defense, char* names[], int16_t damage[5]);
    bool setPower(char* name, int index, int16_t damage);
    int16_t getPower(int index);
    void hit(int16_t damage);
    void setGodMode();
    bool isDEAD();
    bool isPAUSE();
    const char* getInfo();
    const char* getName();
    void start();
};

struct Elements
{
    std::list<Player*> people;
    std::list<Abilities> effect;
};

class Map
{
private:
    //graphics background;  /* para receber uma imagem de fundo - SDL2 */
    Elements area[100][100];
public:
    Map(/*graphics background*/);
    int setMapEffect(int x, int y);
    int getPlayers(int x, int y);
};

//std::atomic<bool> flag1, flag2, flag3;
//std::atomic<Player*> playerList[10];

void MainGame(uint16_t PORT1, uint16_t PORT2);
void PlayerMove(int player_i);