#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#include "sys.hpp"


int main() {
    
	sys_get_hw_version();
    sys_get_sw_version();
    sys_get_sku_info();
    return 0;
}

