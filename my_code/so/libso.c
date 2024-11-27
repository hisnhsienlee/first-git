#include <stdio.h>
struct person_t {
    char *name;
    unsigned age;
}person;

int bejo_lib(char *name, int i)
{
    printf("I am %s, do i=%d\n", name, i);

    return 0;
}

int bejo_lib2(struct person_t name)
{
    printf("I am do\n");

    return 0;
}