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
/*CODAGE HAMMING & ERREUR SERVEUR*/

/**
 * @brief Fonction pour générer une erreur aléatoire dans les données.
 * La fonction prend en paramètre un tableau d'entiers représentant les données
 * à modifier. Elle génère un nombre aléatoire entre 0 et 2 inclus, et modifie
 * une des valeurs du tableau en fonction de ce nombre. Si le nombre généré est
 * 1, la valeur correspondante dans le tableau est inversée. Si le nombre généré
 * est différent de 1, aucune modification n'est effectuée. La fonction utilise
 * le générateur de nombres aléatoires de la bibliothèque standard.
 *
 * @param data7 Le tableau d'entiers représentant les données à modifier.
 */
void errorServeur(int *data7) {
    srand(time(NULL)); // Initialisation du générateur de nombres aléatoires

    int i;
    int comp = 99;
    for (i = 0; i < 7; i++) {
        if (comp == 100) {
            int j = rand() % 3;
            printf("%d \n", j);
            if (data7[i] == 1 && j == 1) {
                data7[i] = 0;
            } else if (j == 1) {
                data7[i] = 1;
            }
            comp = -1;
        }
        comp++;
    }
}

/**
 * @brief Fonction pour effectuer le codage de Hamming (7,4).
 * La fonction prend en paramètre un tableau d'entiers représentant les données
 * à coder, et un tableau d'entiers pour stocker les données codées. La fonction
 * calcule les bits de parité et construit le mot de sortie en utilisant le
 * codage de Hamming (7,4).
 *
 * @param data4 Les données à coder.
 * @param encoded7 Le tableau pour stocker les données codées.
 */
void encodeHamming(const int *data4, int *encoded7) {
    // Calcul des bits de parité
    encoded7[2] = data4[0] ^ data4[1] ^ data4[3]; // P1
    encoded7[4] = data4[0] ^ data4[2] ^ data4[3]; // P2
    encoded7[5] = data4[1] ^ data4[2] ^ data4[3]; // P3

    // Construction du mot de sortie
    encoded7[0] = data4[0];
    encoded7[1] = data4[1];
    encoded7[3] = data4[2];
    encoded7[6] = data4[3];
}

/**
 * @brief Fonction pour effectuer le décodage de Hamming (7,4).
 * La fonction prend en paramètre un tableau d'entiers représentant les données
 * codées, et un tableau d'entiers pour stocker les données décodées. La
 * fonction calcule les bits de parité, détecte et corrige les erreurs
 * éventuelles, et construit les données décodées en utilisant le décodage de
 * Hamming (7,4).
 *
 * @param encoded7 Les données codées.
 * @param decoded4 Le tableau pour stocker les données décodées.
 */
void decodageHamming(const int *encoded7, int *decoded4) {
    // Copie des données codées dans un autre tableau
    int decoded7[7];
    for (int i = 0; i < 7; ++i) {
        decoded7[i] = encoded7[i];
    }

    // Calcul des bits de parité
    int p1 = decoded7[2];
    int p2 = decoded7[4];
    int p3 = decoded7[5];

    // Calcul de la position de l'erreur
    int erreur = p1 * 1 + p2 * 2 + p3 * 4;

    // Correction de l'erreur (si présente)
    if (erreur != 0) {
        printf("Erreur détectée à la position : %d\n", erreur);
        // Inversion du bit erroné
        decoded7[erreur - 1] = !decoded7[erreur - 1];
    }

    // Construction des données décodées
    decoded4[0] = decoded7[0];
    decoded4[1] = decoded7[1];
    decoded4[2] = decoded7[3];
    decoded4[3] = decoded7[6];
}

/*===================================================
FONCTIONS UTILISES & DIVERSES
===================================================*/

/**
 * @brief Convertit un entier en sa représentation binaire.
 *
 * @param num L'entier à convertir.
 * @return Un pointeur vers la représentation binaire de l'entier.
 */
void intToBinaire(int *binary, int num) {
    for (int i = 7; i >= 0; --i) {
        binary[i] = (num >> i) & 1;
    }
}

/**
 * Convertit un caractère en binaire.
 *
 * @param c Le caractère à convertir.
 * @return Un pointeur vers un tableau contenant la représentation binaire du
 * caractère.
 */
void charToBinaire(int *binary, char c) {
    for (int i = 7; i >= 0; --i) {
        binary[i] = (c >> i) & 1;
    }
}

/**
 * Concatène deux tableaux d'entiers en un seul tableau.
 *
 * @param tab1 Le premier tableau d'entiers.
 * @param taille1 La taille du premier tableau.
 * @param tab2 Le deuxième tableau d'entiers.
 * @param taille2 La taille du deuxième tableau.
 * @return Un pointeur vers le tableau concaténé.
 */
int *concatenateTableaux(const int *tab1, int taille1, const int *tab2,
                         int taille2) {
    int *concatene = (int *)malloc(
        (taille1 + taille2) *
        sizeof(int)); // Allouer de la mémoire pour le nouveau tableau

    // Copier les éléments du premier tableau dans le nouveau tableau
    for (int i = 0; i < taille1; ++i) {
        concatene[i] = tab1[i];
    }

    // Copier les éléments du deuxième tableau dans le nouveau tableau
    for (int i = 0; i < taille2; ++i) {
        concatene[taille1 + i] = tab2[i];
    }

    return concatene; // Retourner le tableau concaténé
}
/**
 * @brief Tue tous les processus avec un nom spécifique.
 *
 * Cette fonction recherche les processus avec le nom donné dans le
 * répertoire /proc et les tue en utilisant le signal SIGKILL. Elle itère à
 * travers tous les répertoires dans /proc et vérifie si le nom du processus
 * correspond au nom donné. Si une correspondance est trouvée, le processus
 * est tué.
 *
 * @param process_name Le nom du processus à tuer.
 * @return Void : tue directement les processus.
 */
void kill_processes_by_name(const char *process_name) {
    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir("/proc")) == NULL) {
        perror("opendir(/proc)");
        exit(EXIT_FAILURE);
    }

    struct stat statbuf;
    char proc_path[256];

    while ((entry = readdir(dir)) != NULL) {
        char *endptr;
        long pid = strtol(entry->d_name, &endptr, 10);

        if (*endptr != '\0') {
            continue; // Not a valid PID
        }

        snprintf(proc_path, sizeof(proc_path), "/proc/%ld", pid);
        if (stat(proc_path, &statbuf) == -1) {
            perror("stat");
            continue;
        }

        if (!S_ISDIR(statbuf.st_mode)) {
            continue; // Not a directory
        }

        FILE *cmdline_file;
        char cmdline_path[256];
        snprintf(cmdline_path, sizeof(cmdline_path), "/proc/%ld/cmdline", pid);
        if ((cmdline_file = fopen(cmdline_path, "r")) != NULL) {
            char cmdline[MAX_CMD_LENGTH];
            if (fgets(cmdline, sizeof(cmdline), cmdline_file)) {
                if (strstr(cmdline, process_name) != NULL) {
                    printf("Killing process %ld (%s)\n", pid, cmdline);
                    if (kill(pid, SIGKILL) == -1) {
                        perror("kill");
                    }
                }
            }
            fclose(cmdline_file);
        }
    }

    closedir(dir);
}