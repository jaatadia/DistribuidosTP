union id_response switch (int error) {
 case 0:
   string message<>;
 case 1:
   long id;
 default:
   void;
};

program IDSERVERPROG {
     version IDSERVERVERS {
        id_response GetNuevoIDPersona() = 1;
        id_response GetNuevoIDPuertaEnt(long) = 2;
        id_response GetNuevoIDPuertaSal(long) = 3;
        id_response GetNuevoIDShMem() = 4;
        id_response GetIDPuertaEnt(long) = 5;
        id_response GetIDPuertaSal(long) = 6;
        id_response DevolverID(long) = 7;
      } = 1;
} = 0x20000001;