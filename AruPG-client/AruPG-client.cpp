// AruPG-client.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#include "C:\Users\KDOXG\source\repos\AruPG-server\AruPG-server\lib.h"
/*
	Create a TCP socket
*/

#pragma comment(lib,"ws2_32.lib") //Winsock Library

std::string message = "", resposta;
std::string playerInput, playerInput_pass;
std::string Map, Log, Msg, Default;
std::string printMap, printLog, printMsg, printDefault;
char Player_Port[200], receive[2000];

bool Ready = true;

std::atomic<bool> readyInput = false, readyDefault = false;
std::mutex mInput, mMap, mLog, mMsg, mDefault;
std::condition_variable cvInput;

bool isNumber(const std::string& str)
{
	for (char const& c : str) {
		if (std::isdigit(c) == 0) return false;
	}
	return true;
}

bool isNegative(const std::string& str)
{
	return str[0] == '-' && isNumber(str.substr(1,str.size())) ? true : false;
}

int main(int argc, char* argv[])
{
	// For Application
	char choice;
	std::string param;

	// For Client Instance
	SOCKET Client_GetPort;
	sockaddr_in serverInfo;
	int recv_size;
	uint16_t Main_PORT;
	WSADATA wsa;

	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return 1;
	}

	printf("Initialised.\n");

	//Create a socket
	if ((Client_GetPort = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}

	printf("Socket created.\n");

	//Initializing the server params for connection
	serverInfo.sin_addr.s_addr = INADDR_ANY;
	serverInfo.sin_family = AF_INET;
	serverInfo.sin_port = htons(10000);

	//Connect to remote server
	if (connect(Client_GetPort, (struct sockaddr*)&serverInfo, sizeof(serverInfo)) < 0)
	{
		puts("connect error");
		return 1;
	}

	puts("Connected");

	//Send some data
	/*
	message = _strdup("GET / HTTP/1.1\r\n\r\n");
	if (send(s, message, strlen(message), 0) < 0)
	{
		puts("Send failed");
		return 1;
	}
	puts("Data Send\n");
	*/

	//Receive a reply from the server
	if ((recv_size = recv(Client_GetPort, Player_Port, 200, 0)) == SOCKET_ERROR)
	{
		puts("recv failed");
	}
	puts("Reply received\n");

	//Add a NULL terminating character to make it a proper string before printing
	Player_Port[recv_size] = '\0';
	//puts(Port);

	Main_PORT = (uint16_t)std::stoul(std::string(Player_Port));

	closesocket(Client_GetPort);

	std::thread PlayerConnection(PlayerBehavior,Main_PORT);

	while (Ready)
	{
		std::cout << "Digite: ";
		choice = getchar();
		switch (choice)
		{
		case '0':	//Iniciar o jogo, criar personagem
			playerInput = "START \"<nome>\" <hp> <defesa>";
			do
			{
				std::cout << "Digite o nome: ";
				std::cin >> param;
			} while (param.size() > 50);
			playerInput.replace(playerInput.find("<nome>"), 6, param);

			do
			{
				std::cout << "Digite o HP: ";
				std::cin >> param;
			} while (!isNumber(param) || isNegative(param) || std::stoul(param) >= UINT16_MAX);
			playerInput.replace(playerInput.find("<hp>"), 4, param);

			do
			{
				std::cout << "Digite a defesa: ";
				std::cin >> param;
			} while (!isNumber(param) || isNegative(param) || std::stoul(param) >= UINT16_MAX);
			playerInput.replace(playerInput.find("<defesa>"), 8, param);

			mInput.lock();
			playerInput_pass = playerInput;
			mInput.unlock();
			readyInput.store(true);
		break;

		case '1':	//Criar uma magia para o personagem
			playerInput = "MAGIA \"<nome>\" <posicao> <dano> <tipo>";

			do
			{
				std::cout << "Digite o nome: ";
				std::cin >> param;
			} while (param.size() > 50);
			playerInput.replace(playerInput.find("<nome>"), 6, param);

			do
			{
				std::cout << "Digite a posição: ";
				std::cin >> param;
			} while (!isNumber(param) || std::stoi(param) == 0 || std::stoi(param) > 5);
			playerInput.replace(playerInput.find("<posicao>"), 9, param);

			do
			{
				std::cout << "Digite o dano: ";
				std::cin >> param;
			} while ((!isNumber(param) && !isNegative(param)) || std::stoi(param) >= INT16_MAX || std::stoi(param) <= INT16_MIN);
			playerInput.replace(playerInput.find("<dano>"), 6, param);

			do
			{
				std::cout << "Digite o tipo: ";
				std::cin >> param;
			} while (!isNumber(param) || std::stoi(param) < 0 || std::stoul(param) > 1);
			playerInput.replace(playerInput.find("<tipo>"), 6, param);

			mInput.lock();
			playerInput_pass = playerInput;
			mInput.unlock();
			readyInput.store(true);
		break;

		case '2':	//Atacar algum alvo da mesa
			playerInput = "ATACA \"<nome_do_alvo>\" <ataque> <coordenada_x> <coordenada_y>";

			do
			{
				std::cout << "Digite o nome: ";
				std::cin >> param;
			} while (param.size() > 50);
			playerInput.replace(playerInput.find("<nome_do_alvo>"), 14, param);

			do
			{
				std::cout << "Digite o índice do ataque: ";
				std::cin >> param;
			} while (!isNumber(param) || std::stoi(param) == 0 || std::stoi(param) > 5);
			playerInput.replace(playerInput.find("<ataque>"), 8, param);

			std::cout << "Digite as coordenadas da posição do alvo\n";
			do
			{
				std::cout << "x: ";
				std::cin >> param;
			} while (!isNumber(param) || std::stoi(param) >= 100);
			playerInput.replace(playerInput.find("<coordenada_x>"), 14, param);
			do
			{
				std::cout << "y: ";
				std::cin >> param;
			} while (!isNumber(param) || std::stoi(param) >= 100);
			playerInput.replace(playerInput.find("<coordenada_y>"), 14, param);

			mInput.lock();
			playerInput_pass = playerInput;
			mInput.unlock();
			readyInput.store(true);
		break;

		case '3':	//Mover o personagem para uma posição
			playerInput = "MOVER <coordenada_x> <coordenada_y>";

			std::cout << "Digite as coordenadas da posição do alvo\n";
			do
			{
				std::cout << "x: ";
				std::cin >> param;
			} while (!isNumber(param) || std::stoi(param) >= 100);
			playerInput.replace(playerInput.find("<coordenada_x>"), 14, param);
			do
			{
				std::cout << "y: ";
				std::cin >> param;
			} while (!isNumber(param) || std::stoi(param) >= 100);
			playerInput.replace(playerInput.find("<coordenada_y>"), 14, param);

			mInput.lock();
			playerInput_pass = playerInput;
			mInput.unlock();
			readyInput.store(true);
		break;

		case '4':	//Falar com outro jogador

			playerInput = "FALAR /<mensagem>/";

			do
			{
				std::cout << "Digite sua mensagem: ";
				std::cin >> param;
			} while (param.size() > 500);
			playerInput.replace(playerInput.find("<mensagem>"), 10, param);

			mInput.lock();
			playerInput_pass = playerInput;
			mInput.unlock();
			readyInput.store(true);
		break;

		case '5':	//Verificar os status de outro personagem
			playerInput = "OLHAR";

			mInput.lock();
			playerInput_pass = playerInput;
			mInput.unlock();
			readyInput.store(true);
		break;

		case '6':	//Ativar God Mode
			playerInput = "GODMO";

			mInput.lock();
			playerInput_pass = playerInput;
			mInput.unlock();
			readyInput.store(true);
		break;

		case '7':	//Sair do jogo
			playerInput = "QUIT";
			
			Ready = false;
		break;

		case '8':	//Imprimir o Log
			playerInput = "QUIT";

			Ready = false;
			break;

		case '9':	//Imprimir uma mensagem recebida
			playerInput = "QUIT";

			Ready = false;
			break;

		case 'a':	//Imprimir informações dos personagens
			playerInput = "QUIT";

			Ready = false;
			break;

		default:
			continue;
		break;
		}

		std::this_thread::sleep_for(std::chrono::seconds(4));


	}

	PlayerConnection.join();
	WSACleanup();
	return 0;
}

