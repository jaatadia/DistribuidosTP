all: semaforo.cpp Logger.cpp Parser.cpp Conectador.cpp CSCliente.cpp CECliente.cpp
	g++ -Wall -g -c semaforo.cpp
	g++ -Wall -g -c Logger.cpp
	g++ -Wall -g -c Parser.cpp
	g++ -Wall -g Logger.o CSCliente.cpp -o CSCliente
	g++ -Wall -g Logger.o CECliente.cpp -o CECliente
clean:
	- rm *.o
	- rm CSCliente
	- rm CECliente
