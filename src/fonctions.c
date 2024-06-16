#include "../header/fonctions.h"
#include "../header/types.h"

#include <dirent.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

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

    Adresse_IP ip = table.clients[index_client].adresseIP;
    if (ip.adresse[0] == 0 && ip.adresse[1] == 0 && ip.adresse[2] == 0 &&
        ip.adresse[3] == 0) {
        return 1; // Client avec une adresse IP nulle trouvé
    }
    return -1;
}

/**
 * @brief Affiche les clients d'adresse IP non nulle.
 * @param table Pointeur vers tableau des clients.
 * @return Void : affiche directement les clients d'adresse IP non nulle.
 */
void printClient(Table_Adresse table) {

    printf("LISTE DES CLIENTS\n");
    printf("Nombre de clients : %d\n", table.nombre_clients);
    for (int i = 0; i < NOMBRE_CLIENTS_MAX; i++) {
        if (nullClient(table, i) == -1) {

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
/**
 * @brief Initialise le dictionnaire avec une taille donnée.
 *
 * @param dico Le pointeur vers le dictionnaire à initialiser.
 * @param taille La taille du dictionnaire.
 */
void initialiserDico(Dico *dico, int taille) {

    dico->cellule = malloc(taille * sizeof(Cellule));
    if (dico->cellule == NULL) {
        printf("Erreur lors de l'allocation mémoire.\n");
        exit(EXIT_FAILURE); // Quitter le programme en cas d'échec d'allocation
    }
    dico->nbCellules = 0; // Initialiser le nombre de cellules à 0
    dico->cellule[0].index = -1;
}

/**
 * @brief Ajoute un mot au dictionnaire.
 *
 * Cette fonction ajoute un mot au dictionnaire en assignant l'index et le mot
 * donnés à la prochaine cellule disponible dans le dictionnaire. Si le
 * dictionnaire est déjà plein, elle affiche un message indiquant que le
 * dictionnaire est plein.
 *
 * @param dico Pointeur vers la structure du dictionnaire.
 * @param index L'index du mot.
 * @param mot Le mot à ajouter.
 */
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

/**
 * Vérifie si un élément est présent dans le dictionnaire.
 *
 * @param dico Le dictionnaire à vérifier.
 * @param element L'élément à rechercher dans le dictionnaire.
 * @return 1 si l'élément est présent dans le dictionnaire, 0 sinon.
 */
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

/**
 * Recherche l'index d'un mot donné dans le dictionnaire.
 *
 * @param dico Le dictionnaire dans lequel effectuer la recherche.
 * @param mot Le mot à rechercher.
 * @return L'index du mot dans le dictionnaire, ou 0 s'il n'est pas trouvé.
 */
int rechercheIndexDansDico(Dico *dico, char *mot) {

    for (int i = 0; i < dico->nbCellules; i++) {
        if (strcmp(mot, dico->cellule[i].mot) == 0) {
            return dico->cellule[i].index;
        }
    }
    return 0;
}

/*Dans un dico, il faut tenir compte du dernier élément pour marquer la fin
: si j'ai 3 cellules, j'en initialise 4 pour le token de fin
*/
/**
 * Fonction qui implémente l'algorithme LZ78 pour compresser un message.
 *
 * @param message Le message à compresser.
 * @return Un pointeur vers une structure Output contenant le dictionnaire
 * et les blocs compressés.
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

        /*Si la chaîne est déjà dans le dico, j'avance d'un caractère
        jusqu'à obtenir un mot qui n'est pas encore dans le dico*/

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
/*CODAGE HAMMING & ERREUR SERVEUR*/

/**
 * @brief Fonction pour générer une erreur aléatoire dans les données.
 * La fonction prend en paramètre un tableau d'entiers représentant les
 * données à modifier. Elle génère un nombre aléatoire entre 0 et 2 inclus,
 * et modifie une des valeurs du tableau en fonction de ce nombre. Si le
 * nombre généré est 1, la valeur correspondante dans le tableau est
 * inversée. Si le nombre généré est différent de 1, aucune modification
 * n'est effectuée. La fonction utilise le générateur de nombres aléatoires
 * de la bibliothèque standard.
 *
 * @param data Le tableau d'entiers représentant les données à modifier.
 */
void errorServeur(int *data) {
    srand(time(NULL)); // Initialisation du générateur de nombres aléatoires

    int i;
    int comp = 99;
    for (i = 0; i < 7; i++) {
        if (comp == 100) {
            int j = rand() % 3;
            if (data[i] == 1 && j == 1) {
                data[i] = 0;
            } else if (j == 1) {
                data[i] = 1;
            }
            comp = -1;
        }
        comp++;
    }
}

/**
 * @brief Fonction pour effectuer le codage de Hamming (7,4).
 * La fonction prend en paramètre un tableau d'entiers représentant les
 * données à coder, et un tableau d'entiers pour stocker les données codées.
 * La fonction calcule les bits de parité et construit le mot de sortie en
 * utilisant le codage de Hamming (7,4).
 *
 * @param data Les données à coder sur 4 bits
 * @param hamming Le tableau pour stocker les données codées.
 */
void encodeHamming8to12(int *data, int *hamming) {
    // Initialisation
    for (int i = 0; i < 12; i++) {
        hamming[i] = 0;
    }

    // Bits de données
    hamming[2] = data[0];  // D1
    hamming[4] = data[1];  // D2
    hamming[5] = data[2];  // D3
    hamming[6] = data[3];  // D4
    hamming[8] = data[4];  // D5
    hamming[9] = data[5];  // D6
    hamming[10] = data[6]; // D7
    hamming[11] = data[7]; // D8

    // Bits de parité
    hamming[0] =
        hamming[2] ^ hamming[4] ^ hamming[6] ^ hamming[8] ^ hamming[10];
    hamming[1] =
        hamming[2] ^ hamming[5] ^ hamming[6] ^ hamming[9] ^ hamming[10];
    hamming[3] = hamming[4] ^ hamming[5] ^ hamming[6] ^ hamming[11];
    hamming[7] = hamming[8] ^ hamming[9] ^ hamming[10] ^ hamming[11];
}

/**
 * @brief Décode un code de Hamming (12, 8) et corrige les erreurs si elles
 * sont détectées.
 *
 * Cette fonction prend un tableau de bits de code de Hamming et le décode
 * pour extraire les 8 bits de données d'origine. Elle vérifie également les
 * erreurs dans le code et les corrige si elles sont détectées.
 *
 * @param hamming Le tableau de bits de code de Hamming (12 bits).
 * @param data Le tableau pour stocker les 8 bits de données décodées.
 */
void decodeHamming12to8(int *hamming, int *data) {
    int errorPosition = 0;

    // Calculer les bits de parité
    int p1 = hamming[0] ^ hamming[2] ^ hamming[4] ^ hamming[6] ^ hamming[8] ^
             hamming[10];
    int p2 = hamming[1] ^ hamming[2] ^ hamming[5] ^ hamming[6] ^ hamming[9] ^
             hamming[10];
    int p4 = hamming[3] ^ hamming[4] ^ hamming[5] ^ hamming[6] ^ hamming[11];
    int p8 = hamming[7] ^ hamming[8] ^ hamming[9] ^ hamming[10] ^ hamming[11];

    // Calculer la position de l'erreur
    errorPosition = (p8 << 3) | (p4 << 2) | (p2 << 1) | p1;

    // Si errorPosition n'est pas 0, il y a une erreur à cette position
    if (errorPosition != 0) {
        printf("Error detected at position: %d\n", errorPosition);
        hamming[errorPosition - 1] ^=
            1; // Corriger l'erreur en inversant le bit
    } else {
        printf("No error detected.\n");
    }

    // Extraire les bits de données
    data[0] = hamming[2];
    data[1] = hamming[4];
    data[2] = hamming[5];
    data[3] = hamming[6];
    data[4] = hamming[8];
    data[5] = hamming[9];
    data[6] = hamming[10];
    data[7] = hamming[11];
}

/*===================================================
FONCTIONS UTILISES & DIVERSES
===================================================*/

/**
 * @brief Convertit un caractère en binaire.
 *
 * @param c Le caractère à convertir.
 * @return Un pointeur vers un tableau contenant la représentation binaire
 * du caractère.
 */
void charToBinaire(int *bin, char c) {
    for (int i = 7; i >= 0; --i) {
        bin[7 - i] = (c >> i) & 1;
    }
}

/**
 * @brief Convertit un entier en sa représentation binaire.
 *
 * @param num L'entier à convertir.
 * @return Un pointeur vers la représentation binaire de l'entier.
 */
void intToBinaire(int *bin, int num) {
    for (int i = 0; i < 8; ++i) {
        bin[i] = (num >> (7 - i)) & 1;
    }
}

/**
 * Convertit un tableau binaire en un caractère.
 *
 * @param bin Le tableau binaire à convertir.
 * @return Le caractère correspondant à la conversion.
 */
char binToChar(int *bin) {
    int resultat = 0;
    for (int i = 0; i < 8; ++i) {
        resultat = (resultat << 1) | bin[i];
    }
    return (char)resultat;
}

/**
 * Convertit un tableau binaire en un entier non signé.
 *
 * @param bin Le tableau binaire à convertir.
 * @return La valeur entière convertie.
 */
unsigned int binToEntier(int *bin) {
    unsigned int valeur = 0;
    for (int i = 0; i < 8; i++) {
        valeur = (valeur << 1) | bin[i];
    }
    return valeur;
}

/**
 * @brief Récupère un sous-tableau à partir du tableau donné.
 *
 * Cette fonction récupère un sous-tableau à partir du tableau donné en
 * fonction des indices de début et de fin spécifiés. L'indice de début doit
 * être supérieur ou égal à 0 et inférieur à l'indice de fin.
 *
 * @param tableau Le tableau d'origine.
 * @param debut L'indice de début du sous-tableau.
 * @param fin L'indice de fin du sous-tableau.
 * @return Un sous-tableau alloué dynamiquement contenant les éléments du
 * tableau d'origine. Renvoie NULL si les indices de début et de fin sont
 * invalides ou si l'allocation de mémoire échoue.
 */
int *getSousTableau(int *tableau, int debut, int fin) {
    // Vérifier les bornes
    if (debut < 0 || debut >= fin) {
        printf("Indices de début et fin incorrects.\n");
        return NULL;
    }

    // Calculer la taille du sous-tableau
    int taille = fin - debut + 1;

    // Allouer de la mémoire pour le sous-tableau
    int *sousTableau = (int *)malloc(taille * sizeof(int));
    if (sousTableau == NULL) {
        printf("Erreur d'allocation de mémoire.\n");
        return NULL;
    }

    // Copier les éléments du tableau original vers le sous-tableau
    for (int i = debut; i <= fin; ++i) {
        sousTableau[i - debut] = tableau[i];
    }

    return sousTableau;
}