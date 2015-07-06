all:
	cd Broker; make
	cd Puerta; make
	cd Persona; make
	cd Common; make
	cd Admin; make

cleanLog:
	- cd Broker; rm log.txt
	- cd Puerta; rm log.txt
	- cd Persona; rm log.txt
	- cd Admin; rm log.txt

clean:
	cd Broker; make clean
	cd Puerta; make clean
	cd Persona; make clean
	cd Common; make clean
	cd Admin; make clean
	./IPC_RM.sh
	./PS_RM.sh

run:
	cd Broker; ./BrokerInitializer
	cd Admin; ./MuseoInitializer;
	cd Puerta; ./PuertaInitializer 1;./PuertaInitializer 2
	cd Persona; ./PersonaInitializer 1

stop:
	./IPC_RM.sh
	./PS_RM.sh