void PlayerBehavior(uint16_t PORT)
{
	SOCKET MainClient;
	sockaddr_in serverInfo;
	int recv_size;

	int LogCount = 0;

	if ((MainClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}

	//Initializing the server params for connection
	serverInfo.sin_addr.s_addr = INADDR_ANY;
	serverInfo.sin_family = AF_INET;
	serverInfo.sin_port = htons(PORT);

	//Connect to remote server
	if (connect(MainClient, (struct sockaddr*)&serverInfo, sizeof(serverInfo)) < 0)
	{
		puts("connect error");
		return;
	}

	while (true)
	{
		recv_size = recv(MainClient, receive, 2000, 0);
		resposta = receive;
		receive[3] = '\0';

		if (string_equal(receive, "MAP"))
		{
			mMap.lock();
			Map = resposta.substr(resposta.find('/'), resposta.rfind('/'));
			mMap.unlock();
		}
		else if (string_equal(receive, "MSG"))
		{
			mMsg.lock();
			Msg = resposta.substr(resposta.find('/'), resposta.rfind('/'));
			mMsg.unlock();
		}
		else if (string_equal(receive, "LOG"))
		{
			mLog.lock();
			Log = resposta.substr(resposta.find('/'), resposta.rfind('/'));
			mLog.unlock();
		}
		else if (string_equal(receive, "NON"))
		{
			//does nothing
		}
		else
		{
			mDefault.lock();
			Default = resposta;
			mDefault.unlock();
		}

		if (readyInput.load())
		{
			mInput.lock();
			message = playerInput_pass.c_str();
			mInput.unlock();
			readyInput.store(false);
		}
		send(MainClient, message.c_str(), message.size(), 0);
		message = "";
	}
	
}