#ifndef ERROR_H
#define ERROR_H

// #define MY_NDEBUG

#define handleErr(err_num) handleErrFn(err_num, __FILE__, __LINE__, __FUNCTION__)
    
#define returnErr(expr)     \
{                           \
    ErrEnum err_num = expr; \
    if (err_num)            \
        return err_num;     \
}

#ifdef MY_NDEBUG
#define myAssert(expr)
#else
#define myAssert(expr) myAssertFn(expr, #expr, __FILE__, __LINE__, __FUNCTION__)
#endif


enum ErrEnum
{
    ERR_OK = 0,
    ERR_UNKNOWN,

    ERR_MEM,
    ERR_OPEN_FILE,
    ERR_FILE,

    ERR_ASSERT,
    ERR_STACK_UNDERFLOW,
    ERR_NULL_STACK,
    ERR_BAD_SIZE,
    ERR_POISON,
    ERR_BAD_CANARY,
    ERR_BAD_HASH,

    ERR_NULL_TREE,
    ERR_INVAL_CONNECT,
    ERR_INVAL_NNODES,
    ERR_TREE_FMT,
    ERR_NOT_LEAF,

    ERR_INVAL_YES_NO_ANS,
};

struct ErrDescr
{
    ErrEnum num;
    const char* descr;
};

void myAssertFn(int expr, const char* str_expr, const char* file, int line, const char* function);
void getErrDescr(ErrEnum num, const char** descr);
void handleErrFn(ErrEnum num, const char* file, int line, const char* function);

#endif // ERROR_H