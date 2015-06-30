
COMMON = ../Common

all: InterfazPuerta.cpp $(COMMON)/Logger.o
	g++ -Wall -g -c InterfazPuerta.cpp
	g++ -Wall -g $(COMMON)/Logger.o InterfazPuerta.o Persona.cpp -o Persona
	g++ -Wall -g $(COMMON)/Logger.o PersonaInitializer.cpp -o PersonaInitializer

$(COMMON)/Logger.o:
	cd $(COMMON);make 
	
$(COMMON)/semforo.o:
	cd $(COMMON);make 

clean:
	- rm *.o
	- rm Persona
	- rm PersonaInitializer
	