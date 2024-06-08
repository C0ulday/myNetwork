#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "../header/fonctions.h"
#include "../header/types.h"

/**
 * @file serveur.c
 * @brief Serveur du réseau
 * Serveur permet :
 * - Ce programme recevra des trames des programmes clients et les enverra au programme client correspondant.Il ne modifiera pas la trame.
 * - En cas de destinataire inconnu ou injoignable, le serveur enverra un message spécifique au client.
 * - Le serveur est chargé de vérifier régulièrement si les clients sont “vivants” en cas de non-réponse, le serveur ne redirige plus les trames vers ce client et prévient les autres clients.
 * - En cas de nouveau client le serveur lui alloue une IP et prévient l'apparition aux autres clients.
*/


int main(void)
{

    /*===================================================
    INITIALISATION MENU
    ===================================================*/
    printf("SERVEUR - Menu Principal"\n);
    printf("--------------------------------------------------"\n);

    /*===================================================
    CHOIX UTILISATEUR MENU
    ===================================================*/

    key_t cle_admin = 1;
    int file_id  msgget(cle_admin,IPC_CREAT |0666);
    Table_Adresse table_adresse;

    if(file_id ==-1) {
        perror("msgget");
        exit(EXIT_FAILURE); 
    }

    while (1) {
        
        /*AJOUT CLIENT - ATTRIBUTION ADRESSE IP*/
        // Type 2 = ajout de client
        if(msgrcv(file_id,&table_adresse,2,0) == -1) {
            perror("msgrcv");
        }

    }

    return 0;
}