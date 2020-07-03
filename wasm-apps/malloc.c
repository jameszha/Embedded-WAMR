#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    char *buf;

    printf("Mallocing buffer and writing '1234' to it\n");

    buf = malloc(1024);
    if (!buf) {
        printf("malloc buf failed\n");
        return -1;
    }
    printf("malloc success\n");

    printf("buf ptr: %p\n", buf);

    snprintf(buf, 1024, "%s", "1234\n");
    printf("buf: %s", buf);

    free(buf);
    return 0;
}
