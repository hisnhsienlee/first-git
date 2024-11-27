#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int main(int *argc, char **argv)
{
    void *fHandle;
    void (*func)();
    void (*func2)();

    fHandle = dlopen("./libso.so",RTLD_LAZY);

    if (!fHandle) {
        fprintf (stderr, "%s\n", dlerror());
        exit(1);
    }
    dlerror();

    func = (void(*)())dlsym(fHandle,"bejo_lib");

    if (func) {
        func("BEJO", 999);
    }

    
    func2 = (void(*)())dlsym(fHandle,"bejo_lib2");

    if (func2) {
        func2("BEJO");
    }

    dlclose(fHandle);
    return 0;
}