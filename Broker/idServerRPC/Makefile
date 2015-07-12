all: 
	gcc -Wall -g idServer_xdr.c -c
	gcc -Wall -g idServer_server.c -c 
	gcc -Wall -g idServer_xdr.o idServer_server.o idServer_svc.c -o idServer_svc
	g++ -Wall -g idServer_client.cpp -c 
	g++ -Wall -g idServer_clnt.c -c 
	g++ -Wall -g idServer_client.o idServer_clnt.o ../../Common/Logger.o idServer_xdr.o test.cpp -o test

	
clean:
	- rm *.o
	- rm idServer_svc
