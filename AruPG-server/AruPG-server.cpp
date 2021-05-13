// AruPG-server.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#include "lib.h"
#pragma comment(lib,"Ws2_32.lib")

int main(int argc, char* argv[])
{
    // Application params
    flag1 = false;
    flag2 = false;
    flag3 = false;


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

    std::string enviar;
    char receber[128];
    std::string atual;

    connection = accept(server, (sockaddr*)&socketClient, &clientSize);
    for(clock_t clk = clock(); true; clk = clock())
    {
        enviar = "MAP";
        send(connection, enviar.c_str(), enviar.size(), 0);
        enviar = "LOG";
        send(connection, enviar.c_str(), enviar.size(), 0);


        recv(connection, receber, 128, 0);

        if (receber == 0 || string_equal(receber,"QUIT"))
            break;

        atual = receber;
        receber[5] = '\0';

        if (string_equal(receber, "START"))
        {

        }
        else if (string_equal(receber, "MAGIA"))
        {

        }
        else if (string_equal(receber, "ATACA"))
        {

        }
        else if (string_equal(receber, "MOVER"))
        {

        }
        else if (string_equal(receber, "FALAR"))
        {

        }
        else if (string_equal(receber, "OLHAR"))
        {

        }
        else if (string_equal(receber, "GODMO"))
        {

        }
        else
            printf("Something something went wrong with this player lol.\n\n");

        while (((float)(clock() - clk)) / CLOCKS_PER_SEC < CYCLE_SIZE);
    }

    free(receber);

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
