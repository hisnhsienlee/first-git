/**
 *  Copyright (c) 2017-2018 Qualcomm Technologies, Inc.
 *  All Rights Reserved.
 *  Confidential and Proprietary - Qualcomm Technologies, Inc.
 */

#include <fstream>
#include <iostream>
#include <regex>
#include <string>

#include "Settings.hpp"
#include "EnvUtils.hpp"

namespace telux {

namespace common {

std::map<std::string, std::string> Settings::settingsMap_;
std::string Settings::configFile_;
std::string Settings::appPath_;

/**
 * Check if a file exists.
 */
inline bool fileExists(const std::string &configFile) {
   std::ifstream f(configFile.c_str());
   return f.good();
}

/**
 * Get the user defined value for any configuration setting
 *
 * Format trims and uppercases mapped value that is returned
 */
std::string Settings::getValue(std::string key, bool format) {
   // Check if settingsMap_ is initialized. If not, initialize it.
   if(settingsMap_.size() == 0) {
      configFile_ = getConfigFilePath();
      readSettingsFile(configFile_);
   }
   auto settingsIterator = settingsMap_.find(key);
   if(settingsIterator != settingsMap_.end()) {

      if (format) {
         std::string whitespaces (" \t\f\v\n\r");
         auto begin = settingsIterator->second.find_first_not_of(whitespaces);
         if (std::string::npos != begin) {
             auto end = settingsIterator->second.find_last_not_of(whitespaces);
             settingsIterator->second = settingsIterator->second.substr(begin, end+1);

             std::for_each(settingsIterator->second.begin(), settingsIterator->second.end(),
                           [](char& c) {
                               c = toupper(static_cast<unsigned char> (c));
                           });
         } else {
             // If value is only spaces, convert to empty string
             settingsIterator->second = "";
         }
      }

      return settingsIterator->second;
   } else {
      return std::string("");  // return an empty string when the setting is not configured.
   }
}

/**
 * Utility function to read config file with key value pairs
 * Prepares a map of key value pairs from Key=Value format
 * Discards leading spaces, blank lines and lines starting with #
 * Removes any leading or training spaces around Key and Value if any
 */
void Settings::readSettingsFile(std::string configFilePath) {

   // Create a file stream from the file name
   std::ifstream configFileStream(configFilePath);

   // Iterate through each parameter in the file and read the key value pairs
   std::string param;
   while(std::getline(configFileStream >> std::ws, param)) {
      std::string key;
      std::istringstream paramStream(param);
      if(std::getline(paramStream, key, '=')) {
         // Ignore lines starting with # character
         if(key[0] == '#') {
            continue;
         }
         key = std::regex_replace(key, std::regex(" +$"), "");
         if(key.length() > 0) {
            std::string value;
            if(std::getline(paramStream, value)) {
               value = std::regex_replace(value, std::regex("^ +| +$"), "");
               settingsMap_[key] = value;
            }
         }
      }
   }
}

/**
 * Prints the contents of the map. i.e. all configured settings
 */
void Settings::printSettings() {
   for(auto &iter : settingsMap_) {
      std::cout << "Key: " << iter.first << ",     "
                << "Value: " << iter.second << std::endl;
   }
}

/**
 * Order of search for the config file:
 * 1. Search for <appName>.conf in etc folder.
 * 2. Search for <appName>.conf in the folder that contains the app.
 * 3. Search for tel.conf in etc folder.
 * 4. Search for tel.conf in the folder that contains the app.
 */
std::string Settings::getConfigFilePath() {

   // get folder path where application is running
   appPath_ = EnvUtils::getAppPath();
   // get current config file from /etc
   configFile_ = "/etc/" + EnvUtils::getCurrentAppName() + ".conf";

   if(fileExists(configFile_)) {
      return configFile_;
   }

   // get current config file where application is running
   configFile_ = appPath_ + "/" + EnvUtils::getCurrentAppName() + ".conf";
   if(fileExists(configFile_)) {
      return configFile_;
   }

   // get default config file (tel.conf) from /etc
   configFile_ = "/etc/" + std::string(DEFAULT_CONFIG_FILE);
   if(fileExists(configFile_)) {
      return configFile_;
   }

   // get default config file where application is running
   configFile_ = appPath_ + "/" + std::string(DEFAULT_CONFIG_FILE);
   if(fileExists(configFile_)) {
      return configFile_;
   }

   // return an empty string when the default file is not configured.
   configFile_ = "";
   return configFile_;
}

}  // end namespace common

}  // end namespace telux
