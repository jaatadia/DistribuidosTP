#include "idServer_client.h"
#include "../../Common/Logger.h"
#include <stdio.h>

int main(int argc, char* argv[]){
        Logger::startLog("./log.txt","test");
	printf("%ld\n",idServer_client::getInst()->getNuevoIdPersona());
	printf("%ld\n",idServer_client::getInst()->getNuevoIdPuertaEnt(9));
	printf("%ld\n",idServer_client::getInst()->getNuevoIdPuertaSal(222));
	printf("%ld\n",idServer_client::getInst()->getNuevoIdShMem());
	printf("%ld\n",idServer_client::getInst()->getNuevoIdPersona());
        Logger::closeLogger();
        
	return 0;
}
