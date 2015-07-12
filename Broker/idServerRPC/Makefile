all: 
	g++ -Wall -g idServer_client.cpp -c 
	gcc -Wall -g idServer_xdr.c -c
	gcc -Wall -g idServer_server.c -c 
	gcc -Wall -g idServer_xdr.o idServer_server.o idServer_svc.c -o idServer_svc
	
clean:
	- rm *.o
	- rm idServer_svc
