#include "logger.h"

#define STAT_FAILURE -1
#define FILE_PATH "./log.txt"
#define DEFAULT_LOG_FILE_MAX_SIZE 5 * 1024 * 1024

logger logger::instance;

logger::logger() {
    logfile.open(FILE_PATH, std::ofstream::out | std::ofstream::trunc);
    logfile.close();
    logfile.open(FILE_PATH, std::ios::app);
}

logger::~logger() {
    std::lock_guard<std::mutex> lock(log_mutex_);
    log_status_.store(log_status::NOT_AVAILABLE);
    if(logfile.is_open()) {
        logfile.close();
    }
}

logger &logger::get_instance() {
   return instance;
}

void logger::init() {
     init_log_file_max_size();
}

bool logger::start_logger() {
    if (log_status_ == log_status::AVAILABLE) {
        return true;
    }

    if (log_status_ == log_status::NOT_AVAILABLE) {
        return false;
    }

    if (log_status_ == log_status::INIT) {
        init();
        log_status_.store(log_status::AVAILABLE);
    }

    return true;
}

void logger::init_log_file_max_size() {
   log_file_max_size_ = DEFAULT_LOG_FILE_MAX_SIZE;
}

ino_t logger::reopen_log_file() {
   struct stat st;
   if (logfile.is_open()) {
      logfile.close();
   }
   logfile.clear();

   logfile.open(FILE_PATH, std::ios::app);
   if (logfile.rdstate() == std::ios_base::goodbit) {
       stat(FILE_PATH, &st);
       return st.st_ino;
   }
   return 0;
}

void logger::write_to_file(std::ostringstream &outputStream)
{
    std::lock_guard<std::mutex> lock(log_mutex_);
    if (log_status_ != log_status::AVAILABLE) {
        return;
    }

    bool log_file_changed = false;
    struct stat st;
    if (stat(FILE_PATH, &st) == STAT_FAILURE) {
        return;
    }

    if(!log_file_changed) {
        if (st.st_size > log_file_max_size_) {
            std::cout << st.st_size << " " << log_file_max_size_ << std::endl;
            //log_file_changed = true;
        }
        std::cout << st.st_size << " " << st.st_ino << std::endl;
        if (inode_number_ != st.st_ino) {
            log_file_changed = true;
        }
    }

    if(log_file_changed) {
        inode_number_ = reopen_log_file();
    }

    if(logfile.rdstate() == std::ios_base::goodbit) {
        logfile << outputStream.str() << std::endl;
    }
}

void logger::write_to_console(std::ostringstream &outputStream) {
   std::cout << outputStream.str() << std::endl;
}

void logger::write_log_msg(std::ostringstream &os, const std::string &file_name,const std::string &line)
{
    std::string full_line = "";
    std::ostringstream output_stream;

    const char *last = strrchr(file_name.c_str(), '/');
    if(last != nullptr) {
        full_line = " " + std::string(last + 1) + "[" + line + "] ";
    } else {
        full_line = " " + std::string(file_name) + "[" + line + "] ";
    }
    output_stream <<  full_line;
    output_stream << std::this_thread::get_id() << ": ";
    if(!os.str().empty()) {
        output_stream << os.str();
        //write_to_file(output_stream);
        write_to_console(output_stream);
    }
}