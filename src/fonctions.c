#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../header/fonctions.h"
#include "../header/types.h"

/**
 * @file fonctions.c
 * @brief contient des fonctions utiles au programme.
 */

/*===================================================

FONCTIONS ADMIN

===================================================*/
/*MENU*/

/**
 * @brief Affiche un menu avec des tirets pour séparer les sections.
 *
 * @param title Le titre du menu.
 * @param items Le tableau des items du menu.
 * @param item_count Le nombre d'items dans le menu.
 */
void print_menu(const char *title, const char items[][MENU_ITEM_LENGTH],
                int item_count) {
    // Afficher le titre du menu
    printf("%s\n", title);

    // Afficher les tirets
    for (int i = 0; i < SEPARATOR_LENGTH; i++) {
        printf("-");
    }
    printf("\n");

    // Afficher les items du menu
    for (int i = 0; i < item_count; i++) {
        printf("%d. %s\n", i + 1, items[i]);
    }

    // Afficher les tirets à la fin du menu
    for (int i = 0; i < SEPARATOR_LENGTH; i++) {
        printf("-");
    }
    printf("\n");
}

/*===================================================

FONCTIONS SERVEUR

===================================================*/
/*TABLE ADRESSE*/
/**
 * @brief Initialise une table de clients.
 *
 * @param table L'adresse de la table.
 * @param nombre_clients_max Le nombre de clients que contiendra la table de
 * clients.
 */
void initializeTableClients(Table_Adresse *table, int nombre_clients_max) {

    if (table == NULL) {
        perror("table is NULL");
        exit(EXIT_FAILURE);
    }

    table->type = 0; // Initialisation de type, ajuster selon vos besoins
    table->nombre_clients = 0;

    // Initialiser les adresses IP des clients à 0
    for (int i = 0; i < nombre_clients_max; i++) {
        for (int j = 0; j < 4; j++) {
            table->clients[i].adresseIP.adresse[j] = 0;
        }
        table->clients[i].num = 0;
    }
}
/**
 * @brief Vérifie que l'adresse IP d'un client est nulle, c.à.d 0.0.0.0.
 * @param table Pointeur vers tableau des clients.
 * @param index_client Indice du client à vérifier.
 * @return 1 si l'adresse est nulle, -1 sinon.
 */
int nullClient(Table_Adresse *table, int index_client) {
    int client_existe =
        existAdresseIP(*table, table->clients[index_client].adresseIP);
    if (client_existe != -1) {
        if (table->clients[client_existe].adresseIP.adresse[0] == 0 &&
            table->clients[client_existe].adresseIP.adresse[1] == 0 &&
            table->clients[client_existe].adresseIP.adresse[2] == 0 &&
            table->clients[client_existe].adresseIP.adresse[3] == 0) {
            return 1;
        }
    }
    return -1;
}

/*ADRESSES IP*/
/**
 * @brief Vérifie l'existence d'une adresse IP dans une table d'adresses.
 *
 * @param table Tableau des clients.
 * @param ip L'adresse IP a vérifié.
 * @return La position du client si l'adresse est trouvée, -1 sinon.
 */
int existAdresseIP(Table_Adresse table, Adresse_IP ip) {

    int n = table.nombre_clients;
    for (int i = 0; i < n; i++) {
        if (table.clients[i].adresseIP.adresse[0] == ip.adresse[0] &&
            table.clients[i].adresseIP.adresse[1] == ip.adresse[1] &&
            table.clients[i].adresseIP.adresse[2] == ip.adresse[2] &&
            table.clients[i].adresseIP.adresse[3] == ip.adresse[3]) {
            return i;
        }
    }
    return -1;
}

/**
 * @brief Génère une adresse IP qui n'a pas été déjà attribuée dans le réseau.
 * @param table Tableau des clients.
 * @param ip L'adresse IP a vérifié.
 * @return Void : modifie directement ip.
 */
void generateAdresseIP(Table_Adresse table, Adresse_IP *ip) {

    // Initialiser le générateur de nombres aléatoires
    srand(time(NULL));
    do {
        for (int i = 0; i < 4; i++) {
            ip->adresse[i] = (unsigned char)(rand() % 256);
        }
    } while (existAdresseIP(table, *ip) != -1);
}

/**
 * @brief Ajoute un nouveau client dans la table de clients avec une adresse IP
 * valide.
 * @param table Tableau des clients.
 * @return La position du nouveau client dans la table.
 */
int addClient(Table_Adresse *table) {

    Adresse_IP ip;
    int n = table->nombre_clients;
    int i = 0;

    for (i = 0; i < n; i++) {
        if (table->clients[i].adresseIP.adresse[0] == 0 &&
            table->clients[i].adresseIP.adresse[1] == 0 &&
            table->clients[i].adresseIP.adresse[2] == 0 &&
            table->clients[i].adresseIP.adresse[3] == 0) {
            break;
        }
    }
    generateAdresseIP(*table, &ip);
    table->clients[i].adresseIP = ip;
    table->clients[i].num = n + 1;
    table->nombre_clients++;

    return i;
}

/**
 * @brief Supprime un client de la table en mettant son adresse à zéro.
 * @param table Tableau des clients.
 * @param ip L'adresse IP a vérifié.
 * @return Void : modifie directement ip.
 */
void suppClient(Table_Adresse *table, Adresse_IP *ip) {

    int client_existe = existAdresseIP(*table, *ip);

    if (client_existe != -1) {
        for (int i = 0; i < 4; i++) {
            table->clients[client_existe].adresseIP.adresse[i] = 0;
        }
    }
    table->clients[client_existe].num = 0;
}
