/* 
 * File:   InterfazPuerta.h
 * Author: knoppix
 *
 * Created on June 14, 2015, 2:58 PM
 */

#ifndef INTERFAZPUERTA_H
#define	INTERFAZPUERTA_H

class InterfazPuerta {
private:
    int colaEntrada;
    int colaEntradaRespuesta;
    int colaSalida;
    int colaSalidaRespuesta;
    
public:
    
    InterfazPuerta();

    // devuelve si pudo entrar o no 
    bool entrar(int numeroPuerta);
    
    //devuelve -1 si no pudo entrar, o la tarjeta en caso contrario
    int entrarInvestigador(int numeroPuerta,int pertenencias);
    
    //pasea por tanto tiempo o hasta que le digan que salga
    void pasear(int milisegundos);
    
    //true pudo salir, false no
    bool salir(int numeroPuerta);
    
    //devuelve -1 en caso de que no sea la puerta correcta o las pertenencias
    int salirInvestigador(int numeroPuerta,int tarjeta);
    
    virtual ~InterfazPuerta();


};

#endif	/* INTERFAZPUERTA_H */

