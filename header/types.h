#ifndef _TYPES_C_
#define _TYPES_C_

/**
 * @file types.c
 * @brief contient les constantes et structures définies.
 */

typedef struct {
    unsigned char adresse[4];
} Adresse_IP;

typedef struct{
  Adresse_IP adresse;
  int num;
}Client;

typedef struct {
    long type;
    int nombre_clients;
    Client* clients;
} Table_Adresse;


#endif // !_TYPES_C_