#include "../header/types.h"
#ifndef _FONCTIONS_C
#define _FONCTIONS_C

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

int existAdresseIP(Table_Adresse table, Adresse_IP *ip);
void generateAdresseIP(Table_Adresse table, Adresse_IP *ip);
int addClient(Table_Adresse *table);
void suppClient(Table_Adresse *table, Adresse_IP *ip);

#endif // !_FONCTIONS_C
