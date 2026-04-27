#ifndef PLANIFICADOR_H
#define PLANIFICADOR_H
 
#include <pthread.h>
#include "tcb_camion.h"

#define MAX_COLA 20

typedef enum {
    FIFO = 0, 
    RR = 1
} tipoAlgoritmo;

typedef struct Planificador
{
    tipoAlgoritmo tipo; 
    int quantum;
    TCBCamion *cola[MAX_COLA];
    int cantidad;
    pthread_mutex_t mutex_cola; 
    pthread_cond_t cond_turno;
    TCBCamion *camion_activo;

    void (*encolar)(struct Planificador *p, TCBCamion *c);
    TCBCamion *(*seleccionar)(struct Planificador *p);
    void (*post_ejecucion)(struct Planificador *p, TCBCamion *c);

} Planificador;

void inicializar_planificador(Planificador *p, tipoAlgoritmo tipo, int quantum);
void destruir_planificador(Planificador *p);


#endif