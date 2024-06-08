#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "../header/fonctions.h"
#include "../header/types.h"

/**
 * @file serveur.c
 * @brief Serveur du réseau
 * Serveur permet :
 * - Ce programme recevra des trames des programmes clients et les enverra au
 * programme client correspondant.Il ne modifiera pas la trame.
 * - En cas de destinataire inconnu ou injoignable, le serveur enverra un
 * message spécifique au client.
 * - Le serveur est chargé de vérifier régulièrement si les clients sont
 * “vivants” en cas de non-réponse, le serveur ne redirige plus les trames vers
 * ce client et prévient les autres clients.
 * - En cas de nouveau client le serveur lui alloue une IP et prévient
 * l'apparition aux autres clients.
 */

int main(void) {

    /*===================================================
    CHOIX UTILISATEUR MENU
    ===================================================*/

    Table_Adresse table;
    // Initilisation de la table des clients
    initializeTableClients(&table, NOMBRE_CLIENTS_MAX);

    // Déclaration de la clé de file Serveur - Admin
    key_t cle_admin = ftok("cle.txt", 1);
    ;
    // Déclaration de la clé de file Serveur - Client
    key_t cle_client = ftok("cle.txt", 2);
    int file_id = msgget(cle_admin, 0666);

    if (file_id == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    while (1) {

        /*LANCEMENT SERVEUR*/
        if (msgrcv(file_id, &table, sizeof(Table_Adresse) - sizeof(long), 1,
                   0) == -1) {
            perror("msgrcv");
            exit(EXIT_FAILURE);
        }
        if (table.type == 1) {
            /*===================================================
            INITIALISATION MENU
            ===================================================*/
            printf("SERVEUR - Menu Principal\n");
            printf("--------------------------------------------------\n");
            printf("En attente de requêtes...\n");
        }
        /*AJOUT CLIENT - ATTRIBUTION ADRESSE IP*/
        // Type 2 = ajout de client
        if (msgrcv(file_id, &table, sizeof(Table_Adresse) - sizeof(long), 2,
                   0) == -1) {
            perror("msgrcv");
            exit(EXIT_FAILURE);
        }
        if (table.type == 2) {
            // Ajout de client
            printf("( ! ) Nouvelle requête : attribution d'adresse IP à un "
                   "client\n");
            int index_client = addClient(&table);
            printf("( + ) Client %d : %u.%u.%u.%u\n",
                   table.clients[index_client].num,
                   table.clients[index_client].adresseIP.adresse[0],
                   table.clients[index_client].adresseIP.adresse[1],
                   table.clients[index_client].adresseIP.adresse[2],
                   table.clients[index_client].adresseIP.adresse[3]);
        }
    }

    return 0;
}