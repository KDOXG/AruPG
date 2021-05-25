# AruPG
##### RPG de mesa de dois jogadores.

[Acessar relatório (necessita @inf)](https://docs.google.com/document/d/1UpyvMn2EG-AOG-cTgMatSTgbBfmJMrUf_M7QdMUVNrA/edit?usp=sharing)

# Como usar

1. Compilar o projeto com Visual C++
- Versão do SDK do Windows: 10.0
- Plataforma: Visual Studio 2019 (v142)
- Padrão: ISO C++ 14
- Executável compilado disponível em [Releases](https://github.com/KDOXG/AruPG/releases/tag/1.0)
2. Executar o servidor. Ele estará a espera da conexão de cada cliente.
3. Executar os clientes. Com isso, a tarefa entrará em execução.
- Cada cliente poderá escolher uma ação independentemente. Porém, a ação só será processada pelo servidor após os dois clientes enviarem suas respostas.
- Acessar o relatório para verificar os possíveis comandos.
4. O servidor encerrará caso os dois jogadores escolham a opção de encerrar a partida ou caso os dois clientes sejam desconectados.
