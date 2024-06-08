#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../header/fonctions.h"
#include "../header/types.h"

/**
 * @file fonctions.c
 * @brief contient des focntions utiles au programme.
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

/*ADRESSES IP*/
/**
 * @brief Vérifie l'existence d'une adresse IP dans une table d'adresses.
 *
 * @param ips La table d'adresses.
 * @param n La taille de la table d'adresses.
 * @param ip L'adresse IP a vérifié.
 * @return 1 si l'adresse est trouvée, 0 sinon.
 */
int existAdresseIP(Adresse_IP *ips, int n, Adresse_IP *ip) {
    for (int i = 0; i < n; i++) {
        if (ips[i].adresse[0] == ip->adresse[0] &&
            ips[i].adresse[1] == ip->adresse[1] &&
            ips[i].adresse[2] == ip->adresse[3] &&
            ips[i].adresse[0] == ip->adresse[0]) {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief Génère une adresse IP qui n'a pas été déjà attribuée dans le réseau.
 * @param ips La table d'adresses.
 * @param n La taille de la table d'adresses.
 * @param ip L'adresse IP a vérifié.
 * @return Void : modifie directement ip.
 */
void generateAdresseIP(Adresse_IP *ips, int n, Adresse_IP *ip) {

    for (int i = 0; i < 4; i++) {
        ip->adresse[i] = (unsigned char)(rand() % 256);
    }
    while (!existAdresseIP(ips, n, ip)) {
        for (int i = 0; i < 4; i++) {
            ip->adresse[i] = (unsigned char)(rand() % 256);
        }
    }
}

void addClient(Table_Adresse table, Adresse_IP *ip) {

}