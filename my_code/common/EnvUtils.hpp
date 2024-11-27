/*
 *  Copyright (c) 2018 Qualcomm Technologies, Inc.
 *  All Rights Reserved.
 *  Confidential and Proprietary - Qualcomm Technologies, Inc.
 */

/**
 * @brief EnvUtils provides utility functions to get configuration file path,
 * application name and current running application path.
 */

#ifndef ENVUTILS_HPP
#define ENVUTILS_HPP

#include <string>

#define DEFAULT_CONFIG_FILE "tel.conf"

namespace telux {

namespace common {

/*
 * EnvUtils class provides utility methods to get current running application and
 * config file path.
 */
class EnvUtils {
public:
   // Get folder path where application is running.
   static std::string getAppPath();

   // Get current running application name.
   static std::string getCurrentAppName();

   // Get current running process path.
   static std::string getCurrentProcessPath();

private:
   // avoid creating instance for this object
   EnvUtils() {
   }

};  // end of class EnvUtils

}  // End of namespace common

}  // End of namespace telux

#endif  // ENVUTILS_HPP
