#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

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

    // Déclaration de la clé de file Serveur - Client

    int file_id = msgget(cle_admin, 0666);

    if (file_id == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    /*LANCEMENT SERVEUR*/
    if (msgrcv(file_id, &table, sizeof(Table_Adresse), 1, 0) == -1) {
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

    while (1) {

        /*CHOIX 1 - LANCEMENT SERVEUR*/
        if (msgrcv(file_id, &table, sizeof(Table_Adresse), 1, IPC_NOWAIT) !=
            -1) {
            printf("( !! ) Je suis déjà lancé !\n");
            printf("En attente de requêtes...\n");
        }
        /*CHOIX 2 - AJOUT CLIENT*/
        // Le message est envoyé sans attendre forcément de réponse :
        // non-bloquant
        msgrcv(file_id, &table, sizeof(Table_Adresse), 2, IPC_NOWAIT);

        if (table.type == 2) {
            printf("( ! ) Nouvelle requête : Allocation d'une adresse IP "
                   "client\n");
            int index_client = addClient(&table);
            printf("\t( + ) Client %d : %u.%u.%u.%u\n",
                   table.clients[index_client].num,
                   table.clients[index_client].adresseIP.adresse[0],
                   table.clients[index_client].adresseIP.adresse[1],
                   table.clients[index_client].adresseIP.adresse[2],
                   table.clients[index_client].adresseIP.adresse[3]);
            // Le serveur renvoie la table m.à.j à l'admin
            // Type = 6 est le type par défaut
            // Cette affectation permet de sortir du càs Type =2
            table.type = 6;
            if (msgsnd(file_id, &table, sizeof(Table_Adresse), 0) == -1) {
                perror("msgsnd");
                exit(EXIT_FAILURE);
            }
            // Le serveur attend 1 seconde pour laisser l'admin récupérer la
            // table m.à.j
            sleep(1);
        }
        /*CHOIX 3 - SUPPRESSION CLIENT*/
        sleep(1);
        msgrcv(file_id, &table, sizeof(Table_Adresse), 3, IPC_NOWAIT);
        if (table.type == 3) {
            printf("( - ) Nouvelle requête : Suppression d'un client\n");
            printf("%s", table.buffer);
        }
    }

    return 0;
}