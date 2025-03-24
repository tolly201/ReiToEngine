#ifndef CORE_HAL_SINGLETON_FACTORY_H
#define CORE_HAL_SINGLETON_FACTORY_H
#include <algorithm>
#include <iostream>
#include <mutex>
#include <type_traits>  // for std::is_same
#include <vector>

#include "Core/HAL/Macro/Macro.h"
#include "Core/Platform/Memory/Include/RTDebugHeapMemoryManager.h"
#include "Core/Platform/Memory/Memory.h"
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
        std::cout << "register\n";
        pass->pNext = instances;
        instances = pass;
        pass->by_pass = true;
        std::cout << "register down\n";
    }

    void register_instance(void* ptr, void (*destroy)(void*)) {
        std::lock_guard<std::mutex> lock(mutex);
        std::cout << "register\n";
        SingletonWrapper* wrapper = new SingletonWrapper();
        wrapper->destructor_ptr = destroy;
        wrapper->instance_ptr = ptr;
        wrapper->pNext = instances;
        instances = wrapper;
        wrapper->by_pass = false;
        std::cout << "register down\n";
    }
};

class SingletonFactory {
   public:
    static SingletonManager& GetSingletonManager()
    {
        static SingletonManager singletonManager;
        return singletonManager;
    };
   protected:
    RTDebugHeapMemoryManager memoryManager;
};


template <typename T>
class RTENGINE_API Singleton {
   private:
    static std::once_flag once_flag;
    static T* instance_ptr;

    static void create_instance() {
        std::cout << "inner_create_instance" << std::endl;
        std::cout << IndependentConstruct << std::endl;
        std::cout << Singleton<T>::IndependentConstruct << std::endl;
        if (IndependentConstruct) {
            SingletonWrapper* wrapper = Singleton<T>::IndependentConstructor();
            instance_ptr = static_cast<T*>(wrapper->instance_ptr);
            wrapper->destructor_ptr = Singleton::destroy_instance;
            SingletonFactory::GetSingletonManager().register_instance(wrapper);
            std::cout << instance_ptr << std::endl;
        } else {
            instance_ptr = new T();
            std::cout << instance_ptr << std::endl;
            SingletonFactory::GetSingletonManager().register_instance(instance_ptr, Singleton::destroy_instance);
        }
        std::cout << instance_ptr << std::endl;
        std::cout << "finish_inner_create_instance" << std::endl;
    }

    static void destroy_instance(void* ptr) {
        std::cout << "inner_destroy_instance" << std::endl;
        delete static_cast<T*>(ptr);
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
        std::cout << "try get a instance()" << std::endl;
        std::call_once(once_flag, create_instance);
        return *instance_ptr;
    }
};

template <typename T>
bool Singleton<T>::IndependentConstruct = false;

template <typename T>
SingletonWrapper* Singleton<T>::IndependentConstructor() {
    std::cout << "default nullptr\n";
    return nullptr;
}

template <typename T>
T* Singleton<T>::instance_ptr = nullptr;

template <typename T>
std::once_flag Singleton<T>::once_flag;

}  // namespace ReiToEngine
#endif
