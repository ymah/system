#define N 100                         /* nombre de places dans le tampon */
#define SEM_MAGIC 0xC0FEBABE
#ifndef SEM_H
#define SEM_H

typedef void *objet_t;

struct sem_s
{
    char* name;
    int cpt;
    ctx_t ctx;
};




typedef struct object_s{
    int value;} object_t;

void sem_init(struct sem_s *sem, unsigned int val,char * name);
void sem_down(struct sem_s *sem);
void sem_up(struct sem_s *sem);

#endif


struct sem_s mutex, vide, plein;

void produce_object(object_t* object);
void use_object(object_t object);
void push_object(object_t object);
void pull_object(object_t* object);
void producer();
void consumer();
