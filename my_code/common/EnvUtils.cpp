/**
 *  Copyright (c) 2018 Qualcomm Technologies, Inc.
 *  All Rights Reserved.
 *  Confidential and Proprietary - Qualcomm Technologies, Inc.
 */

#include <fstream>
#include <iostream>
#include <string>

extern "C" {
#include <limits.h>
#include <unistd.h>
}

#include "EnvUtils.hpp"

namespace telux {

namespace common {

/**
 * Get current running process path.
 */
std::string EnvUtils::getCurrentProcessPath() {
   char path[PATH_MAX];
   ssize_t count = readlink("/proc/self/exe", path, PATH_MAX);
   std::string fullPath = std::string(path, (count > 0) ? count : 0);
   return fullPath;
}

/**
 * Get current running application name.
 */
std::string EnvUtils::getCurrentAppName() {
   std::string path = getCurrentProcessPath();
   auto const pos = path.find_last_of('/');
   return path.substr(pos + 1);
}

/**
 * Get folder path where application is running.
 */
std::string EnvUtils::getAppPath() {
   std::string fullPath = getCurrentProcessPath();
   auto const pos = fullPath.find_last_of('/');
   return fullPath.substr(0, pos);
}

}  // end namespace common

}  // end namespace telux
