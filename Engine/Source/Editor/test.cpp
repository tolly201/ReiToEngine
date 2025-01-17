#include <iostream>
#include "Core/Common/MemoryManager/MemoryManager.h"
int main()
{
    std::cout << "tes1t";
    ReiToEngine::RTCMemoryManager testm;
    void* data = testm.Allocate(20, 4);
    testm.Deallocate(data, 20, 4);
    return 0;
}