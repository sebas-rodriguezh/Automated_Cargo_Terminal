#ifndef CAMION_H
#define CAMION_H
 
#include <pthread.h>
#include "tcb_camion.h"


typedef enum
{
    NUEVO, LISTO, EJECUCION, BLOQUEADO, TERMINADO
} Estado;

typedef struct
{
    pthread_t thread_id; 
    Estado estado; 
    TCBCamion *params; 

} Camion;

void *ejecutar_camion(void *arg);


#endif