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
    GOD
};

enum class AbilityKind
{
    PLAYER,
    AREA
};

struct Abilities
{
    char name[50];
    int16_t damage;
    AbilityKind kind;
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
    int pos_x, pos_y;
    PlayerStatus status;
    void setInfo();
public:
    Player(char* name,  uint16_t hp, uint16_t defense, char* names[], int16_t damage[5], AbilityKind kind = AbilityKind::PLAYER);
    bool setPower(char* name, int index, int16_t damage, AbilityKind kind);
    int16_t getPower(int index, bool* flag);
    void hit(int16_t damage);
    void setGodMode();
    void move(int x, int y);
    int getPosX();
    int getPosY();
    bool isDEAD();
    bool isALIVE();
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
    void setMapEffect(int x, int y, Abilities effect);
    bool removeMapEffect(int x, int y, std::string name);
    void removeMapEffect(int x, int y);
    bool checkMapEffect(int x, int y);
    std::list<Abilities> getMapEffect(int x, int y);
    void playerMove(int old_x, int old_y, int new_x, int new_y, Player* player);
    bool findPlayer(int x, int y, Player* player);
};

void MainGame(uint16_t PORT1, uint16_t PORT2);
void PlayerMove(int player_i);