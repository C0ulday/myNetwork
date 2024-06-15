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
    Trame trame;

    // Sémaphore pour permettre à différents clients d'utiliser ce même code
    int sem_id = semget(SEM_KEY, NOMBRE_CLIENTS_MAX, 0666);
    struct sembuf sem_op;

    sem_op.sem_num = 0;
    sem_op.sem_op = -1; // P operation
    sem_op.sem_flg = 0;

    if (semop(sem_id, &sem_op, 1) == -1) {
        perror("semop P");
        exit(EXIT_FAILURE);
    }

    int reponse_user;
    int choix_client;
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
    while (1) {
        if (msgrcv(file_id, &message, sizeof(message) - sizeof(long), 1,
                   IPC_NOWAIT) != -1) {
            printf("( ツ ) Je suis le client %s\n", message.buffer);
            printf("Que puis-je fais pour vous ?\n");
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
                        int compteur = 0;

                        printf("A quel ami souhaitez-vous envoyer ce message "
                               "?\n N'hésitez pas à jeter un oeil à la liste "
                               "dans admin !\n");
                        printf(">>");
                        scanf("%d", &choix_client);
                        sprintf(message.buffer, "Salut !");

                        printf("( ! ) Traitement de la requête...\n");
                        printf("\tClient %d -> Client %d : %s\n",
                               message.index + 1, choix_client, message.buffer);

                        printf("Compression du message...\n");

                        Output *outputs = LZ78(message.buffer);

                        // Affichage du résultat de la compression

                        printf("Résultat de la compression LZ78 :\n");
                        // Affichage LZ78 et Hamming
                        printf("%d\n", outputs->dico->nbCellules);

                        for (int i = 0; i <= outputs->dico->nbCellules; i++) {
                            // Codage Hamming
                            int index_bin[8], lettre_bin[8];
                            int hamming1[7], hamming2[7];
                            printf("(Bloc %d ) LZ78 :", i + 1);
                            printf("\t(%d, %c)\n", outputs->bloc[i].index,
                                   outputs->bloc[i].lettre);

                            intToBinaire(index_bin, outputs->bloc[i].index);

                            // Diviser index_bin en deux groupes de 4 bits
                            int index_bin_part1[4] = {
                                index_bin[0], index_bin[1], index_bin[2],
                                index_bin[3]};
                            int index_bin_part2[4] = {
                                index_bin[4], index_bin[5], index_bin[6],
                                index_bin[7]};

                            // Encoder chaque groupe de 4 bits
                            encodeHamming(index_bin_part1, hamming1);
                            encodeHamming(index_bin_part2, hamming2);

                            for (int j = 0; j < 7; j++) {
                                trame.DU[compteur++] = hamming1[j];
                            }
                            for (int j = 0; j < 7; j++) {
                                trame.DU[compteur++] = hamming2[j];
                            }

                            charToBinaire(lettre_bin, outputs->bloc[i].lettre);

                            // Diviser lettre_bin en deux groupes de 4 bits
                            int lettre_bin_part1[4] = {
                                lettre_bin[0], lettre_bin[1], lettre_bin[2],
                                lettre_bin[3]};
                            int lettre_bin_part2[4] = {
                                lettre_bin[4], lettre_bin[5], lettre_bin[6],
                                lettre_bin[7]};

                            // Encoder chaque groupe de 4 bits
                            encodeHamming(lettre_bin_part1, hamming1);
                            encodeHamming(lettre_bin_part2, hamming2);

                            for (int j = 0; j < 7; j++) {
                                trame.DU[compteur++] = hamming1[j];
                            }
                            for (int j = 0; j < 7; j++) {
                                trame.DU[compteur++] = hamming2[j];
                            }
                        }
                        trame.id_receiverAdresse = choix_client - 1;
                        trame.id_senderAdresse = message.index;

                        trame.nb_blocs = compteur;
                        trame.type = 2;

                        if (msgsnd(file_id, &trame,
                                   sizeof(Trame) - sizeof(long), 0) == -1) {
                            perror("msgsnd");
                        }

                        // Libérer la mémoire allouée pour le dictionnaire
                        free(outputs->dico);
                        free(outputs->bloc);
                        free(outputs);
                        printf("Encodage de Hamming...\n");
                        sleep(1);
                        printf("Le message a été envoyé avec succès !\n");

                        printf("\n( ツ ) Je suis le client %s\n",
                               message.buffer);
                        printf("Que puis-je fais pour vous ?\n");
                        printf("\n");
                        print_menu(menu_title, menu_items, item_count);
                        printf(">>");
                        scanf("%d", &reponse_user);
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
