
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include "../Common/MensajeAPuerta.h"

int main(int argc,char* argv[]){
    
    int queue = (int) strtol(argv[1], NULL, 0);
    int queueID = msgget(queue,0660);
    if(queueID==-1){
        printf("No encontre la cola %x\n",queue);
        return -1;
    }else{
        printf("Parece que encontre la cola %x\n",queue);
    }
   
    MensajeAPuerta msg;
    if(msgrcv(queueID,&msg,sizeof(MensajeAPuerta)-sizeof(long),0,0)==-1){
        return -1;
    }

    printf("myType: %ld\n",msg.myType);
    printf("origen: %ld\n",msg.origen);
    printf("destino: %ld\n",msg.destino);
    printf("mensaje: %d\n",msg.mensaje);
    printf("tipo: %d\n",msg.tipo);
    printf("pertenencias o tarjeta: %d\n",msg.pertenenciasOTarjeta);
    
    if(msgsnd(queueID,&msg,sizeof(MensajeAPuerta)-sizeof(long),0)==-1){
        return -1;
    }

     
    
}
