#include "idServer_client.h"

idServer_client* idServer_client::instance = NULL;

idServer_client::idServer_client(){
    
    char* host = "localhost"; //TODO leer de archivo
    this->clnt = clnt_create (host, IDSERVERPROG, IDSERVERVERS, "udp");
    if (clnt == NULL) {
        clnt_pcreateerror (host);
        exit (1);
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
    result_1 = getnuevoidpersona_1((void*)&getnuevoidpersona_1_arg, clnt);
    if (result_1 == (id_response *) NULL) {
        clnt_perror (clnt, "call failed");
    }
    if ((*result_1).error == 0){
        Logger::loggError((*result_1).id_response_u.message);
        exit(1);
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
        exit(1);
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
        exit(1);
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
        exit(1);
    }
    return (*result_4).id_response_u.id;
}

long idServer_client::getIdPuertaEnt(long nroPuerta){
    
    id_response  *result_5;
    long  getidpuertaent_1_arg = nroPuerta;
    result_5 = getidpuertaent_1(&getidpuertaent_1_arg, clnt);
    if (result_5 == (id_response *) NULL) {
        clnt_perror (clnt, "call failed");
    }
    if ((*result_5).error == 0){
        Logger::loggError((*result_5).id_response_u.message);
        exit(1);
    }
    return (*result_5).id_response_u.id;
}

long idServer_client::getIdPuertaSal(long nroPuerta){
    
    id_response  *result_6;
    long  getidpuertasal_1_arg = nroPuerta;
    result_6 = getidpuertasal_1(&getidpuertasal_1_arg, clnt);
    if (result_6 == (id_response *) NULL) {
        clnt_perror (clnt, "call failed");
    }
    if ((*result_6).error == 0){
        Logger::loggError((*result_6).id_response_u.message);
        exit(1);
    }
    return (*result_6).id_response_u.id;
}

long idServer_client::devolverId(long id){
    
    id_response  *result_7;
    long  devolverid_1_arg = id;
    result_7 = devolverid_1(&devolverid_1_arg, clnt);
    if (result_7 == (id_response *) NULL) {
        clnt_perror (clnt, "call failed");
    }
    if ((*result_7).error == 0){
        Logger::loggError((*result_7).id_response_u.message);
        exit(1);
    }
    return (*result_7).id_response_u.id;
}

idServer_client::~idServer_client(){
    clnt_destroy (this->clnt);
    delete this->instance;
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
