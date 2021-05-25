// AruPG-client.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#include "lib.h"
/*
	Create a TCP socket
*/

#pragma comment(lib,"ws2_32.lib") //Winsock Library

std::string message = "", resposta;
std::string playerInput, playerInput_pass;
std::string Map = "", Log = "", Msg = "", Look = "";
//std::string printMap, printLog, printMsg, printDefault;
char Player_Port[200], receive[2000];
bool Ready = true;

//std::atomic<bool> Ready = true, readyInput = false, readyDefault = false;
//std::mutex mInput, mMap, mLog, mMsg, mDefault;
//std::condition_variable cvInput;

bool isNumber(const std::string& str)
{
	for (char const& c : str) {
		if (std::isdigit(c) == 0) return false;
	}
	return true;
}

bool isNegative(const std::string& str)
{
	return str[0] == '-' && isNumber(str.substr(1, str.size())) ? true : false;
}

int main(int argc, char* argv[])
{
	// For Application
	char choice;
	std::string param = "";

	int temp_count = 0;

	// For Client Instance
	SOCKET Client_GetPort, MainClient;
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
	serverInfo.sin_addr.s_addr = inet_addr("192.168.100.34");
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

	if ((MainClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}

	serverInfo.sin_port = htons(Main_PORT);

	//std::thread PlayerConnection(PlayerBehavior, Main_PORT);

	while (connect(MainClient, (struct sockaddr*)&serverInfo, sizeof(serverInfo)) < 0)
	{
		temp_count++;
		if (temp_count == 10000)
		{
			puts("connect error");
			return 1;
		}
	}

	puts("Connected");

	while (Ready)
	{
		std::cout << "Wait...\n";
		recv_size = recv(MainClient, receive, 2000, 0);
		if (recv_size == 0)
		{
			std::cout << "Server disconnected.";
			break;
		}
		else
			receive[recv_size] = '\0';
		resposta = receive;
		//receive[3] = '\0';
		if (resposta.find("LOG") != std::string::npos)
		{
			Log = resposta.substr(resposta.find('/'), resposta.rfind('/'));
		}
		resposta = resposta.substr(resposta.find(';') + 1, resposta.size());

		if (resposta.find("MAP") != std::string::npos)
		{
			Map = resposta.substr(resposta.find('/'), resposta.find(';') - resposta.find('/') - 2);
		}
		resposta = resposta.substr(resposta.find(';') + 1, resposta.size());

		if (resposta.find("MSG") != std::string::npos)
		{
			Msg = resposta.substr(resposta.find('/'), resposta.rfind('/'));
		}
		/*
		resposta = resposta.substr(resposta.find(';') + 1, resposta.size());

		if (resposta.find("LOO") != std::string::npos)
		{
			Look = resposta_aux.substr(resposta.find('/'), resposta.rfind('/'));
		}
		resposta = resposta.substr(resposta.find(';') + 1, resposta.size());
		*/
		/*
		if (string_equal(receive, "MAP"))
		{
			Map = resposta.substr(resposta.find('/'), resposta.rfind('/')).c_str();
		}
		else if (string_equal(receive, "MSG"))
		{
			Msg = resposta.substr(resposta.find('/'), resposta.rfind('/')).c_str();
		}
		else if (string_equal(receive, "LOG"))
		{
			Log = resposta.substr(resposta.find('/'), resposta.rfind('/')).c_str();
		}
		else if (string_equal(receive, "LOO"))
		{
			Look = resposta.substr(resposta.find('/'), resposta.rfind('/')).c_str();
		}
		else// if (string_equal(receive, "NON"))
		{
			//does nothing
		}
		*/
		std::cout << "0: Iniciar\n1: Criar magia\n2: Atacar alvo\n3: Mover personagem\n4: Falar com jogador\n";
		std::cout << "5: Olhar outro jogador\n6: Ativar God Mode\n7: Sair do jogo\n";
		std::cout << "8: Mostrar os personagens\n9: Mostrar mensagem recebida\na: Mostrar o log\nDigite : ";
		std::cin >> choice;
		switch (choice)
		{
		case '0':	//Iniciar o jogo, criar personagem
			playerInput = "START \"<nome>\" <hp> <defesa>";
			do
			{
				std::cout << "Digite o nome: ";
				std::getline(std::cin, param);
				std::cin.ignore();
			} while (param.size() > 50);
			playerInput.replace(playerInput.find("<nome>"), 6, param);

			do
			{
				std::cout << "Digite o HP: ";
				std::getline(std::cin, param);
			} while (!isNumber(param) || isNegative(param) || std::stoul(param) >= UINT16_MAX);
			playerInput.replace(playerInput.find("<hp>"), 4, param);

			do
			{
				std::cout << "Digite a defesa: ";
				std::getline(std::cin, param);
			} while (!isNumber(param) || isNegative(param) || std::stoul(param) >= UINT16_MAX);
			playerInput.replace(playerInput.find("<defesa>"), 8, param);

			break;

		case '1':	//Criar uma magia para o personagem
			playerInput = "MAGIA \"<nome>\" <posicao> <dano> <tipo>";

			do
			{
				std::cout << "Digite o nome: ";
				std::getline(std::cin, param);
			} while (param.size() > 50);
			playerInput.replace(playerInput.find("<nome>"), 6, param);

			do
			{
				std::cout << "Digite a posição: ";
				std::getline(std::cin, param);
			} while (!isNumber(param) || std::stoi(param) == 0 || std::stoi(param) > 5);
			playerInput.replace(playerInput.find("<posicao>"), 9, param);

			do
			{
				std::cout << "Digite o dano: ";
				std::getline(std::cin, param);
			} while ((!isNumber(param) && !isNegative(param)) || std::stoi(param) >= INT16_MAX || std::stoi(param) <= INT16_MIN);
			playerInput.replace(playerInput.find("<dano>"), 6, param);

			do
			{
				std::cout << "Digite o tipo: ";
				std::getline(std::cin, param);
			} while (!isNumber(param) || std::stoi(param) < 0 || std::stoul(param) > 1);
			playerInput.replace(playerInput.find("<tipo>"), 6, param);

			break;

		case '2':	//Atacar algum alvo da mesa
			playerInput = "ATACA \"<nome_do_alvo>\" <ataque> <coordenada_x> <coordenada_y>";

			do
			{
				std::cout << "Digite o nome: ";
				std::getline(std::cin, param);
			} while (param.size() > 50);
			playerInput.replace(playerInput.find("<nome_do_alvo>"), 14, param);

			do
			{
				std::cout << "Digite o índice do ataque: ";
				std::getline(std::cin, param);
			} while (!isNumber(param) || std::stoi(param) == 0 || std::stoi(param) > 5);
			playerInput.replace(playerInput.find("<ataque>"), 8, param);

			std::cout << "Digite as coordenadas da posição do alvo\n";
			do
			{
				std::cout << "x: ";
				std::getline(std::cin, param);
			} while (!isNumber(param) || std::stoi(param) >= 100);
			playerInput.replace(playerInput.find("<coordenada_x>"), 14, param);
			do
			{
				std::cout << "y: ";
				std::getline(std::cin, param);
			} while (!isNumber(param) || std::stoi(param) >= 100);
			playerInput.replace(playerInput.find("<coordenada_y>"), 14, param);

			break;

		case '3':	//Mover o personagem para uma posição
			playerInput = "MOVER <coordenada_x> <coordenada_y>";

			std::cout << "Digite as coordenadas da posição do alvo\n";
			do
			{
				std::cout << "x: ";
				std::getline(std::cin, param);
			} while (!isNumber(param) || std::stoi(param) >= 100);
			playerInput.replace(playerInput.find("<coordenada_x>"), 14, param);
			do
			{
				std::cout << "y: ";
				std::getline(std::cin, param);
			} while (!isNumber(param) || std::stoi(param) >= 100);
			playerInput.replace(playerInput.find("<coordenada_y>"), 14, param);

			break;

		case '4':	//Falar com outro jogador

			playerInput = "FALAR /<mensagem>/";

			do
			{
				std::cout << "Digite sua mensagem: ";
				std::getline(std::cin, param);
			} while (param.size() > 500);
			playerInput.replace(playerInput.find("<mensagem>"), 10, param);

			break;

		case '5':	//Verificar os status de outro personagem
			playerInput = "OLHAR";

			break;

		case '6':	//Ativar God Mode
			playerInput = "GODMO";

			break;

		case '7':	//Sair do jogo
			playerInput = "QUIT";

			Ready = false;
			break;

		case '8':	//Imprimir informações dos personagens
			std::cout << Map << '\n';
			break;

		case '9':	//Imprimir uma mensagem recebida
			std::cout << Msg << '\n';
			break;

		case 'a':	//Imprimir o Log
			std::cout << Log << '\n';
			break;

		default:
			continue;
			break;
		}

		send(MainClient, playerInput.c_str(), playerInput.size(), 0);
		if (playerInput == "QUIT")
			break;
		playerInput = "";

		std::this_thread::sleep_for(std::chrono::seconds(4));

		/*
		if (readyDefault.load())
		{
			mDefault.lock();
			printDefault = Default.c_str();
			mDefault.unlock();
			readyDefault.store(false);

			std::cout << printDefault << '\n';
		}
		*/
	}

	//PlayerConnection.join();
	closesocket(MainClient);
	WSACleanup();
	return 0;
}