#ifndef _TYPES_H_
#define _TYPES_H_

/**
 * @file types.h
 * @brief Contient les constantes et les structures définies.
 */

#define NOMBRE_CLIENTS_MAX 3

typedef struct {
    unsigned char adresse[4];
} Adresse_IP;

typedef struct {
    Adresse_IP adresseIP;
    int num;
} Client;

typedef struct {
    long type;
    int nombre_clients;
    Client clients[NOMBRE_CLIENTS_MAX];
} Table_Adresse;

#endif // !_TYPES_H_
