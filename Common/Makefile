all: semaforo.cpp Logger.cpp Parser.cpp Conectador.cpp CSCliente.cpp CECliente.cpp
	g++ -Wall -g -c semaforo.cpp
	g++ -Wall -g -c Logger.cpp
	g++ -Wall -g -c Parser.cpp
	g++ -Wall -g -c tcpopact.cpp
	g++ -Wall -g -c Conectador.cpp
	g++ -Wall -g Logger.o CSCliente.cpp -o CSCliente
	g++ -Wall -g Logger.o CECliente.cpp -o CECliente
	g++ -Wall -g Logger.o CSClienteMuseo.cpp -o CSClienteMuseo
	g++ -Wall -g Logger.o CEClienteMuseo.cpp -o CEClienteMuseo
clean:
	- rm *.o
	- rm CSCliente
	- rm CECliente
	- rm CSClienteMuseo
	- rm CEClienteMuseo
