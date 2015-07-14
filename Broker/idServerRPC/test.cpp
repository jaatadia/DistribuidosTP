#include "idServer_client.h"
#include "../../Common/Logger.h"
#include <stdio.h>

int main(int argc, char* argv[]){
    
        Logger::startLog("./log.txt","test");
    if(argc==2){
        for (int i=0;i<atoi(argv[1]);i++){
            long first = idServer_client::getInst()->getNuevoIdPersona();
            printf("%ld\n",first);
            printf("%ld\n",idServer_client::getInst()->getNuevoIdPersona());
            printf("%ld\n",idServer_client::getInst()->getNuevoIdPersona());
            printf("%ld\n",idServer_client::getInst()->getNuevoIdPersona());
            printf("%ld\n",idServer_client::getInst()->getNuevoIdPersona());
            printf("%ld\n",idServer_client::getInst()->getNuevoIdPersona());
            printf("%ld\n",idServer_client::getInst()->getNuevoIdPersona());
            printf("%ld\n",idServer_client::getInst()->getNuevoIdPersona());
            printf("%ld\n",idServer_client::getInst()->getNuevoIdPersona());
            printf("%ld\n",idServer_client::getInst()->getNuevoIdPersona());
            printf("%ld\n",idServer_client::getInst()->getIdPuertaEnt(1));
            printf("%ld\n",idServer_client::getInst()->getIdPuertaSal(1));
            printf("%ld\n",idServer_client::getInst()->devolverId(first));
        }
    }else{
        for (int i=atoi(argv[1]);i<atoi(argv[2]);i++){
            printf("%ld\n",idServer_client::getInst()->devolverId(i));
        }
    }
        
        Logger::closeLogger();
        
	return 0;
}
