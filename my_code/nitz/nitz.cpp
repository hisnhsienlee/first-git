#include "nitz.hpp"

#define FILE_PATH "/tmp/nitz"
std::mutex mutex_;

int read_nitz(char *buffer, int size) {
    std::lock_guard<std::mutex> lock(mutex_);
    FILE *fp;
    fp = fopen(FILE_PATH, "r");
    if (fp != NULL) {
        fread(buffer, sizeof(char), size, fp);
        fclose(fp);
        return 0;
    }
    return 1;
}

int write_nitz(std::string string) {
    std::ofstream file;
    std::lock_guard<std::mutex> lock(mutex_);
    file.open(FILE_PATH, std::ofstream::out | std::ofstream::trunc);
    if(file.is_open()) {
        file.close();
    }

    file.open(FILE_PATH, std::ios::app);
    if(file.is_open()) {
        file << string << std::endl;
        file.close();
        return 0;
    }
    return 1;
}

int main()
{
    char buffer[512] ={0};
    std::string nitz= "23/09/18,05:30:10+32,00";
    write_nitz(nitz);
    read_nitz(buffer,512);
    printf("%s \n" , buffer);
    return 0;
}