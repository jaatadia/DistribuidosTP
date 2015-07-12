#ifndef ID_SERVER_CLIENT
#define ID_SERVER_CLIENT

#include "idServer.h"
#include "../../Common/Logger.h"

class idServer_client {
    
private:
    idServer_client();
    static idServer_client* instance;
    CLIENT* clnt;
    
public:
    static idServer_client* getInst();
    
    long getNuevoIdPersona();
    long getNuevoIdPuertaEnt(long nroPuerta);
    long getNuevoIdPuertaSal(long nroPuerta);
    long getNuevoIdShMem();
    long getIdPuertaEnt(long nroPuerta);
    long getIdPuertaSal(long nroPuerta);
    long devolverId(long id);
    
    virtual ~idServer_client();
    
};
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

#endif /* ID_SERVER_CLIENT */