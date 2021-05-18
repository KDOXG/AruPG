// AruPG-server.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#include "lib.h"
#pragma comment(lib,"Ws2_32.lib")

Player* playerList[2];
bool playerSet[2] = { false };
std::string playerMessage[2];
std::string playerLog;
std::string enviar;
char receber[128];

int main(int argc, char* argv[])
{
    // Application params

    std::string message;

    // Initializing WinSock
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        std::cout << "Failed. Error Code : " << WSAGetLastError();
        return 1;
    }
    // For Server Instance
    SOCKET ServerMain, ConnectionWithClient;
    sockaddr_in socketInfo, socketClient;
    int clientSize = sizeof(sockaddr_in);

    // List of Ports
    std::vector<uint16_t> PORTS;
    PORTS.push_back(6000);
    PORTS.push_back(7000);
    
    // Instancing the server socket
    ServerMain = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Initializing the server params
    socketInfo.sin_addr.s_addr = INADDR_ANY;
    socketInfo.sin_family = AF_INET;
    socketInfo.sin_port = htons(PORTS[0]);

    //char ip[100] = "192.168.100.34";
    //socketInfo.sin_addr.s_addr = inet_addr(ip);

    // Initializing the server bind
    if (bind(ServerMain, (sockaddr*)&socketInfo, sizeof(socketInfo)) == SOCKET_ERROR)
    {
        std::cout << "Bind failed with error code : " << WSAGetLastError();
    }

    // Start listening
    listen(ServerMain, SOMAXCONN);

    // Server Main Loop
    ConnectionWithClient = accept(ServerMain, (sockaddr*)&socketClient, &clientSize);
    for (int count=1; count <= 2; count++)
    {
        //ConnectionWithClient = accept(ServerMain, (sockaddr*)&socketClient, &clientSize);
        std::cout << "Connection Accepted! " << 2-count << " player remains...\n";

        message = std::to_string(PORTS[count-1]);
        
        send(ConnectionWithClient, message.c_str(), message.size(), 0);
    }

    closesocket(ServerMain);

    MainGame(PORTS[0], PORTS[1]);
    
    WSACleanup();
    
    return 0;
}

void MainGame(uint16_t PORT1, uint16_t PORT2)
{
    //For Server
    SOCKET server1, connection1;
    SOCKET server2, connection2;
    sockaddr_in socketInfo1, socketClient1;
    sockaddr_in socketInfo2, socketClient2;
    int clientSize = sizeof(sockaddr_in);

    server1 = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    socketInfo1.sin_addr.s_addr = INADDR_ANY;
    socketInfo1.sin_family = AF_INET;
    socketInfo1.sin_port = htons(PORT1);

    if (bind(server1, (sockaddr*)&socketInfo1, sizeof(socketInfo1)) == SOCKET_ERROR)
    {
        std::cout << "Bind failed with error code : " << WSAGetLastError();
        return;
    }
    listen(server1, SOMAXCONN);

    server2 = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    socketInfo2.sin_addr.s_addr = INADDR_ANY;
    socketInfo2.sin_family = AF_INET;
    socketInfo2.sin_port = htons(PORT2);

    if (bind(server2, (sockaddr*)&socketInfo2, sizeof(socketInfo2)) == SOCKET_ERROR)
    {
        std::cout << "Bind failed with error code : " << WSAGetLastError();
        return;
    }
    listen(server2, SOMAXCONN);

    //Main loop
    connection1 = accept(server1, (sockaddr*)&socketClient1, &clientSize);
    connection2 = accept(server2, (sockaddr*)&socketClient2, &clientSize);
    for(clock_t clk = clock(), end_clk; true; clk = clock())
    {
        //LOG
        enviar = playerLog;
        send(connection1, enviar.c_str(), enviar.size(), 0);
        send(connection2, enviar.c_str(), enviar.size(), 0);
        playerLog = "";

        if (playerSet[0])
        {
            //MAP
            enviar = "MAP";
            send(connection1, enviar.c_str(), enviar.size(), 0);

            //MSG
            enviar = playerMessage[0];
            send(connection1, enviar.c_str(), enviar.size(), 0);
            playerMessage[0] = "";

            recv(connection1, receber, 128, 0);
            PlayerMove(0);
            send(connection1, enviar.c_str(), enviar.size(), 0);
        }

        if (playerSet[1])
        {
            //MAP
            enviar = "MAP";
            send(connection2, enviar.c_str(), enviar.size(), 0);

            //MSG
            enviar = playerMessage[1];
            send(connection2, enviar.c_str(), enviar.size(), 0);
            playerMessage[1] = "";

            recv(connection2, receber, 128, 0);
            PlayerMove(1);
            send(connection2, enviar.c_str(), enviar.size(), 0);
        }
        if (!(playerSet[0] || playerSet[1]))
            break;
        
        do end_clk = clock(); while ((float)(end_clk - clk) / CLOCKS_PER_SEC < CYCLE_SIZE);
    }

    enviar = "Game over.";
    send(connection1, enviar.c_str(), enviar.size(), 0);
    send(connection2, enviar.c_str(), enviar.size(), 0);
    closesocket(server1);
    closesocket(server2);
}

