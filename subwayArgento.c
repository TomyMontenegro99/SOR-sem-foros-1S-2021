#include <stdio.h>      // libreria estandar
#include <stdlib.h>     // para usar exit y funciones de la libreria standard
#include <string.h>
#include <pthread.h>    // para usar threads
#include <semaphore.h>  // para usar semaforos
#include <unistd.h>


#define LIMITE 50


int tiempoEquipo1;
int tiempoEquipo2;
int tiempoEquipo3;
int tiempoEquipo4;

//creo estructura de semaforos 
struct semaforos {
    sem_t sem_mezclar;
	
	//poner demas semaforos aqui
	sem_t sem_salar;
	sem_t sem_agregar;
	sem_t sem_empanar;
};

//creo los pasos con los ingredientes
struct paso {
   char accion [LIMITE];
   char ingredientes[11][LIMITE];
   
};

//creo los parametros de los hilos 
struct parametro {
 int equipo_param;
 struct semaforos semaforos_param;
 struct paso pasos_param[8];
};

//funcion para imprimir las acciones y los ingredientes de la accion
void* imprimirAccion(void *data, char *accionIn) {
	struct parametro *mydata = data;
	//calculo la longitud del array de pasos 
	int sizeArray = (int)( sizeof(mydata->pasos_param) / sizeof(mydata->pasos_param[0]));
	//indice para recorrer array de pasos 
	int i;
	for(i = 0; i < sizeArray; i ++){
		//pregunto si la accion del array es igual a la pasada por parametro (si es igual la funcion strcmp devuelve cero)
		if(strcmp(mydata->pasos_param[i].accion, accionIn) == 0){
		printf("\tEquipo %d - accion %s \n " , mydata->equipo_param, mydata->pasos_param[i].accion);
		//calculo la longitud del array de ingredientes
		int sizeArrayIngredientes = (int)( sizeof(mydata->pasos_param[i].ingredientes) / sizeof(mydata->pasos_param[i].ingredientes[0]) );
		//indice para recorrer array de ingredientes
		int h;
		printf("\tEquipo %d -----------ingredientes : ----------\n",mydata->equipo_param); 
			for(h = 0; h < sizeArrayIngredientes; h++) {
				//consulto si la posicion tiene valor porque no se cuantos ingredientes tengo por accion 
				if(strlen(mydata->pasos_param[i].ingredientes[h]) != 0) {
							printf("\tEquipo %d ingrediente  %d : %s \n",mydata->equipo_param,h,mydata->pasos_param[i].ingredientes[h]);
				}
			}
		}
	}
}

//funcion para tomar de ejemplo
void* cortar(void *data) {
	//creo el nombre de la accion de la funcion 
	char *accion = "cortar";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	usleep( 20000 );
	//doy la se??al a la siguiente accion (cortar me habilita mezclar)
    sem_post(&mydata->semaforos_param.sem_mezclar);
	
    pthread_exit(NULL);
}

void* mezclar (void *data){
	char *accion = "mezclar";
	struct parametro *mydata = data;
	imprimirAccion(mydata,accion);
	usleep( 2000 );
	sem_post(&mydata->semaforos_param.sem_salar);

	pthread_exit(NULL);
}

void* salar (void* data){
	char *accion = "salar";
	struct parametro *mydata = data;
	imprimirAccion(mydata,accion);
	usleep( 2000 );
	sem_post(&mydata->semaforos_param.sem_agregar);

	pthread_exit(NULL);
}

void* agregar (void* data){
	char *accion = "agregar";
	struct parametro *mydata = data;
	imprimirAccion (mydata,accion);
	usleep( 2000 );
	sem_post(&mydata->semaforos_param.sem_empanar);
}

void* empanar (void* data){
	char *accion = "empanar";
	struct parametro *mydata = data;
	imprimirAccion (mydata,accion);
	usleep( 2000 );
	pthread_exit(NULL);
}



