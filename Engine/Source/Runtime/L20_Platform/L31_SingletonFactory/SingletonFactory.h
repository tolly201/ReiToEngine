#ifndef PLATFORM_SINGLETON_FACTORY_H
#define PLATFORM_SINGLETON_FACTORY_H
#include <algorithm>
#include <iostream>
#include <mutex>
#include <type_traits>  // for std::is_same
#include <vector>

#include "L0_Macro/Include.h"
#include "../L30_MemoryManager/Memory.h"
namespace ReiToEngine {
struct SingletonWrapper {
    void* instance_ptr;
    void (*destructor_ptr)(void*);
    bool by_pass;
    SingletonWrapper* pNext;
};

class SingletonManager {
   private:
    SingletonWrapper* instances;
    std::mutex mutex;
   public:
    SingletonManager() = default;
    SingletonManager(const SingletonManager&) = delete;
    SingletonManager& operator=(const SingletonManager&) = delete;
    ~SingletonManager() {
        std::lock_guard<std::mutex> lock(mutex);
        while (instances) {
            SingletonWrapper* raw = instances;
            instances = instances->pNext;
            void* instance = raw->instance_ptr;
            void (*destructor)(void*) = raw->destructor_ptr;
            if (!raw->by_pass) delete raw;
            destructor(instance);
        }
    }
    void register_instance(SingletonWrapper* pass) {
        std::lock_guard<std::mutex> lock(mutex);
        pass->pNext = instances;
        instances = pass;
        pass->by_pass = true;
    }

    void register_instance(void* ptr, void (*destroy)(void*)) {
        std::lock_guard<std::mutex> lock(mutex);
        SingletonWrapper* wrapper = new SingletonWrapper();
        wrapper->destructor_ptr = destroy;
        wrapper->instance_ptr = ptr;
        wrapper->pNext = instances;
        instances = wrapper;
        wrapper->by_pass = false;
    }
};

class SingletonFactory {
   public:
    static SingletonManager& GetSingletonManager()
    {
        static SingletonManager singletonManager;
        return singletonManager;
    };

    static MemoryManager& GetMemoryManager()
    {
        if (!memoryManager.IsSeted())
        {
            std::call_once(once_flag_default, []{memoryManager.SetDefaultMemoryManager();});

            RT_LOG_WARN("Memory manager not set. Using default Mimalloc memory manager.");
        }
        return memoryManager;
    };
    static b8 SetMemoryManager(void* MM = nullptr, EMEMORY_MANAGER_TYPE type = EMEMORY_MANAGER_TYPE::MIMALLOC)
    {
        RT_ASSERT_MESSAGE(type != EMEMORY_MANAGER_TYPE::DEFAULT, "EMEMORY_MANAGER_TYPE::DEFAULT is not a valid type for setting memory manager. Use a specific type like MIMALLOC, STANDARDC, BINNED, or CUSTOM.");

        if (type == EMEMORY_MANAGER_TYPE::CUSTOM) {
            RT_ASSERT_MESSAGE(MM != nullptr, "When setting a custom memory manager, MM must not be nullptr.");
        } else {
            RT_ASSERT_MESSAGE(MM == nullptr, "When not setting a custom memory manager, MM must be nullptr.");
        }

        RT_ASSERT_MESSAGE(!memoryManager.IsSeted(), "Memory manager can only be set once.");

        std::call_once(once_flag_set, [&]{memoryManager.SetMemoryManager(MM, type);});
        return true;
    };
    protected:
    static MemoryManager memoryManager;
    static std::once_flag once_flag_set;
    static std::once_flag once_flag_default;
};

template <typename T>
class Singleton {
   private:
    static std::once_flag once_flag;
    static T* instance_ptr;

    static void create_instance() {
        if (IndependentConstruct) {
            SingletonWrapper* wrapper = Singleton<T>::IndependentConstructor();
            instance_ptr = static_cast<T*>(wrapper->instance_ptr);
            wrapper->destructor_ptr = Singleton::destroy_instance;
            SingletonFactory::GetSingletonManager().register_instance(wrapper);
        } else {
            void * mem = SingletonFactory::GetMemoryManager().Allocate(sizeof(T), alignof(T), RT_MEMORY_TAG::SINGLETON);
            instance_ptr = new(mem) T();
            SingletonFactory::GetSingletonManager().register_instance(instance_ptr, Singleton::destroy_instance);
        }
    }

    static void destroy_instance(void* ptr) {
            static_cast<T*>(ptr)->~T();
            SingletonFactory::GetMemoryManager().Free(ptr, sizeof(T), RT_MEMORY_TAG::SINGLETON);
        // delete static_cast<T*>(ptr);
    }

    static SingletonWrapper* IndependentConstructor();
    static bool IndependentConstruct;

   protected:
    Singleton() = default;
    ~Singleton() = default;

   public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static T& Instance() {
        RT_ASSERT(instance_ptr != nullptr && "Singleton instance not initialized. Call InitializeInstance() first if using IndependentConstruct.");
        return *instance_ptr;
    }

    static T& InitializeInstance() {
        std::call_once(once_flag, create_instance);
        return *instance_ptr;
    }
};

template <typename T>
bool Singleton<T>::IndependentConstruct = false;

template <typename T>
SingletonWrapper* Singleton<T>::IndependentConstructor() {
    return nullptr;
}

template <typename T>
T* Singleton<T>::instance_ptr = nullptr;

template <typename T>
std::once_flag Singleton<T>::once_flag;

RTENGINE_API MemoryManager& GetMemoryManager();
}  // namespace ReiToEngine


#endif
