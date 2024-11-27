/*
 *  Copyright (c) 2017,2019-2020 Qualcomm Technologies, Inc.
 *  All Rights Reserved.
 *  Confidential and Proprietary - Qualcomm Technologies, Inc.
 */

#include <iomanip>
#include <regex>

#include "CommonUtils.hpp"
#include "Logger.hpp"

namespace telux {

namespace common {

/**
 * Logs hex dump from a given string
 */
void CommonUtils::hexDump(const char *data, int length) {
   LOG(DEBUG, "Data:");
   for(int i = 0; i < length; ++i) {
      std::ostringstream dataStream;
      dataStream << std::hex << data[i] << " ";
   }
}

bool CommonUtils::isEqualStrings(std::string str1, std::string str2) {
    return ((str1.size() == str2.size())
            && std::equal(str1.begin(), str1.end(), str2.begin(), [](char &c1, char &c2) {
                   return (c1 == c2 || std::toupper(c1) == std::toupper(c2));
               }));
}

std::vector<std::string> CommonUtils::splitString(const std::string &s, char delim) {
    std::vector<std::string> elements;
    std::istringstream ss(s);
    std::string token;

    while(std::getline(ss, token, delim)) {
        // remove the trailing spaces
        token.erase(std::remove(token.begin(),token.end(),' '),token.end());
        if(!token.empty()) {
            elements.push_back(token);
        }
    }
    return elements;
}

}  // End  of namespace common

}  // End  of namespace telux
