/*
 *  Copyright (c) 2017-2021 Qualcomm Technologies, Inc.
 *  All Rights Reserved.
 *  Confidential and Proprietary - Qualcomm Technologies, Inc.
 */

/**
 * Logger class - provides wrappers for writing log messages to console, diag and log
 * file
 */

extern "C" {
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <syslog.h>

//#include "msg.h"
//#include "diag_lsm.h"
}

#include "Logger.hpp"
#include "Settings.hpp"
#include "EnvUtils.hpp"
#include "CommonUtils.hpp"

#define STAT_FAILURE -1

#define DEFAULT_LOG_FILE_PATH "./"
#define DEFAULT_LOG_FILE_NAME "tel.log"

#define DEFAULT_LOG_FILE_MAX_SIZE 5 * 1024 * 1024  // 5 MB

namespace telux {
namespace common {

Logger &Logger::getInstance() {
   static Logger instance;
   return instance;
}

Logger::Logger() {
   init();
   isLoggerInitialized_ = true;
}

Logger::~Logger() {
   isLoggerInitialized_ = false;
   // Close log file stream if it is open
   if(logFileStream_.is_open()) {
      logFileStream_.close();
   }
   if (changeLogFileSem_ != SEM_FAILED) {
      sem_close(changeLogFileSem_);
   }
}

void Logger::initProcessId() {
   processID_ = getpid();
}

void Logger::initProcessName() {
   processName_ = EnvUtils::getCurrentAppName();
}

void Logger::init() {
   // set required data member by reading configuration file
   initFileLogLevel();
   initConsoleLogLevel();
   //initDiagLogLevel();
   initComponentLogging();
   initLogFileName();
   initLogFileMaxSize();
   initDateTime();
   initProcessName();
   initProcessId();

   // initialize logging
   initFileLogging();
   initConsoleLogging();
   //initDiagLogging();
}

void Logger::initComponentLogging() {
   //Getting the value from tel.conf
   std::string tempStr = Settings::getValue(std::string("TELUX_LOG_COMPONENT_FILTER"));

   if (!tempStr.empty()) {
      std::vector<std::string> logFilter = CommonUtils::splitString(tempStr, ',');
      for(auto s:logFilter) {
         int vertical = stoi(s);
         componentLogFilter_.set(vertical);
      }
   }
}

void Logger::initFileLogging() {
   if(fileLogLevel_ == LogLevel::LEVEL_NONE) {
      isLoggingToFileEnabled_ = false;
   }

   isLoggingToFileEnabled_ = true;
   // Initialize the log file
   if(!logFileStream_.is_open()) {
      // Open the log file for writing
      mode_t mask = umask(0);
      std::string logFile = logFileFullName_;
      logFileStream_.open(logFile, std::ios::app);
      if(logFileStream_.rdstate() != std::ios_base::goodbit) {
          syslog(LOG_NOTICE, "%s open %s fail", __FUNCTION__, logFile.c_str());
          umask(mask);
          return;
      }
      struct stat st;
      if (stat(logFile.c_str(), &st) != STAT_FAILURE) {
         inodeNumber_ = st.st_ino;
      }
      changeLogFileSem_ = sem_open("telLog", O_CREAT | O_RDWR, 0660, 1);
      if (changeLogFileSem_ == SEM_FAILED) {
          logFileStream_ << "Fail open sem.telLog" << std::endl;
      }
      umask(mask);
   }
}

void Logger::initConsoleLogging() {
   if(consoleLogLevel_ == LogLevel::LEVEL_NONE) {
      isLoggingToConsoleEnabled_ = false;
   }
   isLoggingToConsoleEnabled_ = true;
}

// Initialize logging in diag
//void Logger::initDiagLogging() {
//   // Initialize Diag LSM
//   Diag_LSM_Init(NULL);
//}

void Logger::initLogFileName() {
   logFileFullName_ = Settings::getValue(std::string("LOG_FILE_PATH"));

   // Use current directory as default path
   if(logFileFullName_ == "") {
      logFileFullName_ += DEFAULT_LOG_FILE_PATH;
   }
   logFileFullName_ += "/";

   std::string logFileName = Settings::getValue(std::string("LOG_FILE_NAME"));
   if(logFileName != "") {
      logFileFullName_ += logFileName;
   } else {
      logFileFullName_ += DEFAULT_LOG_FILE_NAME;  // Default log file name if not configured
   }
}

void Logger::initLogFileMaxSize() {
   std::string val = Settings::getValue(std::string("MAX_LOG_FILE_SIZE"));
   logFileMaxSize_ = DEFAULT_LOG_FILE_MAX_SIZE;

   if(!val.empty()) {
      logFileMaxSize_ = stoi(val);
   }
}

// Routine Provides time stamp in Nano Seconds
void Logger::getTimeStampNs(struct timespec *tp) {
   tp->tv_sec = tp->tv_nsec = 0;
   clock_gettime(CLOCK_BOOTTIME, tp);
}

const std::string Logger::getCurrentTime() {
   std::tm tmSnapshot;
   std::stringstream ss;
   // Get the current calendar time (Epoch time)
   std::time_t nowTime = std::time(nullptr);

   // std::put_time to get the date and time information from a given calendar time.
   // ::localtime_r() converts the calendar time to broken-down time representation
   // but stores the data in a user-supplied struct, and it is thread safe
   ss << std::put_time(::localtime_r(&nowTime, &tmSnapshot), "%b-%d-%Y %H:%M:%S");
   return ss.str();
}

void Logger::initDateTime() {
   std::string val = Settings::getValue(std::string("LOG_PREFIX_DATE_TIME"));
   if(!val.empty()) {
      if(val == "TRUE") {
         isDateTimeEnabled_ = true;
      } else if(val == "FALSE") {
         isDateTimeEnabled_ = false;
      }
   }
}

void Logger::initConsoleLogLevel() {
   consoleLogLevel_ = LogLevel::LEVEL_INFO;
   std::string logLevelString = Settings::getValue(std::string("CONSOLE_LOG_LEVEL"));
   if(!logLevelString.empty()) {
      consoleLogLevel_ = getLogLevel(logLevelString);
   }
}

LogLevel Logger::getConsoleLogLevel() {
   return consoleLogLevel_;
}

void Logger::initFileLogLevel() {
   fileLogLevel_ = LogLevel::LEVEL_INFO;
   std::string logLevelString = Settings::getValue(std::string("FILE_LOG_LEVEL"));
   if(!logLevelString.empty()) {
      fileLogLevel_ = getLogLevel(logLevelString);
   }
}

LogLevel Logger::getFileLogLevel() {
   return fileLogLevel_;
}

//void Logger::initDiagLogLevel() {
//   diagLogLevel_ = LogLevel::LEVEL_DEBUG;
//   std::string logLevelString = Settings::getValue(std::string("DIAG_LOG_LEVEL"));
//   if(!logLevelString.empty()) {
//      diagLogLevel_ = getLogLevel(logLevelString);
//   }
//}

//LogLevel Logger::getDiagLogLevel() {
//   return diagLogLevel_;
//}

void Logger::writeToConsole(std::ostringstream &outputStream) {
   // newline applied will flush the buffer to stdout
   std::cout << outputStream.str() << std::endl;
}

/* if log file size larger than specified max allowed size then backup the log file and reopen it
 * so log prints to the new log file rather than the backup log file;
  * if found the log file inode number changed which means the log file was backup by someone
  * else , reopen it so log message prints to expected log file.
   *write log message to specified file if the fstream has goodbit, otherwise prints to syslog;
*/
void Logger::writeToFile(std::ostringstream &outputStream) {
   bool logFileChanged = false;
   struct stat st;
   if (stat(logFileFullName_.c_str(), &st) == STAT_FAILURE) {
       return;
   }
   if (st.st_size > logFileMaxSize_) {
      logFileChanged = backupLogFile();
   }
   if (inodeNumber_ != st.st_ino) {
      logFileChanged  = true;
   }

   // Protects log file stream when it is accessed by multiple threads
   std::lock_guard<std::mutex> lock(logFileMutex_);
   if (logFileChanged) {
      inodeNumber_ = reopenLogFile();
   }

   if(logFileStream_.rdstate() == std::ios_base::goodbit) {
      // Write the log message into the file
      logFileStream_ << outputStream.str() << std::endl;
   } else {
      syslog(LOG_NOTICE, "%s", outputStream.str().c_str());
   }
}

//void Logger::writeToDiag(std::ostringstream &outputStream, LogLevel logLevel) {
//   //MSG_SPRINTF_1 takes a c-style string as input
//   std::string logMessage = outputStream.str();
//   switch(logLevel) {
      /*
       * Mapping of log levels in QXDM
       * Perf logs are mapped to MSG_LEGACY_FATAL.
       * Error logs are mapped to MSG_LEGACY_ERROR.
       * Warning logs are mapped to MSG_LEGACY_HIGH.
       * Info logs are mapped to MSG_LEGACY_MED.
       * Debug logs are mapped to MSG_LEGACY_LOW.
       */

//      case LogLevel::LEVEL_PERF:
//         MSG_SPRINTF_1(MSG_SSID_TELUX, MSG_LEGACY_FATAL, "%s", logMessage.c_str());
//         break;
//      case LogLevel::LEVEL_ERROR:
//         MSG_SPRINTF_1(MSG_SSID_TELUX, MSG_LEGACY_ERROR, "%s", logMessage.c_str());
//         break;
//      case LogLevel::LEVEL_WARNING:
 //        MSG_SPRINTF_1(MSG_SSID_TELUX, MSG_LEGACY_HIGH, "%s", logMessage.c_str());
 //        break;
 //     case LogLevel::LEVEL_INFO:
//         MSG_SPRINTF_1(MSG_SSID_TELUX, MSG_LEGACY_MED, "%s", logMessage.c_str());
 //        break;
 //     case LogLevel::LEVEL_DEBUG:
 //        MSG_SPRINTF_1(MSG_SSID_TELUX, MSG_LEGACY_LOW, "%s", logMessage.c_str());
 //        break;
 //     default:
 //        break;
 //  }
//}

void Logger::writeLogMessage(std::ostringstream &os, LogLevel logLevel, const std::string &fileName,
                             const int &component, const std::string &lineNo) {
   std::string timeStamp = "";
   std::string fileNameAndLineNo = "";
   std::string processIdAndName = "";
   std::ostringstream outputStream;
   struct timespec tp;
   tp.tv_sec = tp.tv_nsec = 0;

   // Get current date and time from system if LOG_PREFIX_DATE_TIME flag enabled
   if(isDateTimeEnabled_) {
      timeStamp = " " + this->getCurrentTime();
   }

   // get process id and name
   processIdAndName = std::to_string(processID_) + "/" + processName_;

   // get the filename from full path
   const char *lastSlash = std::strrchr(fileName.c_str(), '/');
   if(lastSlash != nullptr) {
      fileNameAndLineNo = " " + std::string(lastSlash + 1) + "(" + lineNo + ") ";
   } else {
      fileNameAndLineNo = " " + std::string(fileName) + "(" + lineNo + ") ";
   }

   switch(logLevel) {
      case LogLevel::LEVEL_ERROR:
         outputStream << "[E]" << timeStamp << " " << processIdAndName << fileNameAndLineNo;
         break;
      case LogLevel::LEVEL_WARNING:
         outputStream << "[W]" << timeStamp << " " << processIdAndName << fileNameAndLineNo;
         break;
      case LogLevel::LEVEL_INFO:
         outputStream << "[I]" << timeStamp << " " << processIdAndName << fileNameAndLineNo;
         break;
      case LogLevel::LEVEL_DEBUG:
         outputStream << "[D]" << timeStamp << " " << processIdAndName << fileNameAndLineNo;
         break;
      case LogLevel::LEVEL_PERF:
         // Get current time in nano second from BOOT
         this->getTimeStampNs(&tp);
         outputStream << "[TS]" << timeStamp << " " << tp.tv_sec << "." << tp.tv_nsec << " " <<
         processIdAndName << fileNameAndLineNo;
         break;
      default:
         break;
   }

   // Print thread id for debugging
   outputStream << std::this_thread::get_id() << ": ";

   //Check if the ostringstream containing the input argument is empty
   if(!os.str().empty()) {
      outputStream << os.str();

      if(consoleLogLevel_ >= logLevel) {
         writeToConsole(outputStream);
      }

      //if(diagLogLevel_ >= logLevel) {
      //   writeToDiag(outputStream, logLevel);
      //}

      // Don't log into file unless logging to file is enabled
      if(fileLogLevel_ >= logLevel) {
         writeToFile(outputStream);
      }
   }
}

LogLevel Logger::getLogLevel(std::string logLevelString) {
   LogLevel logLevel = LogLevel::LEVEL_DEBUG;  // default log level
   if(logLevelString == "PERF"){
      logLevel = LogLevel::LEVEL_PERF;
   }else if(logLevelString == "ERROR") {
      logLevel = LogLevel::LEVEL_ERROR;
   } else if(logLevelString == "WARNING") {
      logLevel = LogLevel::LEVEL_WARNING;
   } else if(logLevelString == "INFO") {
      logLevel = LogLevel::LEVEL_INFO;
   } else if(logLevelString == "NONE") {
      logLevel = LogLevel::LEVEL_NONE;
   }
   return logLevel;
}

bool Logger::isComponentLogged(const int component) {

   bool shouldLog = false;

   // componentLogFilter_ 0th bit set handle case when all logs are printed.
   // componentLogFilter_.test(component) handle case when component specified in configuration
   // component = 0 handle case when app uses using logger api.
   if (componentLogFilter_.test(0) || componentLogFilter_.test(component) || component == 0) {
      shouldLog = true;
   }
   return shouldLog;
}

bool Logger::isLoggingEnabled(LogLevel logLevel, const int& component) {

   //When component-filtering is enabled then the logs from that technology domain, the logs from
   //common & qmi domain and the apps logs are also printed.
   bool allowComponent = isComponentLogged(component);
   bool logToConsole = consoleLogLevel_ >= logLevel;
   bool logToFile = fileLogLevel_ >= logLevel;
   //bool logToDiag = diagLogLevel_ >= logLevel;
   return ((logToConsole || logToFile ) && (allowComponent));//|| logToDiag
}

/*Close current log file stream and open the new specified log file stream,
  it also change the ownership of the new opened log file.
  This usually called when the current log file backup happen.
  Returns the new log file's inodeNumber in success case, otherwise 0 on failure.
 */
ino_t Logger::reopenLogFile() {
   struct stat st;
   if (logFileStream_.is_open()) {
      logFileStream_.close();
   }
   logFileStream_.clear();

   logFileStream_.open(logFileFullName_, std::ios::app);
   chmod(logFileFullName_.c_str(), 0666);
   if (logFileStream_.rdstate() == std::ios_base::goodbit) {
       stat(logFileFullName_.c_str(), &st);
       return st.st_ino;
   }
   return 0;
}

/* if there is already backup log file and it is not the same one with the current logging file
 * then delete the backup file.
 * rename current logging file to backup log file.
*/
bool Logger::backupLogFile() {
   if (changeLogFileSem_ == SEM_FAILED) {
      syslog(LOG_NOTICE, "%s changeLogFileSem_ == SEM_FAILED", __FUNCTION__);
      return false;
   }
   if (0 != sem_trywait(changeLogFileSem_)) {
      /*someone else is doing the same thing*/
      syslog(LOG_NOTICE, "%s wait semaphore failure %d", __FUNCTION__, errno);
      return false;
   }

   // if backup file exists on the disk delete it
   std::string backupFileName = logFileFullName_ + ".backup";
   struct stat st;
   bool skipBackup = false;
   if (stat(backupFileName.c_str(), &st) != STAT_FAILURE) {
      if (inodeNumber_ != st.st_ino) {
         remove(backupFileName.c_str());
      } else {/*this backup file is the one backup by someone else just now*/
         skipBackup = true;
      }
   }
   if (not skipBackup) {
      rename(logFileFullName_.c_str(), backupFileName.c_str());
   }

   sem_post(changeLogFileSem_);
   return true;
}

}
}
