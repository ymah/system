#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "contexte.h"

struct ctx_s ctx_ping; 
struct ctx_s ctx_pong; 

struct ctx_s *courant = NULL;

int
main()
{
  init_ctx(&ctx_ping, 16384, f_ping, NULL);
  init_ctx(&ctx_pong, 16384, f_pong, NULL);
  switch_to_ctx(&ctx_ping);
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

  return ctx->status;


}

void f_ping(void *args)
{
  while(1) {
    printf("A") ;
    switch_to_ctx(&ctx_pong);
    printf("B") ;
    switch_to_ctx(&ctx_pong);
    printf("C") ;
    switch_to_ctx(&ctx_pong);
  }
}

void f_pong(void *args)
{
  while(1) {
    printf("1") ;
    switch_to_ctx(&ctx_ping);
    printf("2") ;
    switch_to_ctx(&ctx_ping);
  }
} 

void
switch_to_ctx(struct ctx_s *ctx)
{
  
  assert(ctx->ctx_magic == CTX_MAGIC);
  assert(ctx->status != TERMINATED);

  if(courant != NULL){
    
    asm("movl %%ebp,%0": "=r" (courant->ebp));
    asm("movl %%esp,%0": "=r" (courant->esp));
  }
  courant = ctx;

  asm("movl %0,%%ebp": : "r" (courant->ebp));
  asm("movl %0,%%esp": : "r" (courant->esp));

  if(courant->status == READY){
    courant->status = ACTIVATED;
    courant->fct(courant->args);
    courant->status = TERMINATED;
    exit(EXIT_SUCCESS);    
  }
  
}
