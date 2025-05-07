#include "Launch/LaunchEngineLoop.h"

#include "Core/MinimalCore.h"
int main(int argc, const char * argv[]) {
    // return 0;
    RTFATAL("test", 1, "dad");
    RTASSERT(0 == 0);
    // RTASSERT(1 == 0);
    return RuntimeMainLoopEntry(argc, argv);
    // return 0;
}
