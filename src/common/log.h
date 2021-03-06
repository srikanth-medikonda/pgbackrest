/***********************************************************************************************************************************
Log Handler
***********************************************************************************************************************************/
#ifndef COMMON_LOG_H
#define COMMON_LOG_H

#include <stdbool.h>

#include "common/logLevel.h"

/***********************************************************************************************************************************
Max size allowed for a single log message including header
***********************************************************************************************************************************/
#ifndef LOG_BUFFER_SIZE
    #define LOG_BUFFER_SIZE                                         ((size_t)(32 * 1024))
#endif

/***********************************************************************************************************************************
Functions
***********************************************************************************************************************************/
void logInit(
    LogLevel logLevelStdOutParam, LogLevel logLevelStdErrParam, LogLevel logLevelFileParam, bool logTimestampParam,
    unsigned int logProcessMax);
void logClose(void);
bool logFileSet(const char *logFile);

bool logAny(LogLevel logLevel);

LogLevel logLevelEnum(const char *logLevel);
const char *logLevelStr(LogLevel logLevel);

/***********************************************************************************************************************************
Macros

Only call logInternal() if the message will be logged to one of the available outputs.  Also simplify each call site by supplying
commonly-used values.

Note that it's possible that that not all the macros below will appear in the code.  They are included for completeness and future
usage.
***********************************************************************************************************************************/
// Define a macro to test logAny() that can be removed when performing coverage testing.  Checking logAny() saves a function call
// for logging calls that won't be output anywhere, but since the macro then contains a branch it causes coverage problems.
#ifdef DEBUG_COVERAGE
    #define IF_LOG_ANY(logLevel)
#else
    #define IF_LOG_ANY(logLevel)                                                                                                   \
        if (logAny(logLevel))
#endif

#define LOG_INTERNAL(logLevel, logRangeMin, logRangeMax, processId, code, ...)                                                     \
do                                                                                                                                 \
{                                                                                                                                  \
    IF_LOG_ANY(logLevel)                                                                                                           \
    {                                                                                                                              \
        logInternal(logLevel, logRangeMin, logRangeMax, processId, __FILE__, __func__, code, __VA_ARGS__);                         \
    }                                                                                                                              \
} while (0)

#define LOG_PID(logLevel, processId, code, ...)                                                                                    \
    LOG_INTERNAL(logLevel, LOG_LEVEL_MIN, LOG_LEVEL_MAX, processId, code, __VA_ARGS__)

#define LOG_ASSERT_PID(processId, ...)                                                                                             \
    LOG_PID(logLevelAssert, processId, errorTypeCode(&AssertError), __VA_ARGS__)
#define LOG_ERROR_PID(processId, code, ...)                                                                                        \
    LOG_PID(logLevelError, processId, code, __VA_ARGS__)
#define LOG_WARN_PID(processId, ...)                                                                                               \
    LOG_PID(logLevelWarn, processId, 0, __VA_ARGS__)
#define LOG_INFO_PID(processId, ...)                                                                                               \
    LOG_PID(logLevelInfo, processId, 0, __VA_ARGS__)
#define LOG_DETAIL_PID(processId, ...)                                                                                             \
    LOG_PID(logLevelDetail, processId, 0, __VA_ARGS__)
#define LOG_DEBUG_PID(processId, ...)                                                                                              \
    LOG_PID(logLevelDebug, processId, 0, __VA_ARGS__)
#define LOG_TRACE_PID(processId, ...)                                                                                              \
    LOG_PID(logLevelTrace, processId, 0, __VA_ARGS__)

#define LOG(logLevel, code, ...)                                                                                                   \
    LOG_PID(logLevel, 0, code, __VA_ARGS__)

#define LOG_ASSERT(...)                                                                                                            \
    LOG(logLevelAssert, errorTypeCode(&AssertError), __VA_ARGS__)
#define LOG_ERROR(code, ...)                                                                                                       \
    LOG(logLevelError, code, __VA_ARGS__)
#define LOG_WARN(...)                                                                                                              \
    LOG(logLevelWarn, 0, __VA_ARGS__)
#define LOG_INFO(...)                                                                                                              \
    LOG(logLevelInfo, 0, __VA_ARGS__)
#define LOG_DETAIL(...)                                                                                                            \
    LOG(logLevelDetail, 0, __VA_ARGS__)
#define LOG_DEBUG(...)                                                                                                             \
    LOG(logLevelDebug, 0, __VA_ARGS__)
#define LOG_TRACE(...)                                                                                                             \
    LOG(logLevelTrace, 0, __VA_ARGS__)

/***********************************************************************************************************************************
Internal Functions
***********************************************************************************************************************************/
void logInternal(
    LogLevel logLevel, LogLevel logRangeMin,  LogLevel logRangeMax, unsigned int processId, const char *fileName,
    const char *functionName, int code, const char *format, ...);

#endif
