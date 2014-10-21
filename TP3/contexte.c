#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "contexte.h"
#include "hw.h"


static ctx_t context_general = NULL;
static ctx_t premier = NULL;


int
main()
{
  create_ctx(16384, f_ping, NULL);
  create_ctx(16384, f_pong, NULL);
  start_sched();
  while(1);
  exit(EXIT_SUCCESS);

}



/**
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
  return 0;
} 
void yield(){
  
  
  switch_to_ctx(context_general->next_ctx);
  
  return ;

}

void f_ping(void *args)
{
  int i = 0;
  while(1){
    if(!(i++%100000000)){
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
    if(!(i++%100000000)){
      printf("1") ;
      printf("2") ;
      printf("3") ;
      printf("4") ;
      printf("\n");
    }
  }
} 

void
switch_to_ctx(struct ctx_s *ctx)
{

  
  assert(ctx->ctx_magic == CTX_MAGIC);
  assert(ctx->status != TERMINATED);
  
  if(context_general != NULL){
    irq_disable();
    asm("movl %%ebp,%0": "=r" (context_general->ebp));
    asm("movl %%esp,%0": "=r" (context_general->esp));
    irq_enable();
  }
  
  irq_disable();
  context_general = ctx;
  asm("movl %0,%%ebp": : "r" (context_general->ebp));
  asm("movl %0,%%esp": : "r" (context_general->esp));
  irq_enable();
  if(context_general->status == READY){
    irq_disable();
    context_general->status = ACTIVATED;
    irq_enable();
    context_general->fct(context_general->args);
    irq_disable();
    context_general->status = TERMINATED;

    irq_enable();
    exit(EXIT_SUCCESS);    
  
  }
}



void
start_sched()
{
  start_hw();
  setup_irq(TIMER_IRQ,yield);
  
}
