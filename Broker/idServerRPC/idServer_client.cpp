#include "idServer_client.h"
#include <rpc/rpc.h>
#include <fstream>
#include <unistd.h>

idServer_client* idServer_client::instance = NULL;

idServer_client::idServer_client(){
    srand(time(NULL));
    char broker[255];
    bool found=false;
    std::ifstream file;
    file.open("../brokers.conf");
    if(!file.is_open()){
        Logger::loggError("no se pudo abrir el archivo");
        exit(1);
    }
    Logger::logg("Buscando broker");
    while((!found) && (!file.eof())){
        file.getline(broker,255);
        Logger::logg(std::string("Tratando de conectar con broker: ")+broker);
        this->clnt = clnt_create (broker, IDSERVERPROG, IDSERVERVERS, "tcp");
        if (clnt == NULL) {
            Logger::loggError(std::string("No se pudo conectar con broker: ")+broker+" "+clnt_spcreateerror (broker));
        }else{
            found=true;
        }
    }
    file.close();    
    if(!found){
        Logger::loggError("Error al conectarse con el broker");
        exit(1);   
    }
}

idServer_client* idServer_client::getInst(){
    if (instance == NULL) {
        instance = new idServer_client();
    }
    return instance;
}

long idServer_client::getNuevoIdPersona(){
     
    id_response  *result_1;
    char *getnuevoidpersona_1_arg;
    
    int i=1;
    while(true){
        result_1 = getnuevoidpersona_1((void*)&getnuevoidpersona_1_arg, clnt);
        if (result_1 == (id_response *) NULL) { 
            if((rpc_createerr.cf_stat==RPC_TIMEDOUT)&&(i<10)){
                i++;
                sleep(rand()%i);
            }else{
                Logger::logg(clnt_sperror(clnt, "Error pidiendo nuevo id"));
                exit(1);
            }
        }else{
            break;
        }
    }
    
    if ((*result_1).error == 0){
        Logger::loggError((*result_1).id_response_u.message);
        return -1;
        //exit(1);
    }
    return (*result_1).id_response_u.id;
}

long idServer_client::getNuevoIdPuertaEnt(long nroPuerta){
    
    id_response  *result_2;
    long  getnuevoidpuertaent_1_arg = nroPuerta;
    result_2 = getnuevoidpuertaent_1(&getnuevoidpuertaent_1_arg, clnt);
    if (result_2 == (id_response *) NULL) {
        clnt_perror (clnt, "call failed");
    }
    if ((*result_2).error == 0){
        Logger::loggError((*result_2).id_response_u.message);
        return -1;
        //exit(1);
    }
    return (*result_2).id_response_u.id;
}

long idServer_client::getNuevoIdPuertaSal(long nroPuerta){
    
    id_response  *result_3;
    long  getnuevoidpuertasal_1_arg = nroPuerta;
    result_3 = getnuevoidpuertasal_1(&getnuevoidpuertasal_1_arg, clnt);
    if (result_3 == (id_response *) NULL) {
        clnt_perror (clnt, "call failed");
    }
    if ((*result_3).error == 0){
        Logger::loggError((*result_3).id_response_u.message);
        return -1;
        //exit(1);
    }
    return (*result_3).id_response_u.id;
}

long idServer_client::getNuevoIdShMem(){
    
    id_response  *result_4;
    char *getnuevoidshmem_1_arg;
    result_4 = getnuevoidshmem_1((void*)&getnuevoidshmem_1_arg, clnt);
    if (result_4 == (id_response *) NULL) {
        clnt_perror (clnt, "call failed");
    }  
    if ((*result_4).error == 0){
        Logger::loggError((*result_4).id_response_u.message);
        return -1;
        //exit(1);
    }
    return (*result_4).id_response_u.id;
}

long idServer_client::getIdPuertaEnt(long nroPuerta){
    
    id_response  *result_5;
    long  getidpuertaent_1_arg = nroPuerta;
    int i=1;
    while(true){
        result_5 = getidpuertaent_1(&getidpuertaent_1_arg, clnt);
        if (result_5 == (id_response *) NULL) { 
            if((rpc_createerr.cf_stat==RPC_TIMEDOUT)&&(i<10)){
                i++;
                sleep(rand()%i);
            }else{
                Logger::logg(clnt_sperror(clnt, "Error consiguiendo id de la puerta de entrada"));
                exit(1);
            }
        }else{
            break;
        }
    }
    if ((*result_5).error == 0){
        Logger::loggError((*result_5).id_response_u.message);
        return -1;
        //exit(1);
    }
    return (*result_5).id_response_u.id;
    
    
}

