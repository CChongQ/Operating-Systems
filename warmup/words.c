#include "common.h"

int
main(int argc, char **argv)
{
    if (argc > 1)
      {
        for (int count = 1; count < argc; count++)
          {
//            printf("argv[%d] = %s\n", count, argv[count]);
            printf("%s\n", argv[count]);
          }
      }

    return 0;
}
