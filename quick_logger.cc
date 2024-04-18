#include <chrono>

#include "quick_logger.h"

Logger& Logger::instance() {
    static Logger singleton;
    return singleton;
}

bool Logger::init() {
    std::filesystem::path log_file_dir(std::filesystem::current_path() / "log");
    if(!std::filesystem::exists(log_file_dir)){
        std::filesystem::create_directories(log_file_dir);
    }
    std::filesystem::path log_file_path(log_file_dir / log_name());
    log_file_.open(log_file_path.string(), std::ios::out | std::ios::app);
    if(!log_file_.is_open()) {
        std::cout << "WARNING: Failed create log file: " << log_file_path.string() << std::endl;
        return false;
    }
    if(log_file_.fail()) {
        std::cout << "WARNING: Failed opening log file." << std::endl;
        return false;
    }

    enable_log_ = true;
    return true;
}

void Logger::destroy() {
    log_file_.close();
}

std::string Logger::log_name() {
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::microseconds>(
        now.time_since_epoch()) % 1000000;
    
    auto t_t = std::chrono::system_clock::to_time_t(now);
    std::tm local_time = *std::localtime(&t_t);

    std::ostringstream oss;
    // YZ_SMI_Lib_ALL_YYYYMMDDHHMMSS.log
    oss << "YZ_SMI_Lib_ALL_";
    oss << std::put_time(&local_time, "%Y%m%d%H%M%S");
    oss << ".log";
    return oss.str();
}

std::string Logger::log_timestamp() {
    auto now = std::chrono::system_clock::now();

    auto ms = std::chrono::duration_cast<std::chrono::microseconds>(
        now.time_since_epoch()) % 1000000;
    
    auto t_t = std::chrono::system_clock::to_time_t(now);
    std::tm local_time = *std::localtime(&t_t);

    std::ostringstream oss;
    // YY-MM-DD HH:MM:SS.microseconds
    oss << std::put_time(&local_time, "%F %T");
    oss << '.' << std::setfill('0') << std::setw(4) << ms.count();
    return oss.str();
}

std::string Logger::log_level_tag(LogLevel level) {
    if(LogLevel::KInfo == level) {
        return "[INFO]";
    }else if(LogLevel::KDebug == level) {
        return "[DEBUG]";
    }else if(LogLevel::KError == level) {
        return "[ERROR]";
    }else {
        return "";
    }
}