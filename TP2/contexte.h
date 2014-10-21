#define STACK_SIZE 16000
#define dump_sp()                                                       \
  do {									\
    void * ctx_esp;							\
    void * ctx_ebp;							\
    asm ("movl %%ebp, %1" "\n\t"                                        \
         "movl %%esp, %0"						\
         : "=r"(ctx_esp), "=r"(ctx_ebp)/* y is output operand */	\
         :								\
         ); 				\
    printf("EBP: %x, ESP: %x\n",(unsigned int)ctx_ebp, (unsigned int)ctx_esp); \
  }while(0)


typedef void (func_t)(void*);

typedef enum {READY, ACTIVATED, TERMINATED} status_e;


struct ctx_s
{

  void *ebp;
  void *esp;
  void *args;
#define CTX_MAGIC 0xDEADBEEF
  unsigned int ctx_magic;
  func_t *fct;
  int size;
  status_e status;
  char * pile;
};


int init_ctx(struct ctx_s *ctx, int stack_size, func_t f, void *args);
void f_ping(void *arg);
void f_pong(void *arg);
void switch_to_ctx(struct ctx_s *ctx);
