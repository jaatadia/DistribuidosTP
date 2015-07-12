
#ifndef CONECT_TO
#define CONECT_TO

#include "inet.h"


#define PATH_CLIENTE_CS "../Common/CSCliente"
#define NAME_CLIENTE_CS "CSCliente"

#define PATH_CLIENTE_CE "../Common/CECliente"
#define NAME_CLIENTE_CE "CECliente"

#define PATH_CLIENTE_CS_MUSEO "../Common/CSClienteMuseo"
#define NAME_CLIENTE_CS_MUSEO "CSClienteMuseo"

#define PATH_CLIENTE_CE_MUSEO "../Common/CEClienteMuseo"
#define NAME_CLIENTE_CE_MUSEO "CEClienteMuseo"

int conectTo(const char* normbreBroker, long id, long ftokCE, long ftokCS, int portCE, int portCS);
int conectToSHM(const char* normbreBroker, long id, long ftokCE, long ftokCS, int portCE, int portCS);

#endif