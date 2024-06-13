#include "../header/fonctions.h"
#include "../header/types.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <unistd.h>

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

    // Déclaration sémaphore pour clients
    int sem_id = semget(SEM_KEY, NOMBRE_CLIENTS_MAX, IPC_CREAT | 0666);
    if (sem_id == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }
    if (semctl(sem_id, 0, SETVAL, 1) == -1) {
        perror("semctl");
        exit(EXIT_FAILURE);
    }

    Table_Adresse table;
    // Initilisation de la table des clients
    initializeTableClients(&table, NOMBRE_CLIENTS_MAX);

    // Message que le serveur enverra au client si besoin pour l'avertir
    Message message;

    // Déclaration de la clé de file Serveur - Admin
    key_t cle_admin = ftok("cle.txt", 1);

    // Déclaration de la clé de file Serveur - Client
    key_t cle_client = ftok("cle.txt", 2);

    int file_id = msgget(cle_admin, 0666);
    int file_id_client = msgget(cle_client, IPC_CREAT | 0666);

    if (file_id == -1 || file_id_client == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    // Déclaration de la trame
    Trame trame;

    /*LANCEMENT SERVEUR*/
    if (msgrcv(file_id, &table, sizeof(Table_Adresse) - sizeof(long), 1, 0) ==
        -1) {
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
        if (msgrcv(file_id, &table, sizeof(Table_Adresse) - sizeof(long), 1,
                   IPC_NOWAIT) != -1) {
            printf("( !! ) Je suis déjà lancé !\n");
            printf("En attente de requêtes...\n");
        }
        /*CHOIX 2 - AJOUT CLIENT*/
        // Le message est envoyé sans attendre forcément de réponse :
        // non-bloquant
        msgrcv(file_id, &table, sizeof(Table_Adresse) - sizeof(long), 2,
               IPC_NOWAIT);

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
            /*PAQUET ADMIN*/
            table.type = 6;
            if (msgsnd(file_id, &table, sizeof(Table_Adresse) - sizeof(long),
                       0) == -1) {
                perror("msgsnd");
                exit(EXIT_FAILURE);
            }
            // Le serveur se charge de lancer le nouveau client automatiquement
            // si xterm est installé
#ifdef USE_XTERM
            if (system("xterm -e './client' &") != 0) {
                perror("system");
                exit(EXIT_FAILURE);
            }
#endif
            // Le serveur attend 1 seconde pour laisser l'admin récupérer la
            // table m.à.j
            sleep(1);

            /* PAQUET CLIENT*/

            // Envoie du message au client
            message.type = 1;
            message.index = index_client;
            sprintf(message.buffer, "%d : %u.%u.%u.%u\n",
                    table.clients[index_client].num,
                    table.clients[index_client].adresseIP.adresse[0],
                    table.clients[index_client].adresseIP.adresse[1],
                    table.clients[index_client].adresseIP.adresse[2],
                    table.clients[index_client].adresseIP.adresse[3]);
            if (msgsnd(file_id_client, &message, sizeof(Message) - sizeof(long),
                       0)) {
                perror("msgsnd");
                exit(EXIT_FAILURE);
            }
            if (msgsnd(file_id_client, &table,
                       sizeof(Table_Adresse) - sizeof(long), 0)) {
                perror("msgsnd");
                exit(EXIT_FAILURE);
            }
        }
        /*CHOIX 3 - SUPPRESSION CLIENT*/
        if (msgrcv(file_id, &table, sizeof(Table_Adresse) - sizeof(long), 3,
                   IPC_NOWAIT) != -1) {
            printf("( ! ) Suppression d'un client\n");
            printf("%s\n", table.buffer);

            // Le serveur renvoie la table m.à.j à l'admin
            // Type = 6 est le type par défaut
            // Cette affectation permet de sortir du càs Type =3
            table.type = 6;
            if (msgsnd(file_id, &table, sizeof(Table_Adresse) - sizeof(long),
                       0) == -1) {
                perror("msgsnd");
                exit(EXIT_FAILURE);
            }

            // Shut down le client concerné
            message.type = 4;
            if (msgsnd(file_id_client, &message, sizeof(Message) - sizeof(long),
                       0)) {
                perror("msgsnd");
                exit(EXIT_FAILURE);
            }

            // Le serveur attend 1 seconde pour laisser l'admin récupérer la
            // table m.à.j
            sleep(1);
        }
        sleep(1);

        /*CHOIX 5 - ARRET DE TOUT*/
        if (msgrcv(file_id, &table, sizeof(Table_Adresse) - sizeof(long), 5,
                   IPC_NOWAIT) != -1) {
            message.type = 7;
            if (msgsnd(file_id_client, &message, sizeof(message) - sizeof(long),
                       0) == -1) {
                perror("msgsnd");
                exit(EXIT_FAILURE);
            }
            if (msgctl(file_id_client, IPC_RMID, NULL) == -1) {
                perror("msgctl");
                exit(EXIT_FAILURE);
            }
            printf("( !! ) Arrêt du serveur\n");
            exit(EXIT_SUCCESS);
        }
        /* CLIENT QUITTE*/

        if (msgrcv(file_id_client, &message, sizeof(Message) - sizeof(long), 4,
                   IPC_NOWAIT) != -1) {
            printf("( !! ) Client %d : %u.%u.%u.%u s'est déconnecté\n",
                   table.clients[message.index].num,
                   table.clients[message.index].adresseIP.adresse[0],
                   table.clients[message.index].adresseIP.adresse[1],
                   table.clients[message.index].adresseIP.adresse[2],
                   table.clients[message.index].adresseIP.adresse[3]);
            suppClient(&table, &table.clients[message.index].adresseIP);
        }
        /*REQUETE GENERAL DU CLIENT*/
        // Si le client envoie requête type 1, le serveur renvoie la table
        if (msgrcv(file_id_client, &message, sizeof(Message) - sizeof(long), 1,
                   IPC_NOWAIT) == 1) {
            if (msgsnd(file_id_client, &table,
                       sizeof(Table_Adresse) - sizeof(long), 0) == -1) {
                perror("msgsnd");
                exit(EXIT_FAILURE);
            }
        }

        if (msgrcv(file_id_client, &trame, sizeof(Trame) - sizeof(long), 10,
                   IPC_NOWAIT) != -1) {

            int encode7[7];

            for (int i = 0; i < trame.nb_blocs; i++) {
                for (int j = 0; j < 7; j++) {
                    encode7[i] = trame.DU[i];
                }
            }
        }
    }
    return 0;
}