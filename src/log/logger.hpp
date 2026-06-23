

#ifndef LOGGER_HPP
#define LOGGER_HPP
#pragma once


struct _iobuf;
typedef _iobuf FILE;



enum LogLevel {
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_NONE
};


class Logger final
{
private:

    FILE *__file;

    LogLevel __min_level;


    Logger(const char *filename = "info.log", LogLevel minLevel = LOG_LEVEL_WARNING);


    void __file_close__() noexcept;
    
    const char* __writeLog__(LogLevel level, const char* message);
    
public:
    ~Logger();


    Logger* getLogger();

    void set_logfile(const char *filename);
    void set_minlivel(LogLevel level);

    void flog(LogLevel level, const char* format, ...);
    void clog(LogLevel level, const char* format, ...);


    Logger(const Logger&) = delete;
    Logger(Logger&& ) = delete;

    Logger& operator=(const Logger&) = delete;
    Logger& operator=(Logger&&) = delete;
};


#endif // LOGGER_HPP