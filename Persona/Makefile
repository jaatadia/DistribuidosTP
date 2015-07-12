
COMMON = ../Common

all: InterfazPuerta.cpp $(COMMON)/Logger.o $(COMMON)/Parser.o $(COMMON)/tcpopact.o $(COMMON)/tcpopact.o $(COMMON)/Conectador.o
	g++ -Wall -g -c InterfazPuerta.cpp
	g++ -Wall -g $(COMMON)/Logger.o $(COMMON)/Parser.o $(COMMON)/tcpopact.o $(COMMON)/Conectador.o InterfazPuerta.o Persona.cpp -o Persona
	g++ -Wall -g $(COMMON)/Logger.o PersonaInitializer.cpp -o PersonaInitializer

$(COMMON)/Logger.o:
	cd $(COMMON);make 
	
$(COMMON)/Parser.o:
	cd $(COMMON);make 

$(COMMON)/tcpopact.o:
	cd $(COMMON);make 
	
$(COMMON)/Conectador.o:
	cd $(COMMON);make 

clean:
	- rm *.o
	- rm Persona
	- rm PersonaInitializer
	