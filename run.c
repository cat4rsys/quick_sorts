#include <stdio.h>
#include <stdlib.h>

int main()
{
    for (int i = 128; i < 800000; i = (int)(1.25 * i))
    {
        char buf[1024] = {};
        snprintf(buf, 1024, "head -n %d onegin.txt | ./qsort", i);
        system(buf);
    }

    return 0;
}