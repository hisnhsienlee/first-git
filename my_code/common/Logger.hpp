/*
 *  Copyright (c) 2017-2021 Qualcomm Technologies, Inc.
 *  All Rights Reserved.
 *  Confidential and Proprietary - Qualcomm Technologies, Inc.
 */

#ifndef LOGGER_HPP
#define LOGGER_HPP

extern "C" {
#include <semaphore.h>
}

#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>
#include <cstring>
#include <vector>
#include <bitset>

#include "Settings.hpp"
#include "Log.hpp"

namespace telux {
namespace common {

/**
 * Logger class - A singleton class which provides interface to log messages to
 *                a console, diag and to a log file.
 * Log level is configurable
 */
class Logger {

public:
   static Logger &getInstance();

   //getXxxLogLevel functions are utilized in the test cases.
   /*
    * Get the current console logging level
    */
   LogLevel getConsoleLogLevel();

   /*
    * Get the file console logging level
    */
   LogLevel getFileLogLevel();

   /*
    * Get the diag logging level
    */
   LogLevel getDiagLogLevel();

   /*
    * write log a message to console, diag and a log file based on the settings.
    */
   void writeLogMessage(std::ostringstream &os, LogLevel logLevel, const std::string &fileName,
                        const int &component, const std::string &lineNo);

   /*
    * get the current date and time of the device
    */
   const std::string getCurrentTime();

   /*
    * used to verify if instance of class is still valid.
    */
   bool isLoggerInitialized_ = false;

   /*
    * Singleton implementation, copy constructors are disabled.
    */
   Logger(const Logger &) = delete;

   Logger();
   ~Logger();

   /*
    * To check if logging is allowed for the given level and component
    * at least on one log sink
    */
   bool isLoggingEnabled(LogLevel logLevel, const int& component);

private:
   /*
    * initialization of logging
    */
   void init();

   /*
    * set log file max size
    */
   void initLogFileMaxSize();

   /*
    * Get log level from given level string
    */
   LogLevel getLogLevel(std::string logLevelString);

   /*
    * set log file name
    */
   void initLogFileName();

   /*
    * set console logging level to a desired threshold
    */
   void initConsoleLogLevel();

   /*
    * set file logging level to a desired threshold
    */
   void initFileLogLevel();

   /*
    * set diag logging level to a desired threshold
    */
   void initDiagLogLevel();

   /*
    * set component filter to log a desired component
    */
   void initComponentLogging();

   /*
    * set date and time
    */
   void initDateTime();

   /*
    * To provide time stamp with Nano Seconds
    */
   void getTimeStampNs(struct timespec *tp);

   /*
    * initialize log file
    */
   void initFileLogging();

   /*
    * initialize logs to console
    */
   void initConsoleLogging();

   /*
    * initialize diag logging
    */
   void initDiagLogging();

   /*
    * write logs message to console
    */
   void writeToConsole(std::ostringstream &outputStream);

   /*
    * write log message to file
    */
   void writeToFile(std::ostringstream &outputStream);

   /*
    * write log message to diag
    */
   void writeToDiag(std::ostringstream &outputStream, LogLevel logLevel);

   /*
    * set pid
    */
   void initProcessId();

   /*
    * set process name
    */
   void initProcessName();

   /*
    * check if this component should be logged
    */
   bool isComponentLogged(const int component);

   bool backupLogFile();

   ino_t reopenLogFile();

   LogLevel consoleLogLevel_, fileLogLevel_, diagLogLevel_;
   std::ofstream logFileStream_;
   std::mutex logFileMutex_;

   bool isLoggingToFileEnabled_ = false;
   bool isLoggingToConsoleEnabled_ = false;
   bool isDateTimeEnabled_ = false;
   std::string logFileFullName_;
   int logFileMaxSize_;
   int processID_;
   std::bitset<64> componentLogFilter_;
   std::string processName_;
   ino_t inodeNumber_ = 0;
   sem_t *changeLogFileSem_ = SEM_FAILED;
};

template <typename... MessageArgs>
void Log::logMessage(LogLevel logLevel, const std::string &fileName, const std::string &lineNo,
                     const int &component, MessageArgs... params) {
   Logger &logger = Logger::getInstance();

   // Validate if Logger instance is still valid before dumping log.
   if (logger.isLoggerInitialized_ && logger.isLoggingEnabled(logLevel, component)) {
      /*
      * Variable to buffer the input log message from input arguments
      */
      std::ostringstream outputStream;
      constructMessage(outputStream, params...);
      logger.writeLogMessage(outputStream, logLevel, fileName, component, lineNo);
   }
}

/*
 * Recursive helper methods to construct the complete log message
 * from input arguments
 */
template <typename K, typename T>
void Log::constructMessage(K &os, T param) {
   os << param;
}

template <typename K, typename T, typename... MessageArgs>
void Log::constructMessage(K &os, T param, MessageArgs... params) {
   os << param;
   return constructMessage(os, params...);
}
}
}

#endif
