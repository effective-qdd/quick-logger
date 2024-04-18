#ifndef QUICK_LOGGER_H
#define QUICK_LOGGER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <mutex>
#include <filesystem>


#define LOG_INFO(x, ...) Logger::instance().log(__FILE__, __func__, __LINE__, LogLevel::KInfo, x, ##__VA_ARGS__);
#define LOG_DEBUG(x, ...) Logger::instance().log(__FILE__, __func__, __LINE__, LogLevel::KDebug, x, ##__VA_ARGS__);
#define LOG_ERROR(x, ...) Logger::instance().log(__FILE__, __func__, __LINE__, LogLevel::KError, x, ##__VA_ARGS__);

#define LOG_FUNC_CALL LOG_INFO("Call function")

enum class LogLevel {
    KDisable = 0,
    KInfo,
    KDebug,
    KError,
};

class Logger {
public:
    static Logger& instance();

    bool init();
    void destroy();

    template<class T, class... Args> 
    void log(const std::string &file, const std::string &func, int line,
                LogLevel level, const T &log, const Args&... logs) {
        if(!enable_log_) return;

        lock_.lock();
        std::filesystem::path file_(file);
        log_file_ << log_timestamp() 
            << " (" << file_.filename().string() << "::" << line << "::" << func << ")" 
            << log_level_tag(level)
            << ": ";
        log_logs(log, logs...);
        log_file_ << std::endl;
        lock_.unlock();
    }

private:
    bool enable_log_ = false;
    std::ofstream log_file_;
    std::mutex mutex_;
    std::unique_lock<std::mutex> lock_{mutex_, std::defer_lock};

    template<class T, class... Args>
    void log_logs(const T &log, const Args&... logs) {
        log_file_ << log;
        if constexpr (sizeof...(logs) > 0) {
            log_logs(logs...);
        }
    }

    Logger() = default;
    ~Logger() = default;

    std::string log_name();
    std::string log_timestamp();
    std::string log_level_tag(LogLevel level);
};


#endif // QUICK_LOGGER_H