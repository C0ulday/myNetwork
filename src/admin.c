#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

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

#define NOMBRE_CLIENTS_MAX 3;

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

    // Lire l'entrée de l'utilisateur
    printf(">>");
    while (1) {
        while (scanf(" %d", &reponse_user) == 0) {
            // L'entrée n'est pas un entier valide
            printf("Entrée non valide. Veuillez entrer un entier.\n");
            scanf(" %d", &reponse_user);

            // Nettoyer le tampon d'entrée
            fgets(buffer, sizeof(buffer), stdin);
        }

        Table_Adresse table_adresse;
        table_adresse.clients = malloc(NOMBRE_CLIENTS_MAX * sizeof(Client));
        for(int i=0;i<NOMBRE_CLIENTS_MAX;i++) {
            table_adresse.clients->adresse = malloc(4 * sizeof(unsigned char));
        }
        
        
        // Création d'une file de message
        key_t cle_serveur = 1;
        int file_id  msgget(cle_serveur,IPC_CREAT |0666);

        int nombre_clients =0;

        if(file_id ==-1) {
            perror("mssget");
            exit(EXIT_FAILURE); 
        }

        switch (reponse_user) {

        case 1:

            // Lancement d'un terminal externe avec une instance de serveur
            system("xterm -title SERVEUR 'xterm -e ./sevreur &");

            break;
        case 2:
            /*NOUVEAU CLIENT*/
            printf("Création d'un nouveau client...\n");
            nombre_clients++;
            sleep(3);
            // Envoie au serveur du nouveau client
            // Type 2 = ajout de client
            table_adresse.type =2;
            table_adresse.nombre_clients = nombre_clients;
            if(msgsnd(file_id,&table_adresse,sizeof(table_adresse.ips),0)) {
                perror("msgsnd");
                exit(EXIT_FAILURE);
            }
            

            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            printf("A bientôt !");
            exit(EXIT_SUCCESS);
        }
    }

    return 0;
}