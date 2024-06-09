#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#include "../header/fonctions.h"
#include "../header/types.h"

/**
 * @file admin.c
 * @brief Admin du réseau
 * Admin permet :
 * - Lancement du programme serveur.
 * - Création des nouveaux clients (pouvant être appelé pendant le
 * fonctionnement).
 * - Arrêt de certains client(pouvant être appelé pendant le fonctionnement).
 *
 * @details
 * Les fonctions de ce fichier sont utilisées pour administrer le réseau en
 * gérant les serveurs et les clients. Cela inclut la capacité de démarrer et
 * d'arrêter des clients dynamiquement.
 */

int main(void) {

    /*===================================================
    INITIALISATION MENU
    ===================================================*/
    int reponse_user;
    char buffer[64];

    // Titre du menu
    const char *menu_title = "ADMIN - Menu Principal";

    // Items du menu
    const char menu_items[MAX_MENU_ITEMS][MENU_ITEM_LENGTH] = {
        "Lancer le serveur", "Créer un nouveau client", "Arrêter un client",
        "Afficher la liste des clients", "Quitter"};

    // Nombre d'items dans le menu
    int item_count = 5;

    // Afficher le menu
    print_menu(menu_title, menu_items, item_count);

    /*===================================================
    CHOIX UTILISATEUR MENU
    ===================================================*/
    Table_Adresse table;
    // Initilisation de la table des clients
    initializeTableClients(&table, NOMBRE_CLIENTS_MAX);

    // Création de la file de message Admin - Serveur
    key_t cle_serveur = ftok("cle.txt", 1);
    int file_id = msgget(cle_serveur, IPC_CREAT | 0666);
    if (file_id == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    // Lire l'entrée de l'utilisateur
    printf(">>");
    while (1) {
        while (scanf(" %d", &reponse_user) == 0) {
            // L'entrée n'est pas un entier valide
            printf(
                "Entrée non valide. Veuillez entrer un entier entre 1 et 6.\n");
            print_menu(menu_title, menu_items, item_count);
            printf(">>");
            scanf("%d", &reponse_user);

            // Nettoyer le tampon d'entrée
            fgets(buffer, sizeof(buffer), stdin);
        }

        switch (reponse_user) {

        case 1:

            // Lancement d'un terminal externe avec une instance de serveur
            printf("Lancement du serveur...\n");
            // Type 1 = message normal
            table.type = 1;
            if (msgsnd(file_id, &table, sizeof(Table_Adresse) - sizeof(long),
                       0) == -1) {
                perror("msgsnd");
                exit(EXIT_FAILURE);
            }
            printf("Serveur lancé avec succès !\n");
            printf("\n");
            print_menu(menu_title, menu_items, item_count);
            printf(">>");
            break;
        case 2:
            /*NOUVEAU CLIENT*/
            if (table.nombre_clients < NOMBRE_CLIENTS_MAX) {
                printf("Création d'un nouveau client...\n");
                // Envoie au serveur du nouveau client
                // Type 2 = ajout de client
                table.type = 2;
                if (msgsnd(file_id, &table,
                           sizeof(Table_Adresse) - sizeof(long), 0) == -1) {
                    perror("msgsnd");
                    exit(EXIT_FAILURE);
                }
                // Attendre 1 seconde pour laisser le serveur récupérer la
                // table m.à.j.
                sleep(1);
                // L'admin réucprère la table m.à.j. du serveur
                if (msgrcv(file_id, &table,
                           sizeof(Table_Adresse) - sizeof(long), 6, 0) == -1) {
                    perror("msgrcv");
                    exit(EXIT_FAILURE);
                }
                printf("\n");
                print_menu(menu_title, menu_items, item_count);
                printf(">>");
            } else {
                printf("Nombre limite de clients atteint !\n");
                printf("\n");
                print_menu(menu_title, menu_items, item_count);
                printf(">>");
            }

            break;
        case 3:
            /*ARRÊT CLIENT*/
            if (table.nombre_clients > 0) {
                printf("Quel client souhaitez-vous arrêter ?\n");
                // Afficher la liste des clients
                printClient(table);
                // Demander à l'utilisateur de choisir un client
                int client_a_arreter = 0;
                printf(">>");
                scanf("%d", &client_a_arreter);
                client_a_arreter--;
                // Supprimer le client
                if (client_a_arreter < 0 ||
                    nullClient(table, client_a_arreter) == 1) {
                    printf("Client non trouvé !\n");
                    printf("\n");
                    print_menu(menu_title, menu_items, item_count);
                    printf(">>");
                    break;
                }
                // Envoie du client supprimé au serveur
                sprintf(table.buffer, "\t( - ) Client %d: %u.%u.%u.%u\n",
                        table.clients[client_a_arreter].num,
                        table.clients[client_a_arreter].adresseIP.adresse[0],
                        table.clients[client_a_arreter].adresseIP.adresse[1],
                        table.clients[client_a_arreter].adresseIP.adresse[2],
                        table.clients[client_a_arreter].adresseIP.adresse[3]);
                suppClient(&table, &table.clients[client_a_arreter].adresseIP);
                // Type 3 = arrêt de client
                table.type = 3;
                if (msgsnd(file_id, &table,
                           sizeof(Table_Adresse) - sizeof(long), 0) == -1) {
                    perror("msgsnd");
                    exit(EXIT_FAILURE);
                }
                printf("\n");
                print_menu(menu_title, menu_items, item_count);
                printf(">>");
            } else {
                printf("Aucun client à arrêter !\n");
                printf("\n");
                print_menu(menu_title, menu_items, item_count);
                printf(">>");
            }
            break;
        case 4:
            /*AFFICHER LISTE CLIENTS*/
            msgrcv(file_id, &table, sizeof(Table_Adresse) - sizeof(long), 6,
                   IPC_NOWAIT);
            printf("LISTE DES CLIENTS\n");
            if (table.nombre_clients == 0) {
                printf("Aucun client enregistré !\n");
                printf("\n");
                print_menu(menu_title, menu_items, item_count);
                printf(">>");
                break;
            }
            printf("Nombre de clients : %d\n", table.nombre_clients);
            printClient(table);
            printf("\n");
            print_menu(menu_title, menu_items, item_count);
            printf(">>");
            break;
        case 5:
            printf("( ᵔ ᵕ ᵔ ) A bientôt !\n");
            // Envoie un signal au serveur pour qu'il se termine
            table.type = 5;
            msgsnd(file_id, &table, sizeof(Table_Adresse) - sizeof(long), 0);
            sleep(1);
            if (msgctl(file_id, IPC_RMID, NULL) == -1) {
                perror("msgctl");
            }
            exit(EXIT_SUCCESS);
        default:
            printf("凸( •̀_•́ )凸 Arrêtez de faire le fou/la folle !\n");
            print_menu(menu_title, menu_items, item_count);
            printf(">>");
        }
    }

    return 0;
}