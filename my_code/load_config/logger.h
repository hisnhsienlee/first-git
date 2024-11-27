#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <sstream>
#include <mutex>
#include <thread>
#include <chrono>
#include <iostream>
#include <string>
#include <atomic>

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#define LINE_STR(x) #x
#define LINE(x) LINE_STR(x)

#define LOG(args...) \
   log::log_msg(__FILE__, LINE(__LINE__), args)

enum class log_status {
    INIT,
    NOT_AVAILABLE,
    AVAILABLE,
};

class logger {
public:
    static logger &get_instance();
    logger();
    ~logger();
    void write_log_msg(std::ostringstream &os, const std::string &file_name,const std::string &line);
    void write_to_file(std::ostringstream &outputStream);
    void write_to_console(std::ostringstream &outputStream);
    void init_log_file_max_size();
    bool start_logger();
    void init();
    ino_t reopen_log_file();
private:
    std::mutex log_mutex_;
    static logger instance;
    std::ofstream logfile;
    int log_file_max_size_;
    ino_t inode_number_ = 0;
    std::atomic<log_status> log_status_{log_status::INIT};
};

class log {
public:
   template <typename... args>
   static void log_msg(const std::string &file_name, const std::string &line, args... params);
private:

   template <typename O, typename P>
   static void construct_msg(O &os, P param);

   template <typename O, typename P, typename... args>
   static void construct_msg(O &os, P param, args... params);
};

template <typename... args>
void log::log_msg(const std::string& file_name, const std::string &line, args... params) {
    
    logger &logger = logger::get_instance();

    if (logger.start_logger()) {
        std::ostringstream outputStream;
        construct_msg(outputStream, params...);
        logger.write_log_msg(outputStream, file_name, line);
    }
}

template <typename O, typename P>
void log::construct_msg(O &os, P param) {
   os << param;
}

template <typename O, typename P, typename... args>
void log::construct_msg(O &os, P param, args... params) {
   os << param;
   return construct_msg(os, params...);
}

#endif