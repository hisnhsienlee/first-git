#ifndef NITZ_HPP
#define NITZ_HPP

#include <fstream>
#include <sstream>
#include <mutex>

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>


int nitz_read(char *buffer, int size);

int nitz_write();


#endif //NITZ_HPP