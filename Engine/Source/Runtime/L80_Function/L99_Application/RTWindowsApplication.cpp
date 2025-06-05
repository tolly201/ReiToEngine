#include "RTWindowsApplication.h"

#ifdef RT_SYSTEM_WINDOWS
using namespace ReiToEngine;

b8 RTWindowsApplication::Initialize(IGame* game)
{
    return RTApplication::Initialize(game);
}

b8 RTWindowsApplication::Run()
{
   return RTApplication::Run();
}

void RTWindowsApplication::Terminate()
{

    RTApplication::Terminate();
    // Unregister the window class
}

b8 RTWindowsApplication::StartGame()
{
    return RTApplication::StartGame();
}

#endif
