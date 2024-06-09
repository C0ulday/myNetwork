#include "../header/types.h"

#ifndef _FONCTIONS_H
#define _FONCTIONS_H

/*===================================================
FONCTIONS ADMIN
===================================================*/
/*MENU*/
#define MAX_MENU_ITEMS 10
#define MENU_ITEM_LENGTH 50
#define SEPARATOR_LENGTH 50

void print_menu(const char *title, const char items[][MENU_ITEM_LENGTH],
                int item_count);

/*===================================================
FONCTIONS SERVEUR
===================================================*/

/**
 * @brief Initialise la table des clients avec un nombre maximum de clients
 * donné.
 *
 * @param table Un pointeur vers la table des adresses.
 * @param nombre_clients_max Le nombre maximum de clients.
 */
void initializeTableClients(Table_Adresse *table, int nombre_clients_max);

/**
 * @brief Met à zéro les informations d'un client à l'index donné dans la table
 * des adresses.
 *
 * @param table La table des adresses.
 * @param index_client L'index du client à mettre à zéro.
 * @return 1 si le client a été mis à zéro avec succès, sinon 0.
 */
int nullClient(Table_Adresse table, int index_client);

/**
 * @brief Affiche les informations d'un client à l'index donné dans la table des
 * adresses.
 *
 * @param table La table des adresses.
 */
void printClient(Table_Adresse table);

/**
 * @brief Vérifie si une adresse IP existe dans la table des adresses.
 *
 * @param table La table des adresses.
 * @param ip L'adresse IP à vérifier.
 * @return 1 si l'adresse IP existe dans la table, sinon 0.
 */
int existAdresseIP(Table_Adresse table, Adresse_IP ip);

/**
 * @brief Génère une adresse IP unique et l'assigne à un client dans la table
 * des adresses.
 *
 * @param table La table des adresses.
 * @param ip Un pointeur vers l'adresse IP générée.
 */
void generateAdresseIP(Table_Adresse table, Adresse_IP *ip);

/**
 * @brief Ajoute un nouveau client à la table des adresses.
 *
 * @param table Un pointeur vers la table des adresses.
 * @return 1 si le client a été ajouté avec succès, sinon 0.
 */
int addClient(Table_Adresse *table);

/**
 * @brief Supprime un client de la table des adresses en utilisant son adresse
 * IP.
 *
 * @param table Un pointeur vers la table des adresses.
 * @param ip Un pointeur vers l'adresse IP du client à supprimer.
 */
void suppClient(Table_Adresse *table, Adresse_IP *ip);
/**
 * Initialise le dictionnaire avec la taille spécifiée.
 *
 * @param dico Le dictionnaire à initialiser.
 * @param taille La taille du dictionnaire.
 */
void initialiserDico(Dico *dico, int taille);

/**
 * Ajoute un mot au dictionnaire à l'index spécifié.
 *
 * @param dico Le dictionnaire auquel ajouter le mot.
 * @param index L'index auquel ajouter le mot.
 * @param mot Le mot à ajouter au dictionnaire.
 */
void ajouterDansDico(Dico *dico, int index, char *mot);

/**
 * Vérifie si un élément existe dans le dictionnaire.
 *
 * @param dico Le dictionnaire dans lequel effectuer la recherche.
 * @param element L'élément à rechercher.
 * @return 1 si l'élément est trouvé, 0 sinon.
 */
int DansDico(Dico *dico, char *element);

/**
 * Recherche l'index d'un mot dans le dictionnaire.
 *
 * @param dico Le dictionnaire dans lequel effectuer la recherche.
 * @param mot Le mot à rechercher.
 * @return L'index du mot dans le dictionnaire, ou -1 s'il n'est pas trouvé.
 */
int rechercheIndexDansDico(Dico *dico, char *mot);

/**
 * Compresse le message donné en utilisant l'algorithme LZ78.
 *
 * @param message Le message à compresser.
 * @return La sortie compressée.
 */
Output *LZ78(char *message);
#endif // !_FONCTIONS_H