long idServer_client::getIdPuertaSal(long nroPuerta){
    
    id_response  *result_6;
    long  getidpuertasal_1_arg = nroPuerta;
    int i=1;
    while(true){
        result_6 = getidpuertasal_1(&getidpuertasal_1_arg, clnt);
        if (result_6 == (id_response *) NULL) { 
                if((rpc_createerr.cf_stat==RPC_TIMEDOUT)&&(i<10)){
                    i++;
                    sleep(rand()%i);
                }else{
                    Logger::logg(clnt_sperror(clnt, "Error consiguiendo el id de la puerta de salida"));
                    exit(1);
                }
            }else{
                break;
            }
    }
    if ((*result_6).error == 0){
        Logger::loggError((*result_6).id_response_u.message);
        return -1;
        //exit(1);
    }
    return (*result_6).id_response_u.id;
}

long idServer_client::devolverId(long id){
    
    id_response  *result_7;
    long  devolverid_1_arg = id;
    int i=1;
    while(true){
        result_7 = devolverid_1(&devolverid_1_arg, clnt);
        if (result_7 == (id_response *) NULL) {
            if((rpc_createerr.cf_stat==RPC_TIMEDOUT)&&(i<10)){
                i++;
                sleep(rand()%i);
            }else{
                Logger::logg(clnt_sperror(clnt, "Error devolviendo el id"));
                exit(1);
            }
        }else{
            break;
        }
    }
    if ((*result_7).error == 0){
        Logger::loggError((*result_7).id_response_u.message);
        return -1;
        //exit(1);
    }
    return (*result_7).id_response_u.id;
}

idServer_client::~idServer_client(){
    clnt_destroy (this->clnt);
}

void idServer_client::destroy(){
    delete instance;
    instance=NULL;
}

/*  
void
idserverprog_1(char *host)
{
	CLIENT *clnt;
	id_response  *result_1;
	char *getnuevoidpersona_1_arg;
	id_response  *result_2;
	long  getnuevoidpuertaent_1_arg;
	id_response  *result_3;
	long  getnuevoidpuertasal_1_arg;
	id_response  *result_4;
	char *getnuevoidshmem_1_arg;
	id_response  *result_5;
	long  getidpuertaent_1_arg;
	id_response  *result_6;
	long  getidpuertasal_1_arg;
	id_response  *result_7;
	long  devolverid_1_arg;

#ifndef	DEBUG
	clnt = clnt_create (host, IDSERVERPROG, IDSERVERVERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	// DEBUG

	result_1 = getnuevoidpersona_1((void*)&getnuevoidpersona_1_arg, clnt);
	if (result_1 == (id_response *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	result_2 = getnuevoidpuertaent_1(&getnuevoidpuertaent_1_arg, clnt);
	if (result_2 == (id_response *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	result_3 = getnuevoidpuertasal_1(&getnuevoidpuertasal_1_arg, clnt);
	if (result_3 == (id_response *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	result_4 = getnuevoidshmem_1((void*)&getnuevoidshmem_1_arg, clnt);
	if (result_4 == (id_response *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	result_5 = getidpuertaent_1(&getidpuertaent_1_arg, clnt);
	if (result_5 == (id_response *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	result_6 = getidpuertasal_1(&getidpuertasal_1_arg, clnt);
	if (result_6 == (id_response *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	result_7 = devolverid_1(&devolverid_1_arg, clnt);
	if (result_7 == (id_response *) NULL) {
		clnt_perror (clnt, "call failed");
	}
        
        printf("Los resultados fueron: %d, %d, %d, %d, %d, %d, %d\n",(*result_1).id_response_u.id,(*result_2).id_response_u.id,(*result_3).id_response_u.id,(*result_4).id_response_u.id,(*result_5).id_response_u.id,(*result_6).id_response_u.id,(*result_7).id_response_u.id);
        
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 // DEBUG
}


int
main (int argc, char *argv[])
{
	char *host;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	idserverprog_1 (host);
exit (0);
 }
 */
