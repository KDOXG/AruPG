// AruPG-server.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#include "lib.h"

int main()
{
    // Initializing WinSock
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        std::cout << "Failed. Error Code : " << WSAGetLastError();
        return 1;
    }
    SOCKET ServerMain;
    sockaddr_in socketInfo;

    std::vector<uint16_t> PORTS;
    PORTS.push_back(5656);
    
    ServerMain = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    socketInfo.sin_addr.s_addr = INADDR_ANY;
    socketInfo.sin_family = AF_INET;
    socketInfo.sin_port = htons(PORTS[0]);

    char ip[100] = "192.168.100.34";
    //socketInfo.sin_addr.s_addr = inet_addr(ip);

    if (bind(ServerMain, (sockaddr*)&socketInfo, sizeof(socketInfo)) == SOCKET_ERROR)
    {
        std::cout << "Bind failed with error code : " << WSAGetLastError();
    }



    std::cout << "Hello World!\n";
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
