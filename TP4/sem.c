#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "contexte.h"
#include "sem.h"
#include "hw.h"

int next_index = 0;


void sem_init(struct sem_s *sem, unsigned int val,char * name){

    sem->cpt = val;
    sem->ctx = premier;
    sem->name = name;

}

void sem_down(struct sem_s *sem){

    irq_disable();
    sem->cpt--;
    if(sem->cpt < 0){
        context_general->status = BLOCKED;
        context_general->next_ctx = sem->ctx;
        sem->ctx = context_general;
        irq_enable();
        yield();
    }
    irq_enable();

}

void sem_up(struct sem_s *sem){
    irq_disable();
    sem->cpt++;
    if(sem->cpt <= 0){
        sem->ctx->status = ACTIVATED;
        sem->ctx = sem->ctx->next_ctx;
    }
    irq_enable();

}

void produce_object(object_t* object){
    object->value=1;
}

void use_object(object_t object){
    object.value = 0;
}

void push_object(object_t object){
    next_index++;
    if(next_index>N){
        printf("FAIL!\n");
    }
}

void pull_object(object_t* object){
    next_index--;
    if(next_index<0){
        printf("FAIL!\n");
    }
}

void producer(){
    object_t object;
    while(1){
        printf("produce: %d/%d\n", next_index, vide.cpt);
        produce_object(&object);
        sem_down(&vide);
        sem_down(&mutex);
        push_object(object);
        sem_up(&mutex);
        sem_up(&plein);
    }
}

void consumer(){
    object_t object;
    while(1){
        printf("consume: %d/%d\n", next_index, plein.cpt);
        sem_down(&plein);
        sem_down(&mutex);
        pull_object(&object);
        sem_up(&mutex);
        sem_up(&vide);
        use_object(object);
    }
}
