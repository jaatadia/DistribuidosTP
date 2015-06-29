 

all: Broker.cpp Conectador.cpp BrokerInitializer.cpp CSBroker.cpp CEBroker.cpp CSCliente.cpp CECliente.cpp NameServerBroker.cpp NameServer.cpp
	g++ -c Logger.cpp
	g++ -Wall -g Logger.o Broker.cpp -o Broker
	g++ -Wall -g Logger.o BrokerInitializer.cpp -o BrokerInitializer
	g++ -Wall -g Logger.o CSBroker.cpp -o CSBroker
	g++ -Wall -g Logger.o CEBroker.cpp -o CEBroker
	g++ -Wall -g Logger.o NameServerBroker.cpp -o NameServerBroker
	g++ -Wall -g Logger.o NameServer.cpp -o NameServer
	
	
clean:
	- rm *.o
	- rm Broker
	- rm BrokerInitializer
	- rm CSBroker
	- rm CEBroker
	- rm NameServerBroker
	- rm NameServer
