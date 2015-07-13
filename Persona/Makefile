
COMMON = ../Common
ID=../Broker/idServerRPC

all: InterfazPuerta.cpp $(COMMON)/Logger.o $(COMMON)/Parser.o $(COMMON)/tcpopact.o $(COMMON)/tcpopact.o $(COMMON)/Conectador.o $(ID)/idServer_client.o $(ID)/idServer_clnt.o $(ID)/idServer_xdr.o
	g++ -Wall -g -c InterfazPuerta.cpp
	g++ -Wall -g $(COMMON)/Logger.o $(COMMON)/Parser.o $(COMMON)/tcpopact.o $(COMMON)/Conectador.o $(ID)/idServer_client.o $(ID)/idServer_clnt.o $(ID)/idServer_xdr.o InterfazPuerta.o Persona.cpp -o Persona
	g++ -Wall -g $(COMMON)/Logger.o PersonaInitializer.cpp -o PersonaInitializer

$(COMMON)/Logger.o:
	cd $(COMMON);make 
	
$(COMMON)/Parser.o:
	cd $(COMMON);make 

$(COMMON)/tcpopact.o:
	cd $(COMMON);make 
	
$(COMMON)/Conectador.o:
	cd $(COMMON);make 

$(ID)/idServer_client.o:
	cd $(ID);make
	
$(ID)/idServer_clnt.o: 
	cd $(ID);make
	
$(ID)/idServer_xdr.o:
	cd $(ID);make

clean:
	- rm *.o
	- rm Persona
	- rm PersonaInitializer
	