#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <unistd.h>

#include "../header/fonctions.h"
#include "../header/types.h"

/**
 * @file client.c
 * @brief Client du réseau
 * Client permet :
 * - Ce programme pourra envoyer des datas utilisateurs (DU) qui seront des
 * caractères alphanumériques, ils seront envoyés sur le réseau uniquement avec
 * des “0” ou des “1” (langage binaire). Ces DU et le client destinataire seront
 * envoyés par l’utilisateur
 * - Réception de trame réseau : le programme sera capable d’extraire les DU à
 * partir du code binaire.
 * - Capacité à répondre au serveur qu’il est toujours en fonctionnement.
 * - Proposition à l’utilisateur des différents clients et du texte à envoyer.
 */

int main(void) {

    Table_Adresse table;
    initializeTableClients(&table, NOMBRE_CLIENTS_MAX);

    Message message;

    int sem_id = semget(SEM_KEY, NOMBRE_CLIENTS_MAX, 0666);
    struct sembuf sem_op;

    // Opération P (wait) pour décrémenter le sémaphore
    sem_op.sem_num = 0;
    sem_op.sem_op = -1; // P operation
    sem_op.sem_flg = 0;

    if (semop(sem_id, &sem_op, 1) == -1) {
        perror("semop P");
        exit(EXIT_FAILURE);
    }

    int reponse_user;
    char message_choix;

    int cle_serveur = ftok("cle.txt", 2);
    int file_id = msgget(cle_serveur, 0666);

    if (file_id == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }
    // Titre du menu
    const char *menu_title = "CLIENT - Menu Principal";

    // Items du menu
    const char menu_items[MAX_MENU_ITEMS][MENU_ITEM_LENGTH] = {
        "Envoyer un message à un ami", "Quitter"};

    // Nombre d'items dans le menu
    int item_count = 2;

    /*===================================================
    CHOIX UTILISATEUR MENU
    ===================================================*/
    /*CHOIX 2 - AJOUT CLIENT*/
    while (1) {
        if (msgrcv(file_id, &message, sizeof(message) - sizeof(long), 1,
                   IPC_NOWAIT) != -1) {
            printf("( ツ ) Je suis le client %s\n", message.buffer);
            printf("Que puis-je fais pour vous ?\n");

            msgrcv(file_id, &table, sizeof(table) - sizeof(long), 4,
                   IPC_NOWAIT);
            sem_op.sem_op = 1; // V operation
            if (semop(sem_id, &sem_op, 1) == -1) {
                perror("semop V");
                exit(EXIT_FAILURE);
            }
            // Afficher le menu
            print_menu(menu_title, menu_items, item_count);
            printf(">>");
            scanf("%d", &reponse_user);
            while (1) {
                switch (reponse_user) {
                case 1:
                    printf(
                        "( ツ ) Si j'étais vous j'enverrais ça à mon ami :\n");
                    printf(
                        "\ta. Salut ! \n"
                        "\tb. Aide-moi à faire mon exo de Maths !\n"
                        "\tc. C'est quoi ta série préférée ?\n"
                        "\td. Hello fils de fille de joie, on boit un verre ce "
                        "soir ?\n"
                        "\te. Message personnalisé\n"
                        "\tf. Quitter \n");
                    printf(">>");
                    scanf(" %c", &message_choix);

                    switch (message_choix) {
                    case 'a':
                        break;
                    case 'b':
                        break;
                    case 'c':
                        break;
                    case 'd':
                        break;
                    case 'e':
                        break;
                    case 'f':
                        /*ARRET CLIENT */
                        msgrcv(file_id, &message,
                               sizeof(message) - sizeof(long), 4, IPC_NOWAIT);
                        // Envoie un signal au serveur pour l'avertir qu'il
                        // s'arrête
                        message.type = 4;
                        msgsnd(file_id, &message,
                               sizeof(message) - sizeof(long), 0);
                        sleep(1);
                        printf("( ᵔ ᵕ ᵔ ) A bientôt !\n");
                        exit(EXIT_SUCCESS);
                    default:
                        printf("Je ne comprends pas votre choix ! \n");
                        printf("\n");
                        printf(">>");
                        break;
                    }
                    break;
                case 2:
                    /*ARRET CLIENT */
                    msgrcv(file_id, &message, sizeof(message) - sizeof(long), 4,
                           IPC_NOWAIT);
                    // Envoie un signal au serveur pour l'avertir qu'il
                    // s'arrête
                    message.type = 4;
                    if (msgsnd(file_id, &message,
                               sizeof(message) - sizeof(long), 0) == -1) {
                        perror("msgsnd");
                        exit(EXIT_FAILURE);
                    }
                    sleep(1);
                    printf("( ᵔ ᵕ ᵔ ) A bientôt !\n");
                    exit(EXIT_SUCCESS);
                default:
                    printf("Choix invalide\n");
                    scanf("%d", &reponse_user);
                    printf(">>");
                    break;
                }
            }
        }
    }
    return 0;
}