void* ejecutarReceta(void *i) {
	
	//variables semaforos
	sem_t sem_mezclar;
	
	//crear variables semaforos aqui
	sem_t sem_cortar;
	sem_t sem_salar;
	sem_t sem_agregar;
	sem_t sem_empanar;
	
	//variables hilos
	pthread_t p1; 
	//crear variables hilos aqui
	pthread_t p2;
	pthread_t p3;
	pthread_t p4;
	
	//numero del equipo (casteo el puntero a un int)
	int p = *((int *) i);
	
	printf("Ejecutando equipo %d \n", p);

	//reservo memoria para el struct
	struct parametro *pthread_data = malloc(sizeof(struct parametro));

	//seteo los valores al struct
	
	
	//seteo numero de grupo
	pthread_data->equipo_param = p;

	//seteo semaforos
	pthread_data->semaforos_param.sem_mezclar = sem_mezclar;
	//setear demas semaforos al struct aqui
	pthread_data->semaforos_param.sem_salar = sem_salar;
	pthread_data->semaforos_param.sem_agregar = sem_agregar;
	pthread_data->semaforos_param.sem_mezclar = sem_mezclar;
	pthread_data->semaforos_param.sem_empanar = sem_empanar;
	pthread_data->semaforos_param.sem_cortar = sem_cortar;
	

	//seteo las acciones y los ingredientes 
     	strcpy(pthread_data->pasos_param[0].accion, "cortar");
	    strcpy(pthread_data->pasos_param[0].ingredientes[0], "ajo");
        strcpy(pthread_data->pasos_param[0].ingredientes[1], "perejil");


	    strcpy(pthread_data->pasos_param[1].accion, "mezclar");
	    strcpy(pthread_data->pasos_param[1].ingredientes[0], "ajo");
        strcpy(pthread_data->pasos_param[1].ingredientes[1], "perejil");
 	    strcpy(pthread_data->pasos_param[1].ingredientes[2], "huevo");
	    strcpy(pthread_data->pasos_param[1].ingredientes[3], "carne");

		strcpy(pthread_data->pasos_param[2].accion, "salar");
		strcpy(pthread_data->pasos_param[2].ingredientes[10], "sal"); 

		strcpy(pthread_data->pasos_param[3].accion, "agregar");
		strcpy(pthread_data->pasos_param[3].ingredientes[3], "carne");

		strcpy(pthread_data->pasos_param[4].accion, "cocinar");
		strcpy(pthread_data->pasos_param[4].ingredientes[4], "pan");

		strcpy(pthread_data->pasos_param[5].accion, "empanar");
		strcpy(pthread_data->pasos_param[5].ingredientes[5], "milanesas");

		strcpy(pthread_data->pasos_param[4].accion, "cocinar");
		strcpy(pthread_data->pasos_param[4].ingredientes[5], "milanesas");

		strcpy(pthread_data->pasos_param[0].accion, "cortar");
		strcpy(pthread_data->pasos_param[0].ingredientes[6], "lechuga fresca");
		strcpy(pthread_data->pasos_param[0].ingredientes[7], "tomate");
		strcpy(pthread_data->pasos_param[0].ingredientes[8], "cebolla morada");
		strcpy(pthread_data->pasos_param[0].ingredientes[9], "pepino");







		




		
        
	
	
	//inicializo los semaforos

	sem_init(&(pthread_data->semaforos_param.sem_mezclar),0,0);
	

	
	//inicializar demas semaforos aqui

	sem_init(&(pthread_data->semaforos_param.sem_agregar),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_salar),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_empanar),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_cortar,0,0);


	//creo los hilos a todos les paso el struct creado (el mismo a todos los hilos) ya que todos comparten los semaforos 
    int rc;
    rc = pthread_create(&p1,                           //identificador unico
                            NULL,                          //atributos del thread
                                cortar,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia
	
	
	//crear demas hilos aqui
	int rc2;
	int rc3;
	int rc4;
	int rc5;
	rc2 = pthread_create(&p2,NULL,mezclar,pthread_data);
	rc3 = pthread_create(&p3,NULL,agregar,pthread_data);
	rc4 = pthread_create(&p4,NULL,empanar,pthread_data);
	rc5 = pthread_create(&p5,NULL,salar,pthread_data);
	
	
	//join de todos los hilos
	pthread_join (p1,NULL);
	//crear join de demas hilos

	pthread_join(p2,NULL);
	pthread_join(p3,NULL);
	pthread_join(p4,NULL);

	//valido que el hilo se alla creado bien 
    if (rc){
       printf("Error:unable to create thread, %d \n", rc);
       exit(-1);
     }

	//destruccion de los semaforos 
	sem_destroy(&sem_mezclar);
	//destruir demas semaforos 
	sem_destroy(&sem_salar);
	sem_destroy(&sem_agregar);
	sem_destroy(&sem_empanar);
	sem_destroy(&sem_cortar);
	
	//salida del hilo
	 pthread_exit(NULL);
}


int main ()
{	

	

	//creo los nombres de los equipos 
	int rc;
	int *equipoNombre1 =malloc(sizeof(*equipoNombre1));
	int *equipoNombre2 =malloc(sizeof(*equipoNombre2));
	int *equipoNombre3 =malloc(sizeof(*equipoNombre3));
	int *equipoNombre4 =malloc(sizeof(*equipoNombre4));

  
	*equipoNombre1 = 1;
	*equipoNombre2 = 2;
	*equipoNombre3 = 3;
	*equipoNombre4 = 4;

	//creo las variables los hilos de los equipos
	pthread_t equipo1; 
	pthread_t equipo2;
	pthread_t equipo3;
	pthread_t equipo4;

  
	//inicializo los hilos de los equipos
    rc = pthread_create(&equipo1,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,             //funcion a ejecutar
                                equipoNombre1); 

    rc = pthread_create(&equipo2,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,             //funcion a ejecutar
                                equipoNombre2);
	rc = pthread_create(&equipo3,NULL,ejecutarReceta,equipoNombre3);
	rc = pthread_create(&equipo4,NULL,ejecutarReceta,equipoNombre4);

  


   if (rc){
       printf("Error:unable to create thread, %d \n", rc);
       exit(-1);
     } 

	//join de todos los hilos
	pthread_join (equipo1,NULL);
	pthread_join (equipo2,NULL);
	pthread_join (equipo3,NULL);
	pthread_join (equipo4,NULL);



    pthread_exit(NULL);
}


//Para compilar:   gcc subwayArgento.c -o ejecutable -lpthread
//Para ejecutar:   ./ejecutable
