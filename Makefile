#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#---------------------------- PARAMETROS-----------------------
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#parametros generales
PARAMETROS = g++ -Wall -g
PROYECTO = Simulador
PROYECT = .
RELEASE = $(PROYECT)/Release
OBJECTS = $(PROYECT)/Objects
PROCESOS = Initializer Puerta PuertaSalida Destroyer Cerrar Abrir Waker Persona Locker Encolador
SRC = ./Simulador

OBJETOS = semaforo.o Logger.o Parser.o InterfazPuerta.o InterfazPersonaEntrada.o InterfazPersonaSalida.o InterfazMuseoEntrada.o InterfazMuseoSalida.o InterfazMuseoAdministrador.o InterfazPuertaLocker.o InterfazLocker.o

#parametros para la compilacion del proceso initializer
OBJETOS_INIT = semaforo.o Logger.o Parser.o
INIT_SOURCE = Initializer.cpp
INIT_EXE = Initializer

#parametros para la compilacion del proceso persona
OBJETOS_PERSONA = Logger.o InterfazPuerta.o semaforo.o
PERSONA_SOURCE = Persona.cpp
PERSONA_EXE = Persona

#parametros para la compilacion del proceso Destroyer
OBJETOS_DEST = semaforo.o Logger.o Parser.o
DEST_SOURCE = Destroyer.cpp
DEST_EXE = Destroyer

#parametros para la compilacion del proceso puerta
OBJETOS_PUERTA = semaforo.o Logger.o InterfazPersonaEntrada.o InterfazMuseoEntrada.o InterfazLocker.o
PUERTA_SOURCE = Puerta.cpp
PUERTA_EXE = Puerta

#parametros para la compilacion del proceso puertaSalida
OBJETOS_PUERTA_SALIDA = semaforo.o Logger.o InterfazMuseoSalida.o InterfazPersonaSalida.o InterfazLocker.o
PUERTA_SALIDA_SOURCE = PuertaSalida.cpp
PUERTA_SALIDA_EXE = PuertaSalida


#parametros para la compilacion del proceso Encolador
OBJETOS_ENCOLADOR = semaforo.o Logger.o
ENCOLADOR_SOURCE = EncoladorPuertaSalida.cpp
ENCOLADOR_EXE = Encolador

#parametros para la compilacion del proceso Waker
OBJETOS_WAKER = Logger.o
WAKER_SOURCE = Waker.cpp
WAKER_EXE = Waker

#parametros para la compilacion del proceso abrir
OBJETOS_ABRIR = semaforo.o Logger.o InterfazMuseoAdministrador.o
ABRIR_SOURCE = Abrir.cpp
ABRIR_EXE = Abrir

#parametros para la compilacion del proceso cerrar
OBJETOS_CERRAR = semaforo.o Logger.o InterfazMuseoAdministrador.o
CERRAR_SOURCE = Cerrar.cpp
CERRAR_EXE = Cerrar

#parametros para la compilacion del proceso locker
OBJETOS_LOCKER = InterfazPuertaLocker.o Logger.o
LOCKER_SOURCE = Locker.cpp
LOCKER_EXE = Locker

all: $(PROYECTO)

#////////////////////////Objetos////////////////////
Objetos:
	@echo
	#-----------compilando-----------
	for i in $(OBJETOS); do $(PARAMETROS) $(SRC)/$${i%.*}.cpp -c -o $(OBJECTS)/$$i;done 

#////////////////////////INITIALIZER////////////////////
Initializer: carpetas Objetos
	@echo
	#-----------compilando-----------
	var="";for i in $(OBJETOS_INIT); do var="$$var $(OBJECTS)/$$i";done;$(PARAMETROS) $$var $(SRC)/$(INIT_SOURCE) -o $(RELEASE)/$(INIT_EXE)


#////////////////////////Destroyer////////////////////
Destroyer: carpetas Objetos
	@echo
	#-----------compilando-----------
	var="";for i in $(OBJETOS_DEST); do var="$$var $(OBJECTS)/$$i";done;$(PARAMETROS) $$var $(SRC)/$(DEST_SOURCE) -o $(RELEASE)/$(DEST_EXE)




