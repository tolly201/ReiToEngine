#ifndef CORE_HAL_SINGLETON_NEW_H
#define CORE_HAL_SINGLETON_NEW_H
#include <mutex>
#include <vector>
#include <algorithm>
#include <iostream>
#include "Core/HAL/Macro/Macro.h"
#include <type_traits> // for std::is_same

namespace ReiToEngine
{

class  SingletonManager {
private:
    std::vector<void*> instances;
    std::vector<void(*)(void*)> destructors;
    std::mutex mutex;

    SingletonManager() = default;
    ~SingletonManager() {
        std::lock_guard<std::mutex> lock(mutex);
        for (size_t i = 0; i < instances.size();++i)
        {
            destructors[i](instances[i]);
        }
    }

public:
    SingletonManager(const SingletonManager&) = delete;
    SingletonManager& operator=(const SingletonManager&) = delete;

    static SingletonManager& instance() {
        static SingletonManager manager;
        return manager;
    }

    void register_instance(void* ptr, void(*destroy)(void*)) {
        std::cout << "register\n";
        std::lock_guard<std::mutex> lock(mutex);
        std::cout << "register down\n";
    }
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
        if (IndependentConstruct)
        {
            instance_ptr = Singleton<T>::IndependentConstructor(); // 调用虚函数
            std::cout << instance_ptr << std::endl;
        }
        else instance_ptr = new T();
        std::cout << instance_ptr << std::endl;
        SingletonManager::instance().register_instance(
            instance_ptr,
            Singleton::destroy_instance
        );
        std::cout << instance_ptr << std::endl;
        std::cout << "finish_inner_create_instance" << std::endl;
    }

    static void destroy_instance(void* ptr)
    {
        std::cout << "inner_destroy_instance" << std::endl;
        delete static_cast<T*>(ptr);
    }

    static T* IndependentConstructor();
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
T* Singleton<T>::IndependentConstructor() {
    std::cout << "default nullptr\n";
    return nullptr;
    }

template <typename T>
T* Singleton<T>::instance_ptr = nullptr;

template <typename T>
std::once_flag Singleton<T>::once_flag;

}
#endif
