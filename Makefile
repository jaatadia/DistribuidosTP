all:
	cd Broker; make
	cd Puerta; make
	cd Persona; make
	cd Common; make

clean:
	cd Broker; make clean
	cd Puerta; make clean
	cd Persona; make clean
	cd Common; make clean
	./IPC_RM.sh
	./PS_RM.sh

run:
	cd Broker; ./BrokerInitializer
	cd Puerta; ./MuseoInitializer;./PuertaInitializer 1;./PuertaInitializer 2
	cd Persona; ./PersonaInitializer

stop:
	./IPC_RM.sh
	./PS_RM.sh


