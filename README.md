NOTA: Este README.md solo contiene el texto de la documentación. La documentación completa con formatos, imágenes y texto se encuentra en el archivo PDF: Documentacion_P1_SO 


SISTEMA DE GESTIÓN DE TERMINAL DE CARGA AUTOMATIZADA

Primer proyecto programado

Fecha de entrega:
03/05/2026

Profesor: 
José Calvo Suárez

Asignatura: 
Sistemas Operativos

Integrantes: 
Sebastián Rodríguez Hernández
Jesús Barboza Ríos
José Daniel Garita Guzmán


Documentación interbloqueo y condición de carrera

El sistema fue hecho para ejecutar múltiples hilos (camiones), evitando problemas como los interbloqueos (deadlocks) y las condiciones de carrera. Para lograrlo, se implementaron mecanismos de sincronización y una estructura que minimiza los riesgos asociados al acceso a recursos compartidos de manera descontrolada. 
En primer lugar, el acceso a los muelles de carga lo hicimos mediante un semáforo, el cual define la cantidad de muelles disponibles. Cada camión, antes de acceder a un muelle, verifica si existe disponibilidad. En caso de que no haya muelles libres, el hilo entra en espera de forma controlada mediante sem_wait(), sin bloquear otros recursos del sistema. Esto hace que otros hilos sigan ejecutándose normalmente, evitando que un camión acapare un recurso innecesariamente.
 
Además, se emplean mutex para proteger recursos compartidos como la cola del planificador, el sistema de logs y el registro de métricas. Estos mutex nos ayudan que solo un hilo a la vez pueda acceder a estos recursos, evitando problemas con los datos y evitando as condiciones de carrera. Por ejemplo, al escribir en el log o las métricas, el acceso se realiza en una sección crítica protegida, asegurando que la información no se corrompa.
 
Una de las cosas más importantes del sistema es que evita explícitamente la condición de retención y espera, una de las condiciones necesarias para que ocurra un deadlock. En este sistema, cada camión solicita un único recurso a la vez, ósea, nunca mantiene un recurso mientras espera por otro. Esto elimina la posibilidad de que dos hilos necesiten mutuamente de sus recursos, los cuales cada hilo tiene bloqueado.
Un ejemplo de esta situación ocurre en el acceso a la cola del planificador. Cuando un camión revisa si es su turno, lo hace dentro de una sección protegida por mutex. Sin embargo, antes de intentar tomar un muelle (operación que puede bloquearse), el hilo libera el mutex. Este detalle es fundamental, ya que evita que un hilo quede bloqueado mientras aún tiene un recurso que otros hilos necesitan. Si no se hace esto, se podría producir un interbloqueo donde otros camiones no podrían acceder a la cola ni avanzar en su ejecución.
Otro método usado es el de pthread_cond_wait(), este sirve para que un hilo espere su turno para que libere automáticamente el mutex mientras este hilo está en espera. Esto garantiza que otros hilos puedan seguir accediendo a la cola del planificador sin interrupciones, manteniendo el sistema libre de deadlock.
 
Asimismo, se definió un orden en la liberación de recursos. Cuando un camión termina de usar un muelle, primero libera el recurso mediante sem_post() y luego entra a la cola del planificador. Con este orden se evitan situaciones en las que el hilo este usando múltiples recursos al mismo tiempo, lo cual podría llegar a generar bloqueos en ciertas situaciones.
Por otro lado, las condiciones de carrera se evitaron mediante el uso de mutex en todas las estructuras que son compartidas. La cola del planificador, por ejemplo, es accedida por múltiples hilos para encolar, desencolar o seleccionar camiones. Todas estas operaciones se realizan dentro de secciones controladas por mutex, garantizando la integridad de los datos. Así también como el sistema de logs y el gestor de métricas que están protegidos para evitar accesos simultáneos aleatorios.
Finalmente, el sistema se asegura de que cuando un recurso se libera, los hilos en espera sean notificados (por ejemplo, con pthread_cond_broadcast(); ) para continuar con su ejecución. Así se logra mantener un flujo constante y ordenado en el sistema, evitando bloqueos y mejorando la eficiencia del sistema. 
 
