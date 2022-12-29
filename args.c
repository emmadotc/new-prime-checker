#include <string.h>

#include "args.h"

void init_args(_Args* args)
{
    args->printing = 0;
}

void parse_args(int argc, char** argv, _Args* args)
{
    for(int i = 2; i < argc; ++i)
    {
        if(strlen(argv[i]) > 1)
        {
            if(argv[i][0] == '-')
            {
                if(argv[i][1] == '-')
                {
                    if(strcmp(argv[i], "--print") == 0)
                    {
                        args->printing = 1;
                    }
                } else {
                    for(unsigned long j = 1; j < strlen(argv[i]); ++j)
                    {
                        switch(argv[i][j])
                        {
                            case 'p':
                                args->printing = 1;
                                continue;
                            default:
                                continue;
                        }
                    }
                }
            }
        }
    }
}

