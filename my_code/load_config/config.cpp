#include <fstream>
#include <sstream>
#include <mutex>
#include <thread>
#include <chrono>
#include <iostream>
#include <string>
#include <atomic>
#include <vector>

#include "logger.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#define FILENAME "./thermal-engine.conf"
#define BACKUP_FILENAME "./thermal-engine.conf.bak"

const std::vector<std::string> split(const std::string& str, const std::string& pattern) {
    std::vector<std::string> result;
    std::string::size_type begin, end;

    end = str.find(pattern);
    begin = 0;

    while (end != std::string::npos) {
        if (end - begin != 0) {
            result.push_back(str.substr(begin, end-begin)); 
        }    
        begin = end + pattern.size();
        end = str.find(pattern, begin);
    }

    if (begin != str.length()) {
        result.push_back(str.substr(begin));
    }
    return result;        
}

int main() {
	std::string xxx = "90001 100001 105001 110001 115001 120001";
	std::string pattern = " ";
   	std::vector<std::string> data = split(xxx, pattern);
	const char str1[]="\n";
	LOG("Start load config ...");
	std::ifstream file;
	std::ofstream backup_file;
	
    backup_file.open(BACKUP_FILENAME, std::ofstream::out | std::ofstream::trunc);
    backup_file.close();

 	file.open(FILENAME, std::ios::app);

 	backup_file.open(BACKUP_FILENAME, std::ios::app);

 	if (!file.is_open() && !backup_file.is_open())
 	{
 		LOG( "[WNC] Open file error");
 	}
	int on = 0;
	std::string line;
	while (std::getline(file, line)) {
		// using printf() in all tests for consistency
		if (line.length()!= 0 && line[0] != '#' )
		{
			if (line[0] == '[') {
				on = 0;
			}
			LOG( "[WNC]** ",line.c_str());
   			std::vector<std::string> ret = split(line, pattern);
			if (on) {
				if (ret.size() > 1 && ret[0] == "thresholds") {
					backup_file.write(ret[0].c_str(),strlen(ret[0].c_str()));
					backup_file.write(pattern.c_str(),strlen(pattern.c_str()));
					backup_file.write(xxx.c_str(),strlen(xxx.c_str()));
				} else {
					backup_file.write(line.c_str(),strlen(line.c_str()));
				}
				backup_file.write(str1,strlen(str1));
			} else {
				if (ret.size() >= 2 && ret[1] == "cpuss-0") { //需要修改的改得sensor判斷
					on = 1;
				}
				backup_file.write(line.c_str(),strlen(line.c_str()));
				backup_file.write(str1,strlen(str1));
			}
			
		} else {
			LOG("[WNC]-- ", line.c_str());
			backup_file.write(line.c_str(),strlen(line.c_str()));
			backup_file.write(str1,strlen(str1));
		}
	}
 	file.close();
 	backup_file.close();

error_handler:
 	LOG("close .....");
	return 0;
}
