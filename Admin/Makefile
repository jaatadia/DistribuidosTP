COMMON=../Common
ID=../Broker/idServerRPC/

all: Abrir.cpp Cerrar.cpp InterfazMuseoAdministrador.cpp $(COMMON)/Logger.o $(COMMON)/semaforo.o $(COMMON)/Parser.o $(COMMON)/Conectador.o $(COMMON)/tcpopact.o $(ID)/idServer_client.o $(ID)/idServer_clnt.o $(ID)/idServer_xdr.o
	g++ -Wall -g -c InterfazMuseoAdministrador.cpp
	g++ -Wall -g $(COMMON)/Logger.o $(COMMON)/semaforo.o $(COMMON)/Parser.o $(COMMON)/Conectador.o $(COMMON)/tcpopact.o $(ID)/idServer_client.o $(ID)/idServer_clnt.o $(ID)/idServer_xdr.o InterfazMuseoAdministrador.o Abrir.cpp -o Abrir
	g++ -Wall -g $(COMMON)/Logger.o $(COMMON)/semaforo.o $(COMMON)/Parser.o $(COMMON)/Conectador.o $(COMMON)/tcpopact.o $(ID)/idServer_client.o $(ID)/idServer_clnt.o $(ID)/idServer_xdr.o InterfazMuseoAdministrador.o Cerrar.cpp -o Cerrar
	g++ -Wall -g $(COMMON)/Logger.o AdminInitializer.cpp -o AdminInitializer

$(COMMON)/Logger.o:
	cd $(COMMON);make

$(COMMON)/semaforo.o:
	cd $(COMMON);make
	
$(COMMON)/Parser.o:
	cd $(COMMON);make

$(COMMON)/Conectador.o:
	cd $(COMMON);make
	
$(COMMON)/tcpopact.o:
	cd $(COMMON);make

$(ID)/idServer_client.o:
	cd $(ID);make

$(ID)/idServer_clnt.o:
	cd $(ID);make
	
$(ID)/idServer_xdr.o:
	cd $(ID);make

clean:
	- rm *.o
	- rm Abrir
	- rm Cerrar
	- rm AdminInitializer