#////////////////////////Puerta////////////////////
Puerta: carpetas Objetos
	@echo
	#-----------compilando-----------
	var="";for i in $(OBJETOS_PUERTA); do var="$$var $(OBJECTS)/$$i";done;$(PARAMETROS) $$var $(SRC)/$(PUERTA_SOURCE) -o $(RELEASE)/$(PUERTA_EXE)

#////////////////////////Persona////////////////////
Persona: carpetas Objetos
	@echo
	#-----------compilando-----------
	var="";for i in $(OBJETOS_PERSONA); do var="$$var $(OBJECTS)/$$i";done;$(PARAMETROS) $$var $(SRC)/$(PERSONA_SOURCE) -o $(RELEASE)/$(PERSONA_EXE)


#////////////////////////PuertaSalida////////////////////
PuertaSalida: carpetas Objetos
	@echo
	#-----------compilando-----------
	var="";for i in $(OBJETOS_PUERTA_SALIDA); do var="$$var $(OBJECTS)/$$i";done;$(PARAMETROS) $$var $(SRC)/$(PUERTA_SALIDA_SOURCE) -o $(RELEASE)/$(PUERTA_SALIDA_EXE)


#////////////////////////Waker////////////////////
Waker: carpetas Objetos
	@echo
	#-----------compilando-----------
	var="";for i in $(OBJETOS_WAKER); do var="$$var $(OBJECTS)/$$i";done;$(PARAMETROS) $$var $(SRC)/$(WAKER_SOURCE) -o $(RELEASE)/$(WAKER_EXE)


#////////////////////////Abrir////////////////////
Abrir: carpetas Objetos
	@echo
	#-----------compilando-----------
	var="";for i in $(OBJETOS_ABRIR); do var="$$var $(OBJECTS)/$$i";done;$(PARAMETROS) $$var $(SRC)/$(ABRIR_SOURCE) -o $(RELEASE)/$(ABRIR_EXE)

#////////////////////////Cerrar////////////////////
Cerrar: carpetas Objetos
	@echo
	#-----------compilando-----------
	var="";for i in $(OBJETOS_CERRAR); do var="$$var $(OBJECTS)/$$i";done;$(PARAMETROS) $$var $(SRC)/$(CERRAR_SOURCE) -o $(RELEASE)/$(CERRAR_EXE)

#////////////////////////Locker////////////////////
Locker: carpetas Objetos
	@echo
	#-----------compilando-----------
	var="";for i in $(OBJETOS_LOCKER); do var="$$var $(OBJECTS)/$$i";done;$(PARAMETROS) $$var $(SRC)/$(LOCKER_SOURCE) -o $(RELEASE)/$(LOCKER_EXE)


#////////////////////////Encolador////////////////////
Encolador: carpetas Objetos
	@echo
	#-----------compilando-----------
	var="";for i in $(OBJETOS_ENCOLADOR); do var="$$var $(OBJECTS)/$$i";done;$(PARAMETROS) $$var $(SRC)/$(ENCOLADOR_SOURCE) -o $(RELEASE)/$(ENCOLADOR_EXE)


#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#---------------------------- Otros Comandos -------------------------------
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

carpetas:
	mkdir -p $(RELEASE)
	mkdir -p $(OBJECTS)
	cp ./IPC_RM.sh $(RELEASE)
	cp ./museo.cfg $(RELEASE)


clean:
	rm -f -r $(RELEASE)
	rm -f -r $(OBJECTS)
	./IPC_RM.sh

run:
	cd Broker; ./BrokerInitializer
	cd Puerta; ./MuseoInitializer;./PuertaInitializer 1;./PuertaInitializer 2
	cd Persona; ./PersonaInitializer

$(PROYECTO): $(PROCESOS)	
	g++ -Wall -g $(OBJECTS)/Logger.o $(SRC)/Conectador.cpp  -o $(RELEASE)/Conectador
	g++ -Wall -g $(OBJECTS)/Logger.o $(SRC)/CSCliente.cpp -o $(RELEASE)/CSCliente
	g++ -Wall -g $(OBJECTS)/Logger.o $(SRC)/CECliente.cpp -o $(RELEASE)/CECliente
