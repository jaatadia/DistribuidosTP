
COMMON = ../Common
ID=../Broker/idServerRPC

all: Puerta.cpp $(COMMON)/Logger.o $(COMMON)/semaforo.o $(COMMON)/Parser.o $(COMMON)/tcpopact.o $(COMMON)/Conectador.o InterfazMuseoEntrada.cpp InterfazPersonaEntrada.cpp InterfazMuseoSalida.cpp InterfazPersonaSalida.cpp InterfazLocker.cpp PuertaSalida.cpp Puerta.cpp EncoladorPuertaSalida.cpp Locker.cpp InterfazPuertaLocker.cpp PuertaInitializer.cpp $(ID)/idServer_client.o $(ID)/idServer_clnt.o $(ID)/idServer_xdr.o

	g++ -Wall -g $(COMMON)/Logger.o $(COMMON)/semaforo.o PuertaInitializer.cpp -o PuertaInitializer
	g++ -Wall -g -c InterfazMuseoEntrada.cpp 
	g++ -Wall -g -c InterfazPersonaEntrada.cpp 
	g++ -Wall -g -c InterfazMuseoSalida.cpp 
	g++ -Wall -g -c InterfazPersonaSalida.cpp 
	g++ -Wall -g -c InterfazLocker.cpp
	g++ -Wall -g -c InterfazPuertaLocker.cpp
	g++ -Wall -g $(COMMON)/Logger.o $(COMMON)/semaforo.o $(COMMON)/Parser.o InterfazMuseoEntrada.o InterfazPersonaEntrada.o InterfazLocker.o $(COMMON)/tcpopact.o $(COMMON)/Conectador.o $(ID)/idServer_client.o $(ID)/idServer_clnt.o $(ID)/idServer_xdr.o Puerta.cpp -o Puerta
	g++ -Wall -g $(COMMON)/Logger.o $(COMMON)/semaforo.o $(COMMON)/Parser.o InterfazMuseoSalida.o InterfazPersonaSalida.o InterfazLocker.o $(COMMON)/tcpopact.o $(COMMON)/Conectador.o $(ID)/idServer_client.o $(ID)/idServer_clnt.o $(ID)/idServer_xdr.o PuertaSalida.cpp -o PuertaSalida
	g++ -Wall -g $(COMMON)/Logger.o $(COMMON)/semaforo.o EncoladorPuertaSalida.cpp -o Encolador
	g++ -Wall -g $(COMMON)/Logger.o InterfazPuertaLocker.o Locker.cpp -o Locker
	

$(COMMON)/Logger.o:
	cd $(COMMON);make 

$(COMMON)/Parser.o:
	cd $(COMMON);make 
	
$(COMMON)/semforo.o:
	cd $(COMMON);make 

$(COMMON)/tcpopact.o:
	cd $(COMMON);make 
	
$(COMMON)/Conectador.o:
	cd $(COMMON);make 

$(ID)/idServer_client.o:
	cd $(ID)/make;
	
$(ID)/idServer_clnt.o:
	cd $(ID)/make;
	
$(ID)/idServer_xdr.o:
	cd $(ID)/make;


clean:
	- rm *.o
	- rm Puerta
	- rm PuertaSalida
	- rm Encolador
	- rm Locker
	- rm PuertaInitializer
