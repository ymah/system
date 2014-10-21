#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "contexte.h"
#include "hw.h"
#include "sem.h"



int
main()
{

  sem_init(&mutex, 1 ,"Mutex");
  sem_init(&vide, N ,"Vide");
  sem_init(&plein, 0,"Plein");

  create_ctx(16384, &producer, NULL);

  create_ctx(16384, &consumer, NULL);
  start_sched();
  while(1);
  printf("done\n");
  exit(EXIT_SUCCESS);

}


/*
  fonction init_ctx : intitialse le contexte
*/
int
init_ctx(struct ctx_s *ctx, int stack_size, func_t f, void *args)
{
  ctx->pile = malloc(sizeof(char)*stack_size);
  ctx->esp = ctx->pile + stack_size - 4;
  ctx->ebp = ctx->pile + stack_size - 4;
  ctx->fct = f;
  ctx->args = args;
  ctx->status = READY;
  ctx->ctx_magic = CTX_MAGIC;
  ctx->next_ctx = NULL;
  ctx->size = stack_size;

  return ctx->status;


}
int create_ctx(int stack_size, func_t f, void *args){

  irq_disable();
  if(!premier){
    context_general=(ctx_t)malloc(sizeof(ctx_t));
    init_ctx(context_general,stack_size,f,args);
    premier = context_general;

  }else {

    ctx_t ctx = (ctx_t)malloc(sizeof(ctx_t));
    init_ctx(ctx,stack_size,f,args);
    ctx->next_ctx = premier;
    context_general->next_ctx = ctx;
    context_general = ctx;

  }
  irq_enable();
  return 0;
}
void yield(){

  if(!context_general){
    assert(premier);
    switch_to_ctx(premier);
  }
  else{
    switch_to_ctx(context_general->next_ctx);
  }


  return ;

}

void f_ping(void *args)
{
  int i = 0;
  while(1){
    if(!(i++%10000000)){
      printf("A") ;
      printf("\n") ;
      printf("B") ;
      printf("\n") ;
      printf("C") ;
      printf("\n") ;
    }
  }
}

void f_pong(void *args)
{
  int i = 0;
  while(1){
    if(!(i++%10000000)){
      printf("1") ;
      printf("2") ;
      printf("3") ;
      printf("4") ;
      printf("\n");
    }
  }
}

void f_pang(void *args)
{
  int i = 0;
  while(1){
    if(!(i++%10000000)){
      printf("K") ;
      printf("L") ;
      printf("M") ;
      printf("N") ;
      printf("O");
    }
  }
}

void
switch_to_ctx(struct ctx_s *ctx)
{


  assert(ctx->status != TERMINATED);
  
  irq_disable();
  while(ctx->status == TERMINATED || ctx->status== BLOCKED){
        if(ctx->status== TERMINATED) printf("Finished context encountered\n");
        if(ctx->status == BLOCKED) printf("Frozen context encountered\n");
        if(ctx == ctx->next_ctx){
            /* return to main */
            free(ctx->pile);
            free(ctx);
            printf("Return to main\n");
            __asm__ ("movl %0, %%esp\n" ::"r"(premier->esp));
            __asm__ ("movl %0, %%ebp\n" ::"r"(premier->ebp));
            return;
        }
        else {
            struct ctx_s *next = ctx->next_ctx;
            if (ctx->status == TERMINATED){
                context_general->next_ctx = next;
                free(ctx->pile);
                free(ctx);
            }
            ctx = next;
        }



  }
  if(context_general != NULL){
    asm("movl %%ebp,%0": "=r" (context_general->ebp));
    asm("movl %%esp,%0": "=r" (context_general->esp));
    irq_enable();
  }
  context_general = ctx;
  asm("movl %0,%%ebp": : "r" (context_general->ebp));
  asm("movl %0,%%esp": : "r" (context_general->esp));
  if(context_general->status == READY){
    context_general->status = ACTIVATED;
    context_general->fct(context_general->args);
    context_general->status = TERMINATED;
    exit(EXIT_SUCCESS);

  }
  irq_enable();
}



void
start_sched()
{
  start_hw();
  setup_irq(TIMER_IRQ,yield);
  yield();
}
