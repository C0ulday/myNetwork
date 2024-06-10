#ifndef _TYPES_H_
#define _TYPES_H_

/**
 * @file types.h
 * @brief Contient les constantes et les structures définies.
 */

#define NOMBRE_CLIENTS_MAX 3
#define SEM_KEY 1234
#define MSG_SIZE 16
#define SIZE_MOT_DICO 30
#define MAX_MOTS_DICO 100
#define USE_XTERM

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
    char buffer[256];
    Client clients[NOMBRE_CLIENTS_MAX];
} Table_Adresse;

typedef struct {
    long type;
    int index;
    char buffer[256];
} Message;

/**
 * @struct Trame
 * @brief Crée une trame
 * @details type : Permet de donner le type de message.(Champ obligatoire pour
 * l'usage de file de message)
 * @details DU :
 */
typedef struct {
    long type;
    int DU[MSG_SIZE];
    Adresse_IP senderAdresse;
    Adresse_IP receiverAdresse;
    int nb_transmissions;

} Trame;

/**
 * @struct Cellule
 * @brief  Réprésente un motif de caractères dans la réprésentation LZ78
 */
typedef struct {
    int index;
    char mot[SIZE_MOT_DICO];
} Cellule;

/**
 * @struct Dictionnaire
 * @brief  Contient un ensemble de cellules
 */
typedef struct {
    Cellule *cellule;
    int nbCellules;
} Dico;

/**
 * @struct Bloc
 * @brief  Est un bloc de la sortie de la compression LZ78
 */
typedef struct {
    int index;
    char lettre;
} Bloc;

/**
 * @struct Output
 * @brief  Est la sortie de la compression LZ78
 */
typedef struct {
    Dico *dico;
    Bloc *bloc;
} Output;

#endif // !_TYPES_H_
