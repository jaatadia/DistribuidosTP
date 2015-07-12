 
COMMON = ../Common

all: ServerCS.cpp ServerCE.cpp BrokerInitializer.cpp CSBroker.cpp CEBroker.cpp $(COMMON)/Logger.o $(COMMON)/Parser.o
	g++ -Wall -g $(COMMON)/Logger.o ServerCS.cpp -o ServerCS
	g++ -Wall -g $(COMMON)/Logger.o ServerCE.cpp -o ServerCE
	g++ -Wall -g $(COMMON)/Logger.o $(COMMON)/Parser.o BrokerInitializer.cpp -o BrokerInitializer
	g++ -Wall -g $(COMMON)/Logger.o CSBroker.cpp -o CSBroker
	g++ -Wall -g $(COMMON)/Logger.o CEBroker.cpp -o CEBroker
	g++ -Wall -g $(COMMON)/Logger.o BrokerSHM.cpp -o BrokerSHM

$(COMMON)/Logger.o:
	cd $(COMMON);make 
	
clean:
	- rm *.o
	- rm ServerCS
	- rm ServerCE
	- rm BrokerInitializer
	- rm CSBroker
	- rm CEBroker
