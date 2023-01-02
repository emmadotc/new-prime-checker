#ifndef _h_args
#define _h_args

typedef struct Args {
    _Bool printing;
} _Args;

void init_args(_Args* args);
_Bool parse_args(int argc, char** argv, _Args* args);

#endif

