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
 * @param table Tableau des clients.
 * @param index_client Indice du client à vérifier.
 * @return 1 si l'adresse est nulle, -1 sinon.
 */
int nullClient(Table_Adresse table, int index_client) {

    int result = -1;
    int client_existe =
        existAdresseIP(table, table.clients[index_client].adresseIP);
    if (client_existe != -1) {
        if (table.clients[client_existe].adresseIP.adresse[0] == 0 &&
            table.clients[client_existe].adresseIP.adresse[1] == 0 &&
            table.clients[client_existe].adresseIP.adresse[2] == 0 &&
            table.clients[client_existe].adresseIP.adresse[3] == 0) {
            result = 1;
        }
    }
    return result;
}

/**
 * @brief Affiche les clients d'adresse IP non nulle.
 * @param table Pointeur vers tableau des clients.
 * @return Void : affiche directement les clients d'adresse IP non nulle.
 */
void printClient(Table_Adresse table) {

    for (int i = 0; i < NOMBRE_CLIENTS_MAX; i++) {
        printf("val i : %dval :%d\n", i, nullClient(table, i));
        if (nullClient(table, i) != 1) {
            printf("( ツ ) Client %d : %u.%u.%u.%u\n", table.clients[i].num,
                   table.clients[i].adresseIP.adresse[0],
                   table.clients[i].adresseIP.adresse[1],
                   table.clients[i].adresseIP.adresse[2],
                   table.clients[i].adresseIP.adresse[3]);
        }
        continue;
    }
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
    int i = 0;
    int n = table->nombre_clients;

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
    table->clients[i].num = i + 1;
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
    table->nombre_clients--;
}

/*===================================================
FONCTIONS CLIENT
===================================================*/

/*COMPRESSION  LZ78*/
void initialiserDico(Dico *dico, int taille) {

    dico->cellule = malloc(taille * sizeof(Cellule));
    if (dico->cellule == NULL) {
        printf("Erreur lors de l'allocation mémoire.\n");
        exit(EXIT_FAILURE); // Quitter le programme en cas d'échec d'allocation
    }
    dico->nbCellules = 0; // Initialiser le nombre de cellules à 0
    dico->cellule[0].index = -1;
}

void ajouterDansDico(Dico *dico, int index, char *mot) {

    if (dico->nbCellules < MAX_MOTS_DICO) {
        dico->cellule[dico->nbCellules].index = index;
        strcpy(dico->cellule[dico->nbCellules].mot, mot);
        dico->nbCellules++;
    } else {
        printf("Le dictionnaire est plein.\n");
    }
    dico->cellule[dico->nbCellules].index = -1; // marque la fin du mot
}

int DansDico(Dico *dico, char *element) {

    int i = 0;
    while (dico->cellule[i].index != -1) {
        if (strcmp(dico->cellule[i].mot, element) == 0) {
            return 1;
        }
        i++;
    }
    return 0;
}

int rechercheIndexDansDico(Dico *dico, char *mot) {

    for (int i = 0; i < dico->nbCellules; i++) {
        if (strcmp(mot, dico->cellule[i].mot) == 0) {
            return dico->cellule[i].index;
        }
    }
    return 0;
}

/*Dans un dico, il faut tenir compte du dernier élément pour marquer la fin :
si j'ai 3 cellules, j'en initialise 4 pour le token de fin
*/
Output *LZ78(char *message) {

    Dico *dico = malloc(sizeof(Dico));
    Output *outputs = malloc(sizeof(Output));

    if (dico == NULL || outputs == NULL) {
        printf("Erreur lors de l'allocation mémoire pour Dico ou Outputs.\n");
        exit(EXIT_FAILURE);
    }

    initialiserDico(dico, MAX_MOTS_DICO);
    outputs->dico = dico;
    outputs->bloc = malloc(dico->nbCellules * sizeof(Bloc));

    char temp[SIZE_MOT_DICO];
    int taille = strlen(message);
    int curseur = 0;
    int nbCellules = 0;
    char new_temp[SIZE_MOT_DICO];

    /*On rempli le dico de la prmeière lettre */

    /*Conversion du caractère en chaine pour pouvoir le passer dans les
     * fonctions*/

    temp[0] = message[0];

    outputs->bloc[nbCellules].lettre = temp[0];
    outputs->bloc[nbCellules].index = 0;

    temp[1] = '\0';
    ajouterDansDico(dico, ++nbCellules, temp);

    // On vide le buffer
    strcpy(temp, "");

    /*Je parcours le message*/
    for (int i = 1; i < taille; i++) {
        /*Je convertis d'abord le caractère courant en string*/
        temp[curseur++] = message[i];
        temp[curseur] = '\0';

        /*Si la chaîne n'est pas dans le dico, je l'ajoute*/

        if (DansDico(dico, temp) == 0) {

            outputs->bloc[nbCellules].lettre = temp[curseur - 1];
            outputs->bloc[nbCellules].index =
                rechercheIndexDansDico(dico, temp);
            ajouterDansDico(dico, ++nbCellules, temp);
        }

        /*Si la chaîne est déjà dans le dico, j'avance d'un caractère jusqu'à
        obtenir un mot qui n'est pas encore dans le dico*/

        else {
            // On s'assure d'incrémenter i seulement si on ne dépasse pas la
            // taille du message
            if (i + 1 <= taille) {
                temp[curseur++] = message[++i];
                temp[curseur] = '\0';

                while ((DansDico(dico, temp) == 1) && (i + 1 <= taille)) {
                    temp[curseur++] = message[++i];
                    temp[curseur] = '\0';
                }
                outputs->bloc[nbCellules].lettre = temp[curseur - 1];
                strncpy(new_temp, temp, curseur - 1);
                new_temp[curseur - 1] = '\0';
                outputs->bloc[nbCellules].index =
                    rechercheIndexDansDico(dico, new_temp);
                strcpy(new_temp, "");
            }
            ajouterDansDico(dico, ++nbCellules, temp);
        }
        // on vide le buffer
        curseur = 0;
        strcpy(temp, "");
    }

    return outputs;
}