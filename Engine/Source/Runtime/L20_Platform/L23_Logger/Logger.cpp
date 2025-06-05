#include "Logger.h"
#include "Assert.h"

RTENGINE_API void report_assertion_failure(const char* expression, const char* message, const char* file, i32 line)
{
    Log(LOG_LEVEL_FATAL, "Assertion Failure:%s message: '%s', in file: %s, line: %d\n", expression, message, file, line);
}
bool InitializeLog()
{
    return true;
}

void TerminateLog()
{

}
