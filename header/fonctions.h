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

#endif // !_FONCTIONS_C
