#include "logger.hpp"

// #include <stdlib.h>
// #include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

/**
 * @brief Размер буфера для временной метки (64 символа достаточно для формата ГГГГ-ММ-ДД ЧЧ:ММ:СС).
 */
constexpr int TYME_STR_SIZE = 64;

/**
 * @brief Размер буфера для итогового отформатированного сообщения.
 */
constexpr int FORMATED_STR_SIZE = 1024;


/**
 * @brief Преобразует значение LogLevel в строковое представление.
 * @param level Уровень логирования.
 * @return Указатель на статическую строку (например, "DEBUG", "INFO" и т.д.).
 * @note Используется внутри __writeLog__ для формирования сообщения.
 *          Функция статическая – видна только в этом файле.
 */
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
        __file = nullptr;
    }
}

const char* Logger::__writeLog__(LogLevel level, const char *message, char *formatted)
{
    char timeStr[TYME_STR_SIZE];
    
    time_t rawtime;
    time(&rawtime);

    // Форматируем время в строку
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", localtime(&rawtime));

    // Собираем итоговое сообщение
    snprintf(formatted, FORMATED_STR_SIZE * sizeof(decltype(FORMATED_STR_SIZE)), "%s: %s: %s\n",
             timeStr, levelToString(level), message);

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
    // закрываем старый файл
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
    
    // форматируем сообщение
    vsnprintf(buffer, sizeof(buffer), format, args);

    va_end(args);

    // буфер для готовой строки с меткой времени
    char formatted[FORMATED_STR_SIZE];

    printf("%s", __writeLog__(level, buffer, formatted));
    fflush(stdout);
}

void Logger::flog(LogLevel level, const char *format, ...)
{
    if (level < this->__min_level || !this->__file) return;

    char buffer[2048];
    va_list args;
    va_start(args, format);

    vsnprintf(buffer, sizeof(buffer), format, args);

    va_end(args);

    char formatted[FORMATED_STR_SIZE];

    fprintf(this->__file, "%s", __writeLog__(level, buffer, formatted));
    fflush(this->__file);
}
