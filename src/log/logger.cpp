#include "logger.hpp"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>



static inline const char* getStrTime()
{
    time_t rawtime;
    struct tm* timeinfo;
    char timeStr[64];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", timeinfo);

    return timeStr;
}

static const char* levelToString(LogLevel level) {
    switch (level) {
        case LOG_LEVEL_DEBUG:   return "DEBUG";
        case LOG_LEVEL_INFO:    return "INFO";
        case LOG_LEVEL_WARNING: return "WARNING";
        case LOG_LEVEL_ERROR:   return "ERROR";
        default:                return "UNKNOWN";
    }
}



Logger::Logger(const char *filename, LogLevel minlevel)
    :__min_level(minlevel)
{
    this->set_logfile(filename);
}

void Logger::__file_close__() noexcept
{
    if (this->__file) {
        fclose(__file);
        __file = NULL;
    }
}

const char* Logger::__writeLog__(LogLevel level, const char *message)
{
    char formatted[1024];

    snprintf(formatted, sizeof(formatted), "[%s] [%s] %s\n",
             getStrTime(), levelToString(level), message);

    return formatted;
}

Logger::~Logger()
{
    __file_close__();
}

Logger *Logger::getLogger()
{
    static Logger logger;
    return &logger;

}

void Logger::set_logfile(const char *filename)
{
    __file_close__();

    this->__file = fopen(filename, "a");

    if (!this->__file)
        fprintf(stderr, "Logger: failed to open log file %s\n", filename);
}

void Logger::set_minlivel(LogLevel level)
{
    this->__min_level = level;
}

void Logger::clog(LogLevel level, const char *format, ...)
{
    if (level < this->__min_level) return;

    char buffer[2048];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    printf("%s", __writeLog__(level, buffer));
    fflush(stdout);
}
