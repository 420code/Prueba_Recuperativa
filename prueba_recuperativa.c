prueba_recuperativa.c
#include<stdio.h>
#include<unistd.h>
#include<semaphore.h>
#include<pthread.h>
#include<stdlib.h>

#define Cantidad_Cajas 4;

pthread_t hOfficial;
pthread_t hClient[33];

typedef struct attention{
     
     pthread_t client;
     int name;
     int condition;
}box;

typedef struct affiliate{
    pthread_t id;
    int condition;
}client;


int position=0;
int fullBx=0;
int ClientAttention;
int repeat;


client row[36];
box box[6];



sem_t semaEntrance;
sem_t semaRow;



void *cajaDisponible(void* arg){
    
    int c=0;
    suma=0
    fullBx=0;
    

    long id = (long) arg;
    int pos = (int) id;
    
    
    sem_wait(&semaIngreso);
    for(c=0;c<6;c++){
        if(row[pos].condition==1){
            if(box[c].condition==1){
                printf("Funcionario: La caja numero: %d está ocupada.\n",c);
                fullBx++;
            }else{
                printf("Funcionario: La caja numero %d está disponible\n",c);
                box[c].condition = 1;
                box[c].client = row[pos].id;
                printf("El cliente n: %d ha ingresado a la caja %d.\n",pos,c);
                sleep(6);
                printf("El cliente n:%d ha dejado la caja %d\n",pos,c);
                ClientAttention--;
                box[c].condition = 0;
                row[pos].condition = 2;
                break;
            }
        }
    }
    if(fullBx==6){
        printf("Funcionario: Cliente agregado a la fila.\n");
    }

    sem_post(&semaEntrance);
}

void *solicitarCaja(){
	pthread_t tid;
    int oo;
	tid = pthread_self();
    sem_wait(&semaRow);
    for(oo=0;oo<repeat;oo++){
        if(row[oo].condition==0){
            row[oo].condition=1;
            row[oo].id=tid;
            break;
        }
    }
	printf("Ha ingresado un cliente. posicion en la fila: (%d/%d). ID: %ld\n\n",oo,oo,tid);
    position++;
    sem_post(&semaRow);
}


int main(){
    long i;
    sem_init(&semaRow,0,1);
    sem_init(&semaEntrance, 0, 4);
    int confirmation;
    for(int c = 0; c<4;c++){
       box[c].name = c;
       box[c].condition = 0;
    }
    for(int c = 0; c<20; c++){
        row[c].condition=0;
        row[c].id=0;
    }
    printf("¿Cuantos clientes ingresará? Maximo 36\n");
    scanf("%d",&repeat);
    clientAtenttion = repeat;
    for(i=0;i<repeat;i++){
        confirmation = pthread_create(&hClient[i], NULL, &solicitar_Caja, NULL);
		if(confirmation != 0){
			printf("Error n: %d, al lanzar el hilo\n", confirmation);
        }
    }
    for(i=0;i<repeat;i++){
		pthread_join(hClient[i], NULL);
    }
    do{
        for(i=0;i<repeat;i++){
            confirmation = pthread_create(&hOfficial, NULL, &caja_Disponible, (void*)i);
            if(confirmation != 0){
	            printf("Error n: %d, al lanzar el hilo\n", confirmation);
                exit(-1);
            }
        }
        for(i=0;i<repeat;i++){
            pthread_join(hOfficial, NULL);
        }
    }while(clientAtenttion>1);
    return 0;
}