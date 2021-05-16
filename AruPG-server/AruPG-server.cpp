// AruPG-server.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#include "lib.h"
#pragma comment(lib,"Ws2_32.lib")

std::atomic<Player> playerList[10];
std::atomic<bool> setPlayer[10] = { false };
std::atomic<std::string> messagePlayer[10];
std::atomic<bool> flag1, flag2, flag3;
std::atomic<uint8_t> playerQnt;
std::atomic<std::string> log;
std::atomic<char> response[128];

int main(int argc, char* argv[])
{
    // Application params
    playerQnt = 0;
    
    /*
    flag1 = false;
    flag2 = false;
    flag3 = false;
    */

    std::string message;

    std::thread player1, player2;

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
    PORTS.push_back(5656);
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

        message = std::to_string(PORTS[count]);
        
        send(ConnectionWithClient, message.c_str(), message.size(), 0);
    }

    closesocket(ServerMain);



    
    WSACleanup();
    
    return 0;
}

void player(uint16_t PORT)
{
    //For Application
    int player_i = PORT == 9000 ? 0 : 1;
    std::string tmp;
    char* blank_names[5];
    for (int i = 0; i < 5; i++) { blank_names[i] = (char*)malloc(1); blank_names[i] = strdup("BLANK"); }
    int16_t blank_damage[5] = { 0,0,0,0,0 };

    Player *jogador, *temp;// = new Player((char*)"Abc", (uint16_t)65, (uint16_t)2, blank_names, blank_damage);

    //playerList[player_i].store(jogador);

    //For Server
    SOCKET server, connection;
    sockaddr_in socketInfo, socketClient;
    int clientSize = sizeof(sockaddr_in);

    server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    socketInfo.sin_addr.s_addr = INADDR_ANY;
    socketInfo.sin_family = AF_INET;
    socketInfo.sin_port = htons(PORT);

    if (bind(server, (sockaddr*)&socketInfo, sizeof(socketInfo)) == SOCKET_ERROR)
    {
        std::cout << "Bind failed with error code : " << WSAGetLastError();
        return;
    }
    listen(server, SOMAXCONN);

    //For Application
    std::string enviar;
    char receber[128];
    std::string recebido, param1, param2, param3;

    //Main loop
    connection = accept(server, (sockaddr*)&socketClient, &clientSize);
    for(clock_t clk = clock(), end_clk; true; clk = clock())
    {
        //MAP
        enviar = "MAP";
        send(connection, enviar.c_str(), enviar.size(), 0);
        //LOG
        enviar = log;
        send(connection, enviar.c_str(), enviar.size(), 0);
        log.store("");
        //MSG
        enviar = messagePlayer[player_i].load();
        send(connection, enviar.c_str(), enviar.size(), 0);
        messagePlayer[player_i].store("");


        recv(connection, receber, 128, 0);

        //Apaga seu personagem e sai do jogo
        if (receber == 0 || string_equal(receber, "QUIT"))
        {
            tmp = "Player ";
            if (setPlayer[player_i].load())
            {
                temp = new Player(playerList[player_i].load());
                tmp += temp->getName();
                //temp->hit(MAXINT16);
                //playerList[player_i].store(*temp);
                delete(temp);
                playerQnt--;
                setPlayer[player_i].store(false);
            }
            else
                tmp += std::to_string(player_i);
            tmp += " disconnected.";
            log.store(tmp);
            
            break;
        }

        recebido = receber;
        receber[5] = '\0';

        //Inicializa o personagem com nome, hp e defesa
        if (string_equal(receber, "START"))
        {
            param1 = recebido.substr( recebido.find('\"') + 1, recebido.rfind('\"') - (recebido.find('\"') + 1) );
            recebido = recebido.substr( recebido.rfind('\"') + 2, recebido.size() );
            param2 = recebido.substr( 0, recebido.find(' ') );
            recebido = recebido.substr( recebido.find(' '), recebido.size() );
            param3 = recebido;

            jogador = new Player((char*)param1.c_str(), (uint16_t)stoul(param2), (uint16_t)stoul(param3), blank_names, blank_damage);
            playerList[player_i].store(*jogador);
            setPlayer[player_i].store(true);
            playerQnt++;
            delete(jogador);

            enviar = "Done!";
            send(connection, enviar.c_str(), enviar.size(), 0);
        }

        //Cria uma abilidade para o personagem, com nome, dano e o slot (até 5) que irá ocupar
        else if (string_equal(receber, "MAGIA"))
        {
            param1 = recebido.substr(recebido.find('\"') + 1, recebido.rfind('\"') - (recebido.find('\"') + 1));
            recebido = recebido.substr(recebido.rfind('\"') + 2, recebido.size());
            param2 = recebido.substr(0, recebido.find(' '));
            recebido = recebido.substr(recebido.find(' '), recebido.size());
            param3 = recebido;

            temp = new Player(playerList[player_i].load());
            if (temp->setPower(strdup(param1.c_str()), stol(param2)-1, (int16_t)stoul(param3)))
            {
                playerList[player_i].store(*temp);
                enviar = "Magic set!";
            }
            else
                enviar = "Invalid index! Slot must be between 1 and 5.";
            delete(temp);

            send(connection, enviar.c_str(), enviar.size(), 0);
        }

        //Ataca um personagem ou efeito da mesa com uma determinada abilidade do seu personagem
        else if (string_equal(receber, "ATACA"))
        {
            //mapa
            send(connection, enviar.c_str(), enviar.size(), 0);
        }

        //Move o seu personagem para as coordenadas x e y determinadas
        else if (string_equal(receber, "MOVER"))
        {
            //mapa
            send(connection, enviar.c_str(), enviar.size(), 0);
        }

        //Envia uma mensagem de texto para um jogador da mesa
        else if (string_equal(receber, "FALAR"))
        {
            param1 = recebido.substr(recebido.find('\"') + 1, recebido.rfind('\"') - (recebido.find('\"') + 1));
            recebido = recebido.substr(recebido.rfind('\"') + 2, recebido.size());
            param2 = recebido.substr(recebido.find('/') + 1, recebido.rfind('/') - (recebido.find('/') + 1));

            //Search players
            for (int i = 0; i < 10; i++)
            {
                if (setPlayer[i].load())
                {
                    temp = new Player(playerList[i].load());
                    //If found player - do stuff
                    if (string_equal(param1.c_str(), temp->getName()) && !temp->isDEAD())
                    {
                        delete(temp);
                        temp = new Player(playerList[player_i].load());
                        tmp = "MSG ";
                        tmp += temp->getName() + ':' + param2;
                        messagePlayer[i].store(tmp);
                        delete(temp);
                        enviar = "Message sent.";
                        break;
                    }
                    delete(temp);
                }

                if (i == playerQnt - 1)
                    enviar = "Player not found.";
            }
            
            send(connection, enviar.c_str(), enviar.size(), 0);
        }

        //Recebe informações de um jogador da mesa
        else if (string_equal(receber, "OLHAR"))
        {
            param1 = recebido.substr(recebido.find('\"') + 1, recebido.rfind('\"') - (recebido.find('\"') + 1));

            //Search players
            for (int i = 0; i < 10; i++)
            {
                if (setPlayer[i].load())
                {
                    temp = new Player(playerList[i].load());
                    //If found player - do stuff
                    if (string_equal(param1.c_str(), temp->getName()))
                    {
                        enviar = temp->getInfo();
                        delete(temp);
                        break;
                    }
                    delete(temp);
                }
                
                if (i == 9)
                    enviar = "Player not found.";
            }

            send(connection, enviar.c_str(), enviar.size(), 0);
        }

        //Ativa o modo imortal ao seu personagem
        else if (string_equal(receber, "GODMO"))
        {
            if (setPlayer[player_i].load())
            {
                temp = jogador;
                jogador = new Player(*temp);
                delete(temp);
                jogador->setGodMode();
                playerList[player_i].store(*jogador);
                enviar = "God Mode activated.";
            }
            else
                enviar = "Not happening.";
            send(connection, enviar.c_str(), enviar.size(), 0);
        }
        else printf("Something something went wrong with this player lol.\n\n");
        
        do end_clk = clock(); while ((float)(end_clk - clk) / CLOCKS_PER_SEC < CYCLE_SIZE);
    }

    enviar = "Disconnected.";
    send(connection, enviar.c_str(), enviar.size(), 0);
    closesocket(server);
    WSACleanup();
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