En conclusión, mediante el uso de semáforos, mutex y variables de condición, evitando la retención de recursos, y así se logró construir un sistema que evita interbloqueos y condiciones de carrera.
Paso a paso de cómo se evita el interbloqueo cuando se ejecuta un camión.
El camión adquiere el mutex_cola, el camión necesita revisar si es su turno para poder usar el muelle, para hacerlo sin que haya una condición de carrera, necesita asegurarse de tomar el mutex mencionado anteriormente. 
 
El camión se da cuenta que no es turno, ya que no cumple con la condición del while, debido a la variable de condición, el camión se queda dormido y permite soltar el mutex de la cola, para no irse a dormir con el mutex que otros camiones pueden requerir para operar. 
 
Una vez el camión sale del while, suelta el mutex inmediatamente, y lo hace justamente antes de intentar tomar el muelle. Al hacer esto, se rompe la condición de retención y espera. En el caso de que no se libere el mutex de la cola, el camión podría llegar a tomar el muelle y quedarse bloqueado con el mutex de la cola, generando que ningún camión pueda realizar operaciones sobre la cola del planificador, además, ningún camión podría salir de un muelle porque la cola seguiría bloqueada. 
 
Ahora, el camión puede entrar a tomar el muelle, pero habiendo soltado cualquier mutex que tuviera. Generando que en caso de tener que bloquearse no afecte a la operación de los demás camiones. 
 
Una vez el camión termine su Quantum o su carga completa, debe de soltar el muelle y avisarle al sistema para que el ciclo continúe. Ya cuando se haya liberado el muelle, el camión vuelve a pedir el acceso al mutex_cola para actualizar su estado con el post_ejecución, internamente utiliza el pthread_cond_broadcast para despertar a los camiones en espera. 
 
Conclusión
Este proyecto nos permitió adentrarnos dentro del mundo de los hilos, y de cómo estos de manera gestionada y controlada, permiten el funcionamiento correcto de una terminal que simula una situación real en donde diferentes hilos interactúan entre sí, e incluso cooperan para que la terminal fluya de manera idónea. Identificar secciones críticas y sobre todo flujos que podrían generar interbloqueos es fundamental para diseñar de manera tal que ningún camión pueda experimentar condiciones que afecten a los demás camiones. En nuestro caso, el proyecto se diseñó siguiendo un flujo que permite que todos los camiones sigan el mismo camino de ejecución, y que cada uno de ellos, deba de respetar los mutex, variables de condición y los semáforos. En nuestro caso, cuando se ejecutaba un camión, cada camión debía esperar una variable de condición que indicara si dicho TCB era el activo en la cola del planificador para poder correr en el muelle. Al hacerlo, el cond_wait obliga a que el camión deba de retomar el mutex que soltó, eso simboliza un conflicto a nivel de ejecución, ya que si este trata de tomar un muelle, y no hay disponibles, se irá a dormir con el mutex de la cola, haciendo que ningún camión pueda encolarse o el camión que recién termina su ejecución tome la cola para re-encolarse o simplemente liberar un espacio en el arreglo del planificador, lo que paralizaría el sistema, este interbloqueo haría que nadie más pueda interactuar, ni ejecutar unidades de ejecución. Este proyecto, nos permitió utilizar diferentes estrategias para lidiar con diferentes conflictos, usamos los mutex para proteger secciones críticas, las variables de condición para que un camión pudiese ejecutarse y escribir en el log solo cuando fuese el camión activo del planificador, y los semáforos para el control de flujo, permitiéndonos que n camiones puedan acceder a n muelles según la indicación de los parámetros que se dictan cuando se ejecuta el proyecto. 