void PlayerMove(int player_i)
{
    //For Application
    std::string tmp;
    std::string recebido, param1, param2, param3;

    char* temp_for_malloc;

    //Apaga seu personagem e sai do jogo
    if (string_equal(receber, "QUIT"))
    {
        playerLog = "LOG /Player ";
        if (playerSet[player_i])
        {
            playerLog += playerList[player_i]->getName();
            //playerList[player_i]->hit(MAXINT16);
            playerSet[player_i] = false;
        }
        else
            playerLog += std::to_string(player_i);
        playerLog += " disconnected./";
        return;
    }

    recebido = receber;
    receber[5] = '\0';

    //Inicializa o personagem com nome, hp e defesa
    if (string_equal(receber, "START"))
    {
        param1 = recebido.substr(recebido.find('\"') + 1, recebido.rfind('\"') - (recebido.find('\"') + 1));
        recebido = recebido.substr(recebido.rfind('\"') + 2, recebido.size());
        param2 = recebido.substr(0, recebido.find(' '));
        recebido = recebido.substr(recebido.find(' '), recebido.size());
        param3 = recebido;

        if (playerSet[player_i])
            delete(playerList[player_i]);

        char* blank_names[5];
        for (int i = 0; i < 5; i++) blank_names[i] = _strdup("BLANK");
        int16_t blank_damage[5] = { 0,0,0,0,0 };

        playerList[player_i] = new Player((char*)param1.c_str(), (uint16_t)stoul(param2), (uint16_t)stoul(param3), blank_names, blank_damage);
        playerSet[player_i] = true;

        for (int i = 0; i < 5; i++) free(blank_names[i]);

        enviar = "Done!";
    }

    //Cria uma abilidade para o personagem, com nome, dano e o slot (até 5) que irá ocupar
    else if (string_equal(receber, "MAGIA"))
    {
        param1 = recebido.substr(recebido.find('\"') + 1, recebido.rfind('\"') - (recebido.find('\"') + 1));
        recebido = recebido.substr(recebido.rfind('\"') + 2, recebido.size());
        param2 = recebido.substr(0, recebido.find(' '));
        recebido = recebido.substr(recebido.find(' '), recebido.size());
        param3 = recebido;
        temp_for_malloc = _strdup(param1.c_str());

        if (playerList[player_i]->setPower(temp_for_malloc, stol(param2) - 1, (int16_t)stoul(param3)))
            enviar = "Magic set!";
        else
            enviar = "Couldn't create magic.";
        free(temp_for_malloc);
    }

    //Ataca um personagem ou efeito da mesa com uma determinada abilidade do seu personagem
    else if (string_equal(receber, "ATACA"))
    {
        //mapa
    }

    //Move o seu personagem para as coordenadas x e y determinadas
    else if (string_equal(receber, "MOVER"))
    {
        //mapa
    }

    //Envia uma mensagem de texto para um jogador da mesa
    else if (string_equal(receber, "FALAR"))
    {
        param1 = recebido.substr(recebido.find('/') + 1, recebido.rfind('/') - (recebido.find('/') + 1));

        if (playerSet[!player_i])
        {
            playerMessage[!player_i] = "MSG ";
            playerMessage[!player_i] += playerList[!player_i]->getName() + ':' + param1;
            enviar = "Message sent.";
        }
        else
            enviar = "Player not found.";
    }

    //Recebe informações de um jogador da mesa
    else if (string_equal(receber, "OLHAR"))
    {
        param1 = recebido.substr(recebido.find('\"') + 1, recebido.rfind('\"') - (recebido.find('\"') + 1));

        if (playerSet[!player_i])
            enviar = playerList[!player_i]->getInfo();
        else
            enviar = "Player not found.";
    }

    //Ativa o modo imortal ao seu personagem
    else if (string_equal(receber, "GODMO"))
    {
        if (playerSet[player_i])
        {
            playerList[player_i]->setGodMode();
            enviar = "God Mode activated.";
        }
        else
            enviar = "Not happening.";
    }
    else printf("Something something went wrong with this player lol.\n\n");
}


// Executar programa: Ctrl + F5 ou Menu Depurar > Iniciar Sem Depuração
// Depurar programa: F5 ou menu Depurar > Iniciar Depuração

// Dicas para Começar: 
//   1. Use a janela do Gerenciador de Soluções para adicionar/gerenciar arquivos
//   2. Use a janela do Team Explorer para conectar-se ao controle do código-fonte
//   3. Use a janela de Saída para ver mensagens de saída do build e outras mensagens
//   4. Use a janela Lista de Erros para exibir erros
//   5. Ir Para o Projeto > Adicionar Novo Item para criar novos arquivos de código, ou Projeto > Adicionar Item Existente para adicionar arquivos de código existentes ao projeto
//   6. No futuro, para abrir este projeto novamente, vá para Arquivo > Abrir > Projeto e selecione o arquivo. sln
